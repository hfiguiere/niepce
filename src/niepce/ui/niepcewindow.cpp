/*
 * niepce - ui/niepcewindow.cpp
 *
 * Copyright (C) 2007-2009 Hubert Figuiere
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <string>
#include <boost/bind.hpp>

#include <glibmm/i18n.h>
#include <gtkmm/window.h>
#include <gtkmm/action.h>
#include <gtkmm/box.h>
#include <gtkmm/stock.h>
#include <gtkmm/separator.h>
#include <gtkmm/filechooserdialog.h>

#include "niepce/notifications.hpp"
#include "niepce/notificationcenter.hpp"
#include "niepce/stock.hpp"
#include "fwk/base/debug.hpp"
#include "fwk/base/moniker.hpp"
#include "fwk/utils/boost.hpp"
#include "fwk/toolkit/application.hpp"
#include "fwk/toolkit/configuration.hpp"
#include "fwk/toolkit/notificationcenter.hpp"
#include "fwk/toolkit/configdatabinder.hpp"
#include "fwk/toolkit/undo.hpp"
#include "engine/db/library.hpp"
#include "libraryclient/uidataprovider.hpp"

#include "thumbstripview.hpp"
#include "niepcewindow.hpp"
#include "dialogs/importdialog.hpp"
#include "dialogs/preferencesdialog.hpp"
#include "dialogs/editlabels.hpp"
#include "selectioncontroller.hpp"

using libraryclient::LibraryClient;
using fwk::Application;
using fwk::Configuration;
using fwk::UndoHistory;

namespace ui {


NiepceWindow::NiepceWindow()
    : fwk::Frame("mainWindow-frame")
{
}


NiepceWindow::~NiepceWindow()
{
    Application::Ptr pApp = Application::app();
    pApp->uiManager()->remove_action_group(m_refActionGroup);
}

void 
NiepceWindow::_createModuleShell()
{
    DBG_ASSERT(m_uimanager, "UI manager NULL");
    DBG_ASSERT(m_libClient, "libclient not initialized");
    DBG_ASSERT(m_widget, "widget not built");

    DBG_OUT("creating module shell");

    // main view
    m_moduleshell = ModuleShell::Ptr(
        new ModuleShell(getLibraryClient()));
    m_moduleshell->buildWidget(m_uimanager);

    add(m_moduleshell);

    m_notifcenter->signal_lib_notification
        .connect(sigc::mem_fun(
                     *m_moduleshell->get_gridview(),
                     &GridViewModule::on_lib_notification));
    m_notifcenter->signal_lib_notification
        .connect(sigc::mem_fun(
                     *get_pointer(m_moduleshell->get_list_store()),
                     &ImageListStore::on_lib_notification));
    m_notifcenter->signal_thumbnail_notification
        .connect(sigc::mem_fun(
                     *get_pointer(m_moduleshell->get_list_store()), 
                     &ImageListStore::on_tnail_notification));


    // workspace treeview
    m_workspacectrl = WorkspaceController::Ptr( new WorkspaceController() );

    m_notifcenter->signal_lib_notification
        .connect(sigc::mem_fun(*m_workspacectrl,
                               &WorkspaceController::on_lib_notification));
    add(m_workspacectrl);

    m_hbox.set_border_width(4);
    m_hbox.pack1(*(m_workspacectrl->buildWidget(m_uimanager)), Gtk::EXPAND);
    m_hbox.pack2(*(m_moduleshell->buildWidget(m_uimanager)), Gtk::EXPAND);
    m_databinders.add_binder(new fwk::ConfigDataBinder<int>(m_hbox.property_position(),
                                                                  Application::app()->config(),
                                                                  "workspace_splitter"));

    static_cast<Gtk::Window*>(m_widget)->add(m_vbox);

    Gtk::Widget* pMenuBar = m_uimanager->get_widget("/MenuBar");
    m_vbox.pack_start(*pMenuBar, Gtk::PACK_SHRINK);
    m_vbox.pack_start(m_hbox);


    SelectionController::Ptr selection_controller = m_moduleshell->get_selection_controller();
    m_filmstrip = FilmStripController::Ptr(new FilmStripController(m_moduleshell->get_list_store()));
    add(m_filmstrip);

    m_vbox.pack_start(*(m_filmstrip->buildWidget(m_uimanager)), Gtk::PACK_SHRINK);

    // status bar
    m_vbox.pack_start(m_statusBar, Gtk::PACK_SHRINK);
    m_statusBar.push(Glib::ustring(_("Ready")));

    selection_controller->add_selectable(m_filmstrip.get());

    m_vbox.show_all_children();
    m_vbox.show();
}


Gtk::Widget * 
NiepceWindow::buildWidget(const Glib::RefPtr<Gtk::UIManager> & manager)
{
    DBG_ASSERT(manager, "manager is NULL");

    if(m_widget) {
        return m_widget;
    }
    m_uimanager = manager;
    Gtk::Window & win(gtkWindow());

    m_widget = &win;

    init_actions(manager);
    init_ui(manager);

    m_notifcenter.reset(new niepce::NotificationCenter());

    Glib::ustring name("camera");
    set_icon_from_theme(name);		


    m_notifcenter->signal_lib_notification.connect(
        sigc::mem_fun(*this, &NiepceWindow::on_lib_notification));

    win.set_size_request(600, 400);
    win.show_all_children();
    on_open_library();
    return &win;
}


void NiepceWindow::init_ui(const Glib::RefPtr<Gtk::UIManager> & manager)
{
    Glib::ustring ui_info =
        "<ui>"
        "  <menubar name='MenuBar'>"
        "    <menu action='MenuLibrary'>"
        "      <menuitem action='NewLibrary' />"
        "      <menuitem action='OpenLibrary' />"
        "      <separator/>"
        "      <menuitem action='NewFolder'/>"
        "      <menuitem action='NewProject'/>"
        "      <menuitem action='Import'/>"
        "      <separator/>"
        "      <menuitem action='Close'/>"
        "      <menuitem action='Quit'/>"
        "    </menu>"
        "    <menu action='MenuEdit'>"
        "      <menuitem action='Undo'/>"
        "      <menuitem action='Redo'/>"
        "      <separator/>"
        "      <menuitem action='Cut'/>"
        "      <menuitem action='Copy'/>"
        "      <menuitem action='Paste'/>"
        "      <menuitem action='Delete'/>"
        "      <separator/>"
        "      <menuitem action='Preferences'/>"
        "    </menu>"
        "    <menu action='MenuImage' />"
        "    <menu action='MenuTools'>"
        "      <menuitem action='EditLabels'/>"
        "      <separator/>"        
        "      <menuitem action='ToggleToolsVisible'/>"
        "      <separator/>"        
        "    </menu>"
        "    <menu action='MenuHelp'>"
        "      <menuitem action='Help'/>"
        "      <menuitem action='About'/>"
        "    </menu>"
        "  </menubar>"
        "  <toolbar  name='ToolBar'>"
        "    <toolitem action='Import'/>"
        "    <toolitem action='Quit'/>"
        "  </toolbar>"
        "</ui>";
    m_ui_merge_id = manager->add_ui_from_string(ui_info);
} 



void NiepceWindow::init_actions(const Glib::RefPtr<Gtk::UIManager> & manager)
{
    Glib::RefPtr<Gtk::Action> an_action;

    m_refActionGroup = Gtk::ActionGroup::create();
		
    m_refActionGroup->add(Gtk::Action::create("MenuLibrary", _("_Library")));
    m_refActionGroup->add(Gtk::Action::create("NewLibrary", Gtk::Stock::NEW));
    m_refActionGroup->add(Gtk::Action::create("OpenLibrary", Gtk::Stock::OPEN),
                          sigc::mem_fun(*this, 
                                        &NiepceWindow::on_action_file_open));

    m_refActionGroup->add(Gtk::Action::create("NewFolder", _("New _Folder...")));
    m_refActionGroup->add(Gtk::Action::create("NewProject", _("New _Project...")));

    m_refActionGroup->add(Gtk::Action::create("Import", _("_Import...")),
                          sigc::mem_fun(*this, 
                                        &NiepceWindow::on_action_file_import));
    m_refActionGroup->add(Gtk::Action::create("Close", Gtk::Stock::CLOSE),
                          sigc::mem_fun(gtkWindow(), 
                                        &Gtk::Window::hide));			
    m_refActionGroup->add(Gtk::Action::create("Quit", Gtk::Stock::QUIT),
                          sigc::mem_fun(*Application::app(), 
                                        &Application::quit));	

    m_refActionGroup->add(Gtk::Action::create("MenuEdit", _("_Edit")));

    create_undo_action(m_refActionGroup);
    create_redo_action(m_refActionGroup);

    // FIXME: bind
    m_refActionGroup->add(Gtk::Action::create("Cut", Gtk::Stock::CUT));
    m_refActionGroup->add(Gtk::Action::create("Copy", Gtk::Stock::COPY));
    m_refActionGroup->add(Gtk::Action::create("Paste", Gtk::Stock::PASTE));
    m_refActionGroup->add(Gtk::Action::create("Delete", Gtk::Stock::DELETE));

    m_refActionGroup->add(Gtk::Action::create("Preferences", 
                                              Gtk::Stock::PREFERENCES),
                          sigc::mem_fun(*this,
                                        &NiepceWindow::on_preferences));




    m_refActionGroup->add(Gtk::Action::create("MenuTools", _("_Tools")));
    m_refActionGroup->add(Gtk::Action::create("EditLabels", _("_Edit Labels...")),
                          sigc::mem_fun(*this, &NiepceWindow::on_action_edit_labels));
    m_hide_tools_action = Gtk::ToggleAction::create("ToggleToolsVisible",
                                                    _("_Hide tools"));
    m_refActionGroup->add(m_hide_tools_action,
                          sigc::mem_fun(*this, &Frame::toggle_tools_visible));

    m_refActionGroup->add(Gtk::Action::create("MenuHelp", _("_Help")));
    m_refActionGroup->add(Gtk::Action::create("Help", Gtk::Stock::HELP));
    m_refActionGroup->add(Gtk::Action::create("About", Gtk::Stock::ABOUT),
                          sigc::mem_fun(*Application::app(),
                                        &Application::about));

    manager->insert_action_group(m_refActionGroup);		
		
    gtkWindow().add_accel_group(manager->get_accel_group());
}


void NiepceWindow::on_action_file_import()
{
    int result;
    Configuration & cfg = Application::app()->config();
	
    ImportDialog::Ptr import_dialog(new ImportDialog());

    result = import_dialog->run_modal(shared_frame_ptr());
    switch(result) {
    case 0:
    {
        // import
        const std::list<std::string> & to_import(import_dialog->to_import());
        if(!to_import.empty()) {
            cfg.setValue("last_import_location", to_import.front());
			
            //DBG_OUT("%s", to_import.c_str());
            std::for_each(to_import.begin(), to_import.end(),
                          boost::bind(&LibraryClient::importFromDirectory,
                                      m_libClient, _1, false));
        }
        break;
    }
    case 1:
        // cancel
        break;
    default:
        break;
    }
}



void NiepceWindow::on_action_file_quit()
{
    Application::app()->quit();
}


void NiepceWindow::on_action_file_open()
{
    DBG_OUT("there");
}

void NiepceWindow::on_open_library()
{
    Configuration & cfg = Application::app()->config();
    std::string libMoniker;
    int reopen = 0;
    try {
        reopen = boost::lexical_cast<int>(cfg.getValue("reopen_last_library", "0"));
    }
    catch(...)
    {
    }
    if(reopen) {
        libMoniker = cfg.getValue("lastOpenLibrary", "");
    }
    if(libMoniker.empty()) {
        Gtk::FileChooserDialog dialog(gtkWindow(), _("Create library"),
                                      Gtk::FILE_CHOOSER_ACTION_CREATE_FOLDER);
			
        dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
        dialog.add_button(_("Create"), Gtk::RESPONSE_OK);

        int result = dialog.run();
        Glib::ustring libraryToCreate;
        switch(result)
        {
        case Gtk::RESPONSE_OK:
            libraryToCreate = dialog.get_filename();
            // pass it to the library
            libMoniker = "local:";
            libMoniker += libraryToCreate.c_str();
            cfg.setValue("lastOpenLibrary", libMoniker);
            DBG_OUT("created library %s", libMoniker.c_str());
            break;
        default:
            break;
        }
			
    }
    else {
        DBG_OUT("last library is %s", libMoniker.c_str());
    }
    if(!libMoniker.empty()) {
        open_library(libMoniker);
    }
}




void NiepceWindow::on_preferences()
{
    DBG_OUT("on_preferences");

    fwk::Dialog::Ptr dlg(new PreferencesDialog());
    dlg->run_modal(shared_frame_ptr());

    DBG_OUT("end on_preferences");
}



void NiepceWindow::create_initial_labels()
{
    // TODO make this parametric from resources
    m_libClient->createLabel(_("Label 1"), fwk::RgbColor(55769, 9509, 4369).to_string()); /* 217, 37, 17 */
    m_libClient->createLabel(_("Label 2"), fwk::RgbColor(24929, 55769, 4369).to_string()); /* 97, 217, 17 */
    m_libClient->createLabel(_("Label 3"), fwk::RgbColor(4369, 50629, 55769).to_string()); /* 17, 197, 217 */
    m_libClient->createLabel(_("Label 4"), fwk::RgbColor(35209, 4369, 55769).to_string()); /* 137, 17, 217 */
    m_libClient->createLabel(_("Label 5"), fwk::RgbColor(55769, 35209, 4369).to_string()); /* 217, 137, 17 */
}


