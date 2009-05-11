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

#include <iostream>
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
#include "engine/db/library.hpp"
#include "fwk/toolkit/application.hpp"
#include "fwk/toolkit/configuration.hpp"
#include "fwk/toolkit/notificationcenter.hpp"
#include "fwk/toolkit/configdatabinder.hpp"
#include "fwk/toolkit/undo.hpp"

#include "eog-thumb-view.hpp"
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

Gtk::Widget * 
NiepceWindow::buildWidget()
{
    Gtk::Window & win(gtkWindow());

    Application::Ptr pApp = Application::app();

    m_selection_controller = SelectionController::Ptr(new SelectionController);
    add(m_selection_controller);

    init_actions();
    init_ui();

    m_notifcenter.reset(new niepce::NotificationCenter());

    Glib::ustring name("camera");
    set_icon_from_theme(name);		


    m_notifcenter->signal_lib_notification.connect(
        sigc::mem_fun(*this, &NiepceWindow::on_lib_notification));


    m_notifcenter->signal_lib_notification
        .connect(sigc::mem_fun(
                     *get_pointer(m_selection_controller->list_store()),
                     &ImageListStore::on_lib_notification));
    m_notifcenter->signal_thumbnail_notification
        .connect(sigc::mem_fun(
                     *get_pointer(m_selection_controller->list_store()), 
                     &ImageListStore::on_tnail_notification));

    // main view
    m_mainviewctrl = LibraryMainViewController::Ptr(
        new LibraryMainViewController(m_refActionGroup,
                                      m_selection_controller->list_store()));
    m_notifcenter->signal_lib_notification
        .connect(sigc::mem_fun(
                     *m_mainviewctrl,
                     &LibraryMainViewController::on_lib_notification));

    add(m_mainviewctrl);
    // workspace treeview
    m_workspacectrl = WorkspaceController::Ptr( new WorkspaceController() );

    m_notifcenter->signal_lib_notification
        .connect(sigc::mem_fun(*m_workspacectrl,
                               &WorkspaceController::on_lib_notification));
    add(m_workspacectrl);

    m_hbox.set_border_width(4);
    m_hbox.pack1(*(m_workspacectrl->widget()), Gtk::EXPAND);
    m_hbox.pack2(*(m_mainviewctrl->widget()), Gtk::EXPAND);
    m_databinders.add_binder(new fwk::ConfigDataBinder<int>(m_hbox.property_position(),
                                                                  Application::app()->config(),
                                                                  "workspace_splitter"));

    win.add(m_vbox);

    Gtk::Widget* pMenuBar = pApp->uiManager()->get_widget("/MenuBar");
    m_vbox.pack_start(*pMenuBar, Gtk::PACK_SHRINK);
    m_vbox.pack_start(m_hbox);


    m_filmstrip = FilmStripController::Ptr(new FilmStripController(m_selection_controller->list_store()));
    add(m_filmstrip);

    m_vbox.pack_start(*(m_filmstrip->widget()), Gtk::PACK_SHRINK);

    // status bar
    m_vbox.pack_start(m_statusBar, Gtk::PACK_SHRINK);
    m_statusBar.push(Glib::ustring(_("Ready")));

    m_selection_controller->add_selectable(m_filmstrip.get());
    m_selection_controller->add_selectable(m_mainviewctrl.get());
    m_selection_controller->signal_selected
        .connect(sigc::mem_fun(*m_mainviewctrl,
                               &LibraryMainViewController::on_selected));

    m_selection_controller->signal_activated
        .connect(sigc::mem_fun(*m_mainviewctrl,
                               &LibraryMainViewController::on_image_activated));

    win.set_size_request(600, 400);
    win.show_all_children();
    on_open_library();
    return &win;
}


void NiepceWindow::init_ui()
{
    Application::Ptr pApp = Application::app();
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
        "    <menu action='MenuImage'>"
        "      <menuitem action='PrevImage'/>"
        "      <menuitem action='NextImage'/>"
        "      <separator/>"
        "      <menuitem action='RotateLeft'/>"
        "      <menuitem action='RotateRight'/>"			
        "      <separator/>"
        "      <menu action='SetRating'>"
        "        <menuitem action='SetRating0'/>"
        "        <menuitem action='SetRating1'/>"
        "        <menuitem action='SetRating2'/>"
        "        <menuitem action='SetRating3'/>"
        "        <menuitem action='SetRating4'/>"
        "        <menuitem action='SetRating5'/>"
        "      </menu>"
        "      <menu action='SetLabel'>"
        "        <menuitem action='SetLabel6'/>"
        "        <menuitem action='SetLabel7'/>"
        "        <menuitem action='SetLabel8'/>"
        "        <menuitem action='SetLabel9'/>"
        "        <separator/>"
        "        <menuitem action='EditLabels'/>"
        "      </menu>"
        "      <separator/>"
        "      <menuitem action='DeleteImage'/>"
        "    </menu>"
        "    <menu action='MenuTools'>"
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
    pApp->uiManager()->add_ui_from_string(ui_info);
} 



