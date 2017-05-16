/*
 * niepce - ui/niepcewindow.cpp
 *
 * Copyright (C) 2007-2015 Hubert Figuière
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

#include <glibmm/i18n.h>
#include <glibmm/miscutils.h>
#include <giomm/menu.h>
#include <gtkmm/window.h>
#include <gtkmm/accelkey.h>
#include <gtkmm/action.h>
#include <gtkmm/separator.h>
#include <gtkmm/filechooserdialog.h>

#include "niepce/notifications.hpp"
#include "niepce/notificationcenter.hpp"
#include "fwk/base/debug.hpp"
#include "fwk/base/moniker.hpp"
#include "fwk/utils/boost.hpp"
#include "fwk/toolkit/application.hpp"
#include "fwk/toolkit/configuration.hpp"
#include "fwk/toolkit/notificationcenter.hpp"
#include "fwk/toolkit/configdatabinder.hpp"
#include "fwk/toolkit/undo.hpp"
#include "engine/db/library.hpp"
#include "engine/importer/iimporter.hpp"
#include "libraryclient/uidataprovider.hpp"

#include "thumbstripview.hpp"
#include "niepcewindow.hpp"
#include "dialogs/importdialog.hpp"
#include "dialogs/editlabels.hpp"
#include "selectioncontroller.hpp"

using libraryclient::LibraryClient;
using libraryclient::LibraryClientPtr;
using fwk::Application;
using fwk::Configuration;
using fwk::UndoHistory;

namespace ui {


NiepceWindow::NiepceWindow()
    : fwk::AppFrame("mainWindow-frame")
    , m_vbox(Gtk::ORIENTATION_VERTICAL)
    , m_hbox(Gtk::ORIENTATION_HORIZONTAL)
{
    // headerbar.
    Gtk::HeaderBar *header = Gtk::manage(new Gtk::HeaderBar);
    header->set_show_close_button(true);
    header->set_has_subtitle(true);
    setHeaderBar(header);
}


NiepceWindow::~NiepceWindow()
{
}

void
NiepceWindow::_createModuleShell()
{
    DBG_ASSERT(static_cast<bool>(m_libClient), "libclient not initialized");
    DBG_ASSERT(m_widget, "widget not built");

    DBG_OUT("creating module shell");

    // main view
    m_moduleshell = ModuleShell::Ptr(
        new ModuleShell(getLibraryClient()));
    m_moduleshell->buildWidget();

    add(m_moduleshell);

    m_notifcenter->signal_lib_notification
        .connect(sigc::mem_fun(
                     *m_moduleshell->get_gridview(),
                     &GridViewModule::on_lib_notification));
    m_notifcenter->signal_lib_notification
        .connect(sigc::mem_fun(
                     *get_pointer(m_moduleshell->get_list_store()),
                     &ImageListStore::on_lib_notification));
    m_notifcenter->signal_lib_notification
        .connect(sigc::mem_fun(
                     *m_moduleshell->get_map_module(),
                     &mapm::MapModule::on_lib_notification));
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
    m_hbox.pack1(*(m_workspacectrl->buildWidget()), Gtk::EXPAND);
    m_hbox.pack2(*(m_moduleshell->buildWidget()), Gtk::EXPAND);
    m_databinders.add_binder(new fwk::ConfigDataBinder<int>(m_hbox.property_position(),
                                                                  Application::app()->config(),
                                                                  "workspace_splitter"));

    static_cast<Gtk::Window*>(m_widget)->add(m_vbox);

    static_cast<Gtk::ApplicationWindow&>(gtkWindow()).set_show_menubar(true);
    m_vbox.pack_start(m_hbox);


    SelectionController::Ptr selection_controller = m_moduleshell->get_selection_controller();
    m_filmstrip = FilmStripController::Ptr(
        new FilmStripController(m_moduleshell->get_list_store(), *m_moduleshell));
    add(m_filmstrip);

    m_vbox.pack_start(*(m_filmstrip->buildWidget()), Gtk::PACK_SHRINK);

    // status bar
    m_vbox.pack_start(m_statusBar, Gtk::PACK_SHRINK);
    m_statusBar.push(Glib::ustring(_("Ready")));

    selection_controller->add_selectable(m_filmstrip);

    m_vbox.show_all_children();
    m_vbox.show();
}


Gtk::Widget *
NiepceWindow::buildWidget()
{
    if(m_widget) {
        return m_widget;
    }
    Gtk::Window & win(gtkWindow());

    m_widget = &win;

    init_actions();

    m_notifcenter.reset(new niepce::NotificationCenter());

    Glib::ustring name("camera-photo");
    set_icon_from_theme(name);

    m_notifcenter->signal_lib_notification.connect(
        sigc::mem_fun(*this, &NiepceWindow::on_lib_notification));

    win.set_size_request(600, 400);
    win.show_all_children();
    on_open_library();
    return &win;
}


void NiepceWindow::init_actions()
{
    m_menu = Gio::Menu::create();
    Glib::RefPtr<Gio::Menu> submenu;
    Glib::RefPtr<Gio::Menu> section;

    m_action_group = Gio::SimpleActionGroup::create();
    gtkWindow().insert_action_group("win", m_action_group);

    submenu = Gio::Menu::create();
    m_menu->append_submenu(_("Library"), submenu);

    // move to the workspace
    section = Gio::Menu::create();
    submenu->append_section(section);
    section->append(_("New Folder..."), "action");
    section->append(_("New Project..."), "action");
    fwk::add_action(m_action_group, "Import",
                    sigc::mem_fun(*this,
                                  &NiepceWindow::on_action_file_import),
                    section, _("_Import..."), "win");

    section = Gio::Menu::create();
    submenu->append_section(section);
    fwk::add_action(m_action_group, "Close",
                    sigc::mem_fun(gtkWindow(),
                                  &Gtk::Window::hide),
                    section, _("Close"), "win", "<Primary>w");

    submenu = Gio::Menu::create();
    m_menu->append_submenu(_("Edit"), submenu);

    section = Gio::Menu::create();
    submenu->append_section(section);

    create_undo_action(m_action_group, section);
    create_redo_action(m_action_group, section);

    // FIXME: bind
    section = Gio::Menu::create();
    submenu->append_section(section);

    fwk::add_action(m_action_group,
                    "Cut",
                    Gio::ActionMap::ActivateSlot(), section,
                    _("Cut"), "win", "<control>x");
    fwk::add_action(m_action_group,
                    "Copy",
                    Gio::ActionMap::ActivateSlot(), section,
                    _("Copy"), "win", "<control>c");
    fwk::add_action(m_action_group,
                    "Paste",
                    Gio::ActionMap::ActivateSlot(), section,
                    _("Paste"), "win" "<control>v");
    fwk::add_action(m_action_group,
                    "Delete",
                    sigc::mem_fun(*this, &NiepceWindow::on_action_edit_delete),
                    section, _("Delete"), "win", nullptr);

    submenu = Gio::Menu::create();
    m_menu->append_submenu(_("Tools"), submenu);
    fwk::add_action(m_action_group, "EditLabels",
                    sigc::mem_fun(*this, &NiepceWindow::on_action_edit_labels),
                    submenu, _("Edit Labels..."), "win", nullptr);

    m_hide_tools_action
        = fwk::add_action(m_action_group, "ToggleToolsVisible",
                          sigc::mem_fun(*this, &Frame::toggle_tools_visible),
                          submenu, _("Hide tools"), "win",
                          nullptr);
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
        // XXX change the API to provide more details.
        Glib::ustring source = import_dialog->source_path();
        if(source.empty()) {
            return;
        }
        cfg.setValue("last_import_location", source);

        auto importer = import_dialog->importer();
        DBG_ASSERT(!!importer, "Import can't be null if we clicked import");
        if (importer) {
            importer->do_import(
                source,
                [this] (const std::string & path, bool manage) {
                    m_libClient->importFromDirectory(path, manage);
                });
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

void NiepceWindow::on_open_library()
{
    Configuration & cfg = Application::app()->config();
    std::string libMoniker;
    int reopen = 0;
    try {
        reopen = std::stoi(cfg.getValue("reopen_last_library", "0"));
    }
    catch(...)
    {
    }
    if(reopen) {
        libMoniker = cfg.getValue("lastOpenLibrary", "");
    }
    if(libMoniker.empty()) {
        libMoniker = prompt_open_library();
    }
    else {
        DBG_OUT("last library is %s", libMoniker.c_str());
    }
    if(!libMoniker.empty()) {
        if(!open_library(libMoniker)) {
            ERR_OUT("library %s cannot be open. Prompting.",
                    libMoniker.c_str());
            libMoniker = prompt_open_library();
            open_library(libMoniker);
        }
    }
}

void NiepceWindow::create_initial_labels()
{
    // TODO make this parametric from resources
    m_libClient->createLabel(_("Label 1"), fwk::RgbColour(55769, 9509, 4369).to_string()); /* 217, 37, 17 */
    m_libClient->createLabel(_("Label 2"), fwk::RgbColour(24929, 55769, 4369).to_string()); /* 97, 217, 17 */
    m_libClient->createLabel(_("Label 3"), fwk::RgbColour(4369, 50629, 55769).to_string()); /* 17, 197, 217 */
    m_libClient->createLabel(_("Label 4"), fwk::RgbColour(35209, 4369, 55769).to_string()); /* 137, 17, 217 */
    m_libClient->createLabel(_("Label 5"), fwk::RgbColour(55769, 35209, 4369).to_string()); /* 217, 137, 17 */
}


