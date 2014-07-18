/*
 * niepce - niepce/ui/moduleshell.cpp
 *
 * Copyright (C) 2007-2014 Hubert Figuiere
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


#include <glibmm/i18n.h>
#include <glibmm/ustring.h>

#include <gtkmm/celllayout.h>
#include <gtkmm/cellrenderer.h>

#include "fwk/base/debug.hpp"
#include "niepce/notifications.hpp"
#include "engine/db/library.hpp"
#include "engine/db/libfile.hpp"
#include "fwk/toolkit/application.hpp"
#include "fwk/toolkit/gtkutils.hpp"
#include "moduleshell.hpp"
#include "niepcewindow.hpp"
#include "metadatapanecontroller.hpp"

namespace ui {

Gtk::Widget * ModuleShell::buildWidget(const Glib::RefPtr<Gtk::UIManager> & manager)
{
    if(m_widget) {
        return m_widget;
    }

    m_widget = &m_shell;
    m_shell.insert_action_group("shell", m_actionGroup);

    m_selection_controller = SelectionController::Ptr(new SelectionController);
    add(m_selection_controller);

    m_menu = Gio::Menu::create();

    // "go-previous"
    fwk::add_action(m_actionGroup, "PrevImage",
                    sigc::mem_fun(*m_selection_controller,
                                  &SelectionController::select_previous),
                    m_menu, _("Back"), "shell", "Left");

    // "go-next"
    fwk::add_action(m_actionGroup, "NextImage",
                    sigc::mem_fun(*m_selection_controller,
                                  &SelectionController::select_next),
                    m_menu, _("Forward"), "shell", "Right");

    auto section = Gio::Menu::create();
    m_menu->append_section(section);

    // "object-rotate-left"
    fwk::add_action(m_actionGroup, "RotateLeft",
                    sigc::bind(
                        sigc::mem_fun(*m_selection_controller,
                                      &SelectionController::rotate), -90),
                    section, _("Rotate Left"), "shell", "bracketleft");

    // "object-rotate-right"
    fwk::add_action(m_actionGroup, "RotateRight",
                    sigc::bind(
                        sigc::mem_fun(*m_selection_controller,
                                      &SelectionController::rotate), 90),
                    section, _("Rotate Right"), "shell", "bracketright");

    section = Gio::Menu::create();
    m_menu->append_section(section);
#if 0

    m_actionGroup->add(Gtk::Action::create("SetLabel", _("Set _Label")));
    m_actionGroup->add(Gtk::Action::create("SetLabel6", _("Label _6")),
                       Gtk::AccelKey('6', Gdk::CONTROL_MASK), sigc::bind(
                              sigc::mem_fun(*m_selection_controller,
                                            &SelectionController::set_label),
                              1));
    m_actionGroup->add(Gtk::Action::create("SetLabel7", _("Label _7")),
                       Gtk::AccelKey('7', Gdk::CONTROL_MASK), sigc::bind(
                              sigc::mem_fun(*m_selection_controller,
                                            &SelectionController::set_label),
                              2));
    m_actionGroup->add(Gtk::Action::create("SetLabel8", _("Label _8")),
                       Gtk::AccelKey('8', Gdk::CONTROL_MASK), sigc::bind(
                              sigc::mem_fun(*m_selection_controller,
                                            &SelectionController::set_label),
                              3));
    m_actionGroup->add(Gtk::Action::create("SetLabel9", _("Label _9")),
                       Gtk::AccelKey('9', Gdk::CONTROL_MASK), sigc::bind(
                              sigc::mem_fun(*m_selection_controller,
                                            &SelectionController::set_label),
                              4));

    m_actionGroup->add(Gtk::Action::create("SetRating", _("Set _Rating")));
    m_actionGroup->add(Gtk::Action::create("SetRating0", _("Unrated")),
                       Gtk::AccelKey('0', Gdk::CONTROL_MASK), sigc::bind(
                              sigc::mem_fun(*m_selection_controller,
                                            &SelectionController::set_rating),
                              0));
    m_actionGroup->add(Gtk::Action::create("SetRating1", _("Rating _1")),
                       Gtk::AccelKey('1', Gdk::CONTROL_MASK), sigc::bind(
                              sigc::mem_fun(*m_selection_controller,
                                            &SelectionController::set_rating),
                              1));
    m_actionGroup->add(Gtk::Action::create("SetRating2", _("Rating _2")),
                       Gtk::AccelKey('2', Gdk::CONTROL_MASK), sigc::bind(
                              sigc::mem_fun(*m_selection_controller,
                                            &SelectionController::set_rating),
                              2));
    m_actionGroup->add(Gtk::Action::create("SetRating3", _("Rating _3")),
                       Gtk::AccelKey('3', Gdk::CONTROL_MASK), sigc::bind(
                              sigc::mem_fun(*m_selection_controller,
                                            &SelectionController::set_rating),
                              3));
    m_actionGroup->add(Gtk::Action::create("SetRating4", _("Rating _4")),
                       Gtk::AccelKey('4', Gdk::CONTROL_MASK), sigc::bind(
                              sigc::mem_fun(*m_selection_controller,
                                            &SelectionController::set_rating),
                              4));
    m_actionGroup->add(Gtk::Action::create("SetRating5", _("Rating _5")),
                       Gtk::AccelKey('5', Gdk::CONTROL_MASK), sigc::bind(
                           sigc::mem_fun(*m_selection_controller,
                                         &SelectionController::set_rating),
                           5));

    m_actionGroup->add(Gtk::Action::create("SetFlag", _("Set _Flag")));
    m_actionGroup->add(Gtk::Action::create("SetFlagReject",
                                           _("Flag as _Rejected")),
                       Gtk::AccelKey('x', Gdk::CONTROL_MASK | Gdk::SHIFT_MASK),
                       sigc::bind(
                           sigc::mem_fun(*m_selection_controller,
                                         &SelectionController::set_flag),
                           -1));
    m_actionGroup->add(Gtk::Action::create("SetFlagNone", _("_Unflagged")),
                       Gtk::AccelKey('u', Gdk::CONTROL_MASK | Gdk::SHIFT_MASK),
                       sigc::bind(
                           sigc::mem_fun(*m_selection_controller,
                                         &SelectionController::set_flag),
                           0));
    m_actionGroup->add(Gtk::Action::create("SetFlagPick", _("Flag as _Pick")),
                       Gtk::AccelKey('p', Gdk::CONTROL_MASK | Gdk::SHIFT_MASK),
                       sigc::bind(
                           sigc::mem_fun(*m_selection_controller,
                                         &SelectionController::set_flag),
                           1));

    m_actionGroup->add(Gtk::Action::create("DeleteImage", _("Delete Image")));

    m_actionGroup->add(Gtk::Action::create("WriteMetadata",
                                           _("Write metadata")),
                       sigc::mem_fun(*m_selection_controller,
                                     &SelectionController::write_metadata));
#endif


    Glib::ustring ui_info =
        "<ui>"
        "  <menubar name='MenuBar'>"
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
        "      </menu>"
        "      <menu action='SetFlag'>"
        "        <menuitem action='SetFlagReject'/>"
        "        <menuitem action='SetFlagNone'/>"
        "        <menuitem action='SetFlagPick'/>"
        "      </menu>"
        "      <separator/>"
        "      <menuitem action='DeleteImage'/>"
        "      <separator/>"
        "      <menuitem action='WriteMetadata'/>"
        "    </menu>"
        "  </menubar>"
        "</ui>";
//    m_ui_merge_id = manager->add_ui_from_string(ui_info);
//    DBG_ASSERT(m_ui_merge_id, "merge failed");

    m_gridview = GridViewModule::Ptr(
        new GridViewModule(*this, m_selection_controller->get_list_store()));
    add_library_module(m_gridview, _("Library"));

    m_selection_controller->add_selectable(m_gridview.get());
    m_selection_controller->signal_selected
        .connect(sigc::mem_fun(*this, &ModuleShell::on_selected));
    m_selection_controller->signal_activated
        .connect(sigc::mem_fun(*this, &ModuleShell::on_image_activated));

    m_darkroom = dr::DarkroomModule::Ptr(new dr::DarkroomModule(*this));
    add_library_module(m_darkroom, _("Darkroom"));

    m_mapm = mapm::MapModule::Ptr(new mapm::MapModule(*this));
    add_library_module(m_mapm, _("Map"));

    m_shell.signal_activated.connect(sigc::mem_fun(*this, &ModuleShell::on_module_activated));
    m_shell.signal_deactivated.connect(sigc::mem_fun(*this, &ModuleShell::on_module_deactivated));

    // TODO PrintModuleController
    // add_library_module(, _("Print"));
    return m_widget;
}

void ModuleShell::action_edit_delete()
{
    DBG_OUT("shell - delete");
    m_selection_controller->move_to_trash();
}

void ModuleShell::add_library_module(const ILibraryModule::Ptr & module,
                                                   const std::string & label)
{
    auto w = module->buildWidget(Glib::RefPtr<Gtk::UIManager>());
    if(w) {
        add(module);
        m_shell.append_page(*w, label);
        m_modules.push_back(module);
    }
}

void ModuleShell::on_ready()
{
}


void ModuleShell::on_selected(eng::library_id_t id)
{
    DBG_OUT("selected callback %Ld", (long long)id);
    if(id > 0) {
        m_libraryclient->requestMetadata(id);
    }
    else  {
        m_gridview->display_none();
    }
}

void ModuleShell::on_image_activated(eng::library_id_t id)
{
    DBG_OUT("on image activated %Ld", (long long)id);
    auto store = m_selection_controller->get_list_store();
    auto iter = store->get_iter_from_id(id);
    if(iter) {
        auto libfile = (*iter)[store->columns().m_libfile];
        m_darkroom->set_image(libfile);
        m_shell.activate_page(1);
    }
}

void ModuleShell::on_module_deactivated(int idx)
{
    DBG_ASSERT((idx >= 0) && ((unsigned)idx < m_modules.size()), "wrong module index");
    m_modules[idx]->set_active(false);
}

void ModuleShell::on_module_activated(int idx)
{
    DBG_ASSERT((idx >= 0) && ((unsigned)idx < m_modules.size()), "wrong module index");
    m_shell.getMenuButton().set_menu_model(m_modules[idx]->getMenu());
    m_modules[idx]->set_active(true);
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