void NiepceWindow::init_actions()
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
    m_undo_action = Gtk::Action::create("Undo", Gtk::Stock::UNDO);
    m_refActionGroup->add(m_undo_action, Gtk::AccelKey("<control>Z"),
                          sigc::mem_fun(Application::app()->undo_history(),
                                        &UndoHistory::undo));
    Application::app()->undo_history().signal_changed.connect(
        sigc::mem_fun(*this, &NiepceWindow::undo_state));
    undo_state();
    m_redo_action = Gtk::Action::create("Redo", Gtk::Stock::REDO);
    m_refActionGroup->add(m_redo_action, Gtk::AccelKey("<control><shift>Z"),
                          sigc::mem_fun(Application::app()->undo_history(),
                                        &UndoHistory::redo));
    Application::app()->undo_history().signal_changed.connect(
        sigc::mem_fun(*this, &NiepceWindow::redo_state));
    redo_state();

    // FIXME: bind
    m_refActionGroup->add(Gtk::Action::create("Cut", Gtk::Stock::CUT));
    m_refActionGroup->add(Gtk::Action::create("Copy", Gtk::Stock::COPY));
    m_refActionGroup->add(Gtk::Action::create("Paste", Gtk::Stock::PASTE));
    m_refActionGroup->add(Gtk::Action::create("Delete", Gtk::Stock::DELETE));

    m_refActionGroup->add(Gtk::Action::create("Preferences", 
                                              Gtk::Stock::PREFERENCES),
                          sigc::mem_fun(*this,
                                        &NiepceWindow::on_preferences));

    m_refActionGroup->add(Gtk::Action::create("MenuImage", _("_Image")));

    m_refActionGroup->add(Gtk::Action::create("PrevImage", Gtk::Stock::GO_BACK),
                          Gtk::AccelKey(GDK_Left, Gdk::ModifierType(0)),
                          sigc::mem_fun(*m_selection_controller,
                                        &SelectionController::select_previous));
    m_refActionGroup->add(Gtk::Action::create("NextImage", Gtk::Stock::GO_FORWARD),
                          Gtk::AccelKey(GDK_Right, Gdk::ModifierType(0)),
                          sigc::mem_fun(*m_selection_controller,
                                        &SelectionController::select_next));
    
    an_action = Gtk::Action::create("RotateLeft", niepce::Stock::ROTATE_LEFT);
    m_refActionGroup->add(an_action, sigc::bind(
                          sigc::mem_fun(*m_selection_controller,
                                        &SelectionController::rotate), -90));
    an_action = Gtk::Action::create("RotateRight", niepce::Stock::ROTATE_RIGHT);
    m_refActionGroup->add(an_action, sigc::bind(
                          sigc::mem_fun(*m_selection_controller,
                                        &SelectionController::rotate), 90));
    
    m_refActionGroup->add(Gtk::Action::create("SetLabel", _("Set _Label")));
    m_refActionGroup->add(Gtk::Action::create("SetLabel6", _("Label _6")),
                          Gtk::AccelKey("6"), sigc::bind(
                              sigc::mem_fun(*m_selection_controller, 
                                            &SelectionController::set_label),
                              1));
    m_refActionGroup->add(Gtk::Action::create("SetLabel7", _("Label _7")),
                          Gtk::AccelKey("7"), sigc::bind(
                              sigc::mem_fun(*m_selection_controller, 
                                            &SelectionController::set_label),
                              2));
    m_refActionGroup->add(Gtk::Action::create("SetLabel8", _("Label _8")),
                          Gtk::AccelKey("8"), sigc::bind(
                              sigc::mem_fun(*m_selection_controller, 
                                            &SelectionController::set_label),
                              3));
    m_refActionGroup->add(Gtk::Action::create("SetLabel9", _("Label _9")),
                          Gtk::AccelKey("9"), sigc::bind(
                              sigc::mem_fun(*m_selection_controller, 
                                            &SelectionController::set_label),
                              4));
    m_refActionGroup->add(Gtk::Action::create("EditLabels", _("_Edit Labels...")),
                          sigc::mem_fun(*this, &NiepceWindow::on_action_edit_labels));
    
    m_refActionGroup->add(Gtk::Action::create("SetRating", _("Set _Rating")));
    m_refActionGroup->add(Gtk::Action::create("SetRating0", _("_No Rating")),
                          Gtk::AccelKey("0"), sigc::bind(
                              sigc::mem_fun(*m_selection_controller,
                                            &SelectionController::set_rating),
                              0));
    m_refActionGroup->add(Gtk::Action::create("SetRating1", _("_1 Star")),
                          Gtk::AccelKey("1"), sigc::bind(
                              sigc::mem_fun(*m_selection_controller,
                                            &SelectionController::set_rating),
                              1));
    m_refActionGroup->add(Gtk::Action::create("SetRating2", _("_2 Stars")),
                          Gtk::AccelKey("2"), sigc::bind(
                              sigc::mem_fun(*m_selection_controller,
                                            &SelectionController::set_rating),
                              2));
    m_refActionGroup->add(Gtk::Action::create("SetRating3", _("_3 Stars")),
                          Gtk::AccelKey("3"), sigc::bind(
                              sigc::mem_fun(*m_selection_controller,
                                            &SelectionController::set_rating),
                              3));
    m_refActionGroup->add(Gtk::Action::create("SetRating4", _("_4 Stars")),
                          Gtk::AccelKey("4"), sigc::bind(
                              sigc::mem_fun(*m_selection_controller,
                                            &SelectionController::set_rating),
                              4));
    m_refActionGroup->add(Gtk::Action::create("SetRating5", _("_5 Stars")),
                          Gtk::AccelKey("5"), sigc::bind(
                              sigc::mem_fun(*m_selection_controller,
                                            &SelectionController::set_rating),
                              5));
    m_refActionGroup->add(Gtk::Action::create("DeleteImage", Gtk::Stock::DELETE));

    m_refActionGroup->add(Gtk::Action::create("MenuTools", _("_Tools")));
    m_hide_tools_action = Gtk::ToggleAction::create("ToggleToolsVisible",
                                                    _("_Hide tools"));
    m_refActionGroup->add(m_hide_tools_action,
                          sigc::mem_fun(*this, &Frame::toggle_tools_visible));

    m_refActionGroup->add(Gtk::Action::create("MenuHelp", _("_Help")));
    m_refActionGroup->add(Gtk::Action::create("Help", Gtk::Stock::HELP));
    m_refActionGroup->add(Gtk::Action::create("About", Gtk::Stock::ABOUT),
                          sigc::mem_fun(*Application::app(),
                                        &Application::about));

    Application::app()->uiManager()->insert_action_group(m_refActionGroup);		
		
    gtkWindow().add_accel_group(Application::app()
                                ->uiManager()->get_accel_group());
}

