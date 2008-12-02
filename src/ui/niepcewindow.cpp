/*
 * niepce - ui/niepcewindow.cpp
 *
 * Copyright (C) 2007-2008 Hubert Figuiere
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
#include <gtkmm/combobox.h>

#include "niepce/notifications.h"
#include "niepce/stock.h"
#include "utils/debug.h"
#include "utils/moniker.h"
#include "utils/boost.h"
#include "db/library.h"
#include "libraryclient/libraryclient.h"
#include "framework/application.h"
#include "framework/configuration.h"
#include "framework/notificationcenter.h"
#include "framework/configdatabinder.h"
#include "framework/undo.h"

#include "eog-thumb-view.h"
#include "niepcewindow.h"
#include "librarymainviewcontroller.h"
#include "importdialog.h"
#include "selectioncontroller.h"

using libraryclient::LibraryClient;
using framework::Application;
using framework::Configuration;
using framework::NotificationCenter;
using framework::UndoHistory;

namespace ui {


NiepceWindow::NiepceWindow()
    : framework::Frame("mainWindow-frame")
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

    m_lib_notifcenter.reset(new NotificationCenter());

    Glib::ustring name("camera");
    set_icon_from_theme(name);		

    m_lib_notifcenter->subscribe(niepce::NOTIFICATION_LIB,
                                 boost::bind(&ImageListStore::on_lib_notification, 
                                             m_selection_controller->list_store(), _1));
    m_lib_notifcenter->subscribe(niepce::NOTIFICATION_THUMBNAIL,
                                 boost::bind(&ImageListStore::on_tnail_notification, 
                                             m_selection_controller->list_store(), _1));

    // main view
    m_mainviewctrl = LibraryMainViewController::Ptr(
        new LibraryMainViewController(m_refActionGroup,
                                      m_selection_controller->list_store()));
    m_lib_notifcenter->subscribe(niepce::NOTIFICATION_LIB,
                                 boost::bind(&LibraryMainViewController::on_lib_notification, 
                                             BIND_SHARED_PTR(LibraryMainViewController, m_mainviewctrl)
                                             , _1));
    add(m_mainviewctrl);
    // workspace treeview
    m_workspacectrl = WorkspaceController::Ptr( new WorkspaceController() );
    m_lib_notifcenter->subscribe(niepce::NOTIFICATION_LIB,
                                 boost::bind(&WorkspaceController::on_lib_notification, 
                                             m_workspacectrl, _1));
    m_lib_notifcenter->subscribe(niepce::NOTIFICATION_COUNT,
                                 boost::bind(&WorkspaceController::on_count_notification,
                                             m_workspacectrl, _1));
    add(m_workspacectrl);

    m_hbox.set_border_width(4);
    m_hbox.pack1(*(m_workspacectrl->widget()), Gtk::EXPAND);
    m_hbox.pack2(*(m_mainviewctrl->widget()), Gtk::EXPAND);
    m_databinders.add_binder(new framework::ConfigDataBinder<int>(m_hbox.property_position(),
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
        .connect(boost::bind(&LibraryMainViewController::on_selected,
                             BIND_SHARED_PTR(LibraryMainViewController, m_mainviewctrl)
                             , _1));

    m_selection_controller->signal_activated
        .connect(boost::bind(&LibraryMainViewController::on_image_activated,
                             BIND_SHARED_PTR(LibraryMainViewController, m_mainviewctrl)
                             , _1));

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
                          boost::bind(&NiepceWindow::on_action_file_open,
                                      this));

    m_refActionGroup->add(Gtk::Action::create("NewFolder", _("New _Folder...")));
    m_refActionGroup->add(Gtk::Action::create("NewProject", _("New _Project...")));

    m_refActionGroup->add(Gtk::Action::create("Import", _("_Import...")),
                          sigc::mem_fun(this, 
                                        &NiepceWindow::on_action_file_import));
    m_refActionGroup->add(Gtk::Action::create("Close", Gtk::Stock::CLOSE),
                          sigc::mem_fun(gtkWindow(), 
                                        &Gtk::Window::hide));			
    m_refActionGroup->add(Gtk::Action::create("Quit", Gtk::Stock::QUIT),
                          sigc::mem_fun(*Application::app(), 
                                        &Application::quit));	

    m_refActionGroup->add(Gtk::Action::create("MenuEdit", _("_Edit")));
    Glib::RefPtr<Gtk::Action> undo_action
        = Gtk::Action::create("Undo", Gtk::Stock::UNDO);
    m_refActionGroup->add(undo_action, Gtk::AccelKey("<control>Z"),
                          boost::bind(&UndoHistory::undo,
                                      boost::ref(Application::app()->undo_history())));
    m_undostate_conn = Application::app()->undo_history().changed.connect(
        boost::bind(&NiepceWindow::undo_state, this, undo_action));
    undo_state(undo_action);
    Glib::RefPtr<Gtk::Action> redo_action
        = Gtk::Action::create("Redo", Gtk::Stock::REDO);
    m_refActionGroup->add(redo_action, Gtk::AccelKey("<control><shift>Z"),
                          boost::bind(&UndoHistory::redo,
                                      boost::ref(Application::app()->undo_history())));
    m_redostate_conn = Application::app()->undo_history().changed.connect(
        boost::bind(&NiepceWindow::redo_state, this, redo_action));
    redo_state(redo_action);

    // FIXME: bind
    m_refActionGroup->add(Gtk::Action::create("Cut", Gtk::Stock::CUT));
    m_refActionGroup->add(Gtk::Action::create("Copy", Gtk::Stock::COPY));
    m_refActionGroup->add(Gtk::Action::create("Paste", Gtk::Stock::PASTE));
    m_refActionGroup->add(Gtk::Action::create("Delete", Gtk::Stock::DELETE));

    m_refActionGroup->add(Gtk::Action::create("Preferences", 
                                              Gtk::Stock::PREFERENCES),
                          sigc::mem_fun(this,
                                        &NiepceWindow::on_preferences));

    m_refActionGroup->add(Gtk::Action::create("MenuImage", _("_Image")));

    m_refActionGroup->add(Gtk::Action::create("PrevImage", Gtk::Stock::GO_BACK),
                          Gtk::AccelKey(GDK_Left, Gdk::ModifierType(0)),
                          boost::bind(&SelectionController::select_previous, 
                                      BIND_SHARED_PTR(SelectionController, m_selection_controller)));
    m_refActionGroup->add(Gtk::Action::create("NextImage", Gtk::Stock::GO_FORWARD),
                          Gtk::AccelKey(GDK_Right, Gdk::ModifierType(0)),
                          boost::bind(&SelectionController::select_next, 
                                      BIND_SHARED_PTR(SelectionController, m_selection_controller)));
    
    an_action = Gtk::Action::create("RotateLeft", niepce::Stock::ROTATE_LEFT);
    m_refActionGroup->add(an_action, boost::bind(&SelectionController::rotate, 
                                                 BIND_SHARED_PTR(SelectionController, m_selection_controller)
                                                 , -90));
    an_action = Gtk::Action::create("RotateRight", niepce::Stock::ROTATE_RIGHT);
    m_refActionGroup->add(an_action, boost::bind(&SelectionController::rotate, 
                                                 BIND_SHARED_PTR(SelectionController, m_selection_controller)
                                                 , 90));
    
    m_refActionGroup->add(Gtk::Action::create("SetLabel", _("Set _Label")));
    m_refActionGroup->add(Gtk::Action::create("SetLabel6", _("Label _6")),
                          Gtk::AccelKey("6"),
                          boost::bind(&SelectionController::set_label, 
                                      BIND_SHARED_PTR(SelectionController, m_selection_controller)
                                      , 1));
    m_refActionGroup->add(Gtk::Action::create("SetLabel7", _("Label _7")),
                          Gtk::AccelKey("7"),
                          boost::bind(&SelectionController::set_label, 
                                      BIND_SHARED_PTR(SelectionController, m_selection_controller)
                                      , 2));
    m_refActionGroup->add(Gtk::Action::create("SetLabel8", _("Label _8")),
                          Gtk::AccelKey("8"),
                          boost::bind(&SelectionController::set_label, 
                                      BIND_SHARED_PTR(SelectionController, m_selection_controller)
                                      , 3));
    m_refActionGroup->add(Gtk::Action::create("SetLabel9", _("Label _9")),
                          Gtk::AccelKey("9"),
                          boost::bind(&SelectionController::set_label, 
                                      BIND_SHARED_PTR(SelectionController, m_selection_controller)
                                      , 4));
    
    m_refActionGroup->add(Gtk::Action::create("SetRating", _("Set _Rating")));
    m_refActionGroup->add(Gtk::Action::create("SetRating0", _("_No Rating")),
                          Gtk::AccelKey("0"),
                          boost::bind(&SelectionController::set_rating, 
                                      BIND_SHARED_PTR(SelectionController, m_selection_controller), 0));
    m_refActionGroup->add(Gtk::Action::create("SetRating1", _("_1 Star")),
                          Gtk::AccelKey("1"),
                          boost::bind(&SelectionController::set_rating, 
                                      BIND_SHARED_PTR(SelectionController, m_selection_controller), 1));
    m_refActionGroup->add(Gtk::Action::create("SetRating2", _("_2 Stars")),
                          Gtk::AccelKey("2"),
                          boost::bind(&SelectionController::set_rating, 
                                      BIND_SHARED_PTR(SelectionController, m_selection_controller), 2));
    m_refActionGroup->add(Gtk::Action::create("SetRating3", _("_3 Stars")),
                          Gtk::AccelKey("3"),
                          boost::bind(&SelectionController::set_rating, 
                                      BIND_SHARED_PTR(SelectionController, m_selection_controller), 3));
    m_refActionGroup->add(Gtk::Action::create("SetRating4", _("_4 Stars")),
                          Gtk::AccelKey("4"),
                          boost::bind(&SelectionController::set_rating, 
                                      BIND_SHARED_PTR(SelectionController, m_selection_controller), 4));
    m_refActionGroup->add(Gtk::Action::create("SetRating5", _("_5 Stars")),
                          Gtk::AccelKey("5"),
                          boost::bind(&SelectionController::set_rating, 
                                      BIND_SHARED_PTR(SelectionController, m_selection_controller), 5));

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

void NiepceWindow::undo_state(const Glib::RefPtr<Gtk::Action> & action)
{
    framework::UndoHistory & history(Application::app()->undo_history());
    action->set_sensitive(history.has_undo());
    std::string s = history.next_undo();
    action->property_label() = Glib::ustring(_("Undo ")) + s;
}


void NiepceWindow::redo_state(const Glib::RefPtr<Gtk::Action> & action)
{
    framework::UndoHistory & history(Application::app()->undo_history());
    action->set_sensitive(history.has_redo());
    std::string s = history.next_redo();
    action->property_label() = Glib::ustring(_("Redo ")) + s;
}

void NiepceWindow::on_action_file_import()
{
    int result;
    Configuration & cfg = Application::app()->config();
	
    ImportDialog::Ptr import_dialog(new ImportDialog());

    result = show_modal_dialog(
        *static_cast<Gtk::Dialog*>(import_dialog->buildWidget()));
    switch(result) {
    case 0:
    {
        // import
        const Glib::ustring & to_import(import_dialog->to_import());
        if(!to_import.empty()) {
            cfg.setValue("last_import_location", to_import);
			
            DBG_OUT("%s", to_import.c_str());
            m_libClient->importFromDirectory(to_import, false);
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


void NiepceWindow::preference_dialog_setup(const Glib::RefPtr<Gnome::Glade::Xml> & xml, Gtk::Dialog * dialog)
{
    Gtk::ComboBox * theme_combo = NULL;
    Gtk::CheckButton * reopen_checkbutton = NULL;
    utils::DataBinderPool * binder_pool = new utils::DataBinderPool();

    dialog->signal_hide().connect(boost::bind(&utils::DataBinderPool::destroy, 
                                              binder_pool));
		
    theme_combo = xml->get_widget("theme_combo", theme_combo);
 
// Why are ComboBox so complicated to use?
//    class Columns : 
//            public Gtk::TreeModel::ColumnRecord
//    {
//    public:
//        Gtk::TreeModelColumn<Glib::ustring>  label;
//        Columns() { add(label); }
//    };

//    Columns columns;
//    Glib::RefPtr<Gtk::ListStore> model(Gtk::ListStore::create(columns));

//    theme_combo->set_model(model);
//    const std::vector<framework::Application::ThemeDesc> & themes = framework::Application::app()->get_available_themes();
//    std::vector<framework::Application::ThemeDesc>::const_iterator i;
//    for(i = themes.begin(); i != themes.end(); ++i) {
//        DBG_OUT("adding %s", i->first.c_str());
//        Gtk::TreeIter iter = model->append();
//        (*iter).set_value(columns.label, i->first); 
//    }
    theme_combo->set_active(framework::Application::app()
                            ->get_use_custom_theme());
    theme_combo->signal_changed().connect(
        boost::bind(&framework::Application::set_use_custom_theme,
                    framework::Application::app(),
                    theme_combo->property_active()));

    reopen_checkbutton = xml->get_widget("reopen_checkbutton", reopen_checkbutton);
    binder_pool->add_binder(new framework::ConfigDataBinder<bool>(
                                reopen_checkbutton->property_active(),
                                framework::Application::app()->config(),
                                "reopen_last_library"));
}


void NiepceWindow::on_preferences()
{
    DBG_OUT("on_preferences");
    show_modal_dialog(GLADEDIR"preferences.glade", "preferences",
                      boost::bind(&NiepceWindow::preference_dialog_setup,
                                  this, _1, _2));
    DBG_OUT("end on_preferences");
}


void NiepceWindow::open_library(const std::string & libMoniker)
{
    m_libClient = LibraryClient::Ptr(new LibraryClient(utils::Moniker(libMoniker),
                                                       m_lib_notifcenter));
    set_title(libMoniker);
}


void NiepceWindow::set_title(const std::string & title)
{
    Frame::set_title(std::string(_("Niepce Digital - ")) + title);
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