void NiepceWindow::on_lib_notification(const eng::LibNotification & ln)
{
    switch(ln.type) {
    case eng::Library::NotifyType::NEW_LIBRARY_CREATED:
        create_initial_labels();
        break;
    case eng::Library::NotifyType::ADDED_LABELS:
    {
        eng::Label::ListPtr l 
            = boost::any_cast<eng::Label::ListPtr>(ln.param);
        m_libClient->getDataProvider()->addLabels(l);
        break;
    }
    case eng::Library::NotifyType::LABEL_CHANGED:
    {
        const eng::Label::Ptr & l 
            = boost::any_cast<const eng::Label::Ptr &>(ln.param);
        m_libClient->getDataProvider()->updateLabel(l);
        break;
    }
    case eng::Library::NotifyType::LABEL_DELETED:
    {
        int id = boost::any_cast<int>(ln.param);
        m_libClient->getDataProvider()->deleteLabel(id);
        break;
    }
    default:
        break;
    }
}

std::string NiepceWindow::prompt_open_library()
{
    std::string libMoniker;
    Gtk::FileChooserDialog dialog(gtkWindow(), _("Open library"),
                                  Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
    dialog.add_button(_("Cancel"), Gtk::RESPONSE_CANCEL);
    dialog.add_button(_("Open"), Gtk::RESPONSE_OK);

    int result = dialog.run();
    Glib::ustring libraryToCreate;
    switch(result)
    {
    case Gtk::RESPONSE_OK: {
        Configuration & cfg = Application::app()->config();
        libraryToCreate = dialog.get_filename();
        // pass it to the library
        libMoniker = "local:";
        libMoniker += libraryToCreate.c_str();
        cfg.setValue("lastOpenLibrary", libMoniker);
        DBG_OUT("created library %s", libMoniker.c_str());
        break;
    }
    default:
        break;
    }
    return libMoniker;
}

bool NiepceWindow::open_library(const std::string & libMoniker)
{
    fwk::Moniker mon = fwk::Moniker(libMoniker);
    m_libClient = LibraryClientPtr(new LibraryClient(mon,
                                                     m_notifcenter));
    if(!m_libClient->ok()) {
        m_libClient = nullptr;
        return false;
    }
    set_title(libMoniker);
    m_library_cfg
        = fwk::Configuration::Ptr(
            new fwk::Configuration(
                Glib::build_filename(mon.path(), "config.ini")));
    m_libClient->getAllLabels();
    if(!m_moduleshell) {
        _createModuleShell();
    }
    return true;
}

void NiepceWindow::on_action_edit_labels()
{
    DBG_OUT("edit labels");
    // get the labels.
    EditLabels::Ptr dlg(new EditLabels(getLibraryClient()));
    dlg->run_modal(shared_frame_ptr());
}

void NiepceWindow::on_action_edit_delete()
{
    // find the responder. And pass it.
    m_moduleshell->action_edit_delete();
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