void NiepceWindow::undo_state()
{
    fwk::UndoHistory & history(Application::app()->undo_history());
    m_undo_action->set_sensitive(history.has_undo());
    std::string s = history.next_undo();
    m_undo_action->property_label() = Glib::ustring(_("Undo ")) + s;
}


void NiepceWindow::redo_state()
{
    fwk::UndoHistory & history(Application::app()->undo_history());
    m_redo_action->set_sensitive(history.has_redo());
    std::string s = history.next_redo();
    m_redo_action->property_label() = Glib::ustring(_("Redo ")) + s;
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
    open_library(libMoniker);
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
    m_libClient->createLabel(_("Label 1"), "255 0 0");
    m_libClient->createLabel(_("Label 2"), "0 255 0");
    m_libClient->createLabel(_("Label 3"), "0 0 255");
    m_libClient->createLabel(_("Label 4"), "100 100 100");
    m_libClient->createLabel(_("Label 5"), "");
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
        for(eng::Label::List::const_iterator iter = l->begin();
            iter != l->end(); ++iter) {
                
            m_labels.push_back(*iter);
        }
        break;
    }
    case eng::Library::NOTIFY_LABEL_CHANGED:
    {
        const eng::Label::Ptr & l 
            = boost::any_cast<const eng::Label::Ptr &>(ln.param);
        // TODO: will work as long as we have 5 labels or something.
        for(eng::Label::List::iterator iter = m_labels.begin();
            iter != m_labels.end(); ++iter) {

            if((*iter)->id() == l->id()) {
                (*iter)->set_label(l->label());
                (*iter)->set_color(l->color());
            }
        }
        break;
    }
    case eng::Library::NOTIFY_LABEL_DELETED:
    {
        int id = boost::any_cast<int>(ln.param);
        // TODO: will work as long as we have 5 labels or something.
        for(eng::Label::List::iterator iter = m_labels.begin();
            iter != m_labels.end(); ++iter) {

            if((*iter)->id() == id) {
                DBG_OUT("remove label %d", id);
                iter = m_labels.erase(iter);
                break;
            }
        }
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
}

void NiepceWindow::on_action_edit_labels()
{
    DBG_OUT("edit labels");
    // get the labels.
    EditLabels::Ptr dlg(new EditLabels(get_labels(), getLibraryClient()));
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