void NiepceWindow::on_lib_notification(const eng::LibNotification & ln)
{
    switch(ln.type) {
    case eng::Library::NOTIFY_NEW_LIBRARY_CREATED:
        create_initial_labels();
        break;
    case eng::Library::NOTIFY_ADDED_LABELS:
    {
        eng::Label::ListPtr l 
            = boost::any_cast<eng::Label::ListPtr>(ln.param);
        m_libClient->getDataProvider()->addLabels(l);
        break;
    }
    case eng::Library::NOTIFY_LABEL_CHANGED:
    {
        const eng::Label::Ptr & l 
            = boost::any_cast<const eng::Label::Ptr &>(ln.param);
        m_libClient->getDataProvider()->updateLabel(l);
        break;
    }
    case eng::Library::NOTIFY_LABEL_DELETED:
    {
        int id = boost::any_cast<int>(ln.param);
        m_libClient->getDataProvider()->deleteLabel(id);
        break;
    }
    default:
        break;
    }
}


void NiepceWindow::open_library(const std::string & libMoniker)
{
    m_libClient = LibraryClient::Ptr(new LibraryClient(fwk::Moniker(libMoniker),
                                                       m_notifcenter));
    set_title(libMoniker);
    m_libClient->getAllLabels();
    if(!m_moduleshell) {
        _createModuleShell();
    }
}

void NiepceWindow::on_action_edit_labels()
{
    DBG_OUT("edit labels");
    // get the labels.
    EditLabels::Ptr dlg(new EditLabels(getLibraryClient()));
    dlg->run_modal(shared_frame_ptr());
}

void NiepceWindow::set_title(const std::string & title)
{
    Frame::set_title(_("Niepce Digital - ") + title);
}


}



/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:80
  End:
*/
