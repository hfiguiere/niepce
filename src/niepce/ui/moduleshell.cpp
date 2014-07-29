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

Gtk::Widget * ModuleShell::buildWidget()
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

    auto submenu = Gio::Menu::create();
    section->append_submenu(_("Set Label"), submenu);

    fwk::add_action(m_actionGroup,
                    "SetLabel6",
                    sigc::bind(sigc::mem_fun(*m_selection_controller,
                                             &SelectionController::set_label),
                               1),
                    submenu, _("Label 6"), "shell", "<Primary>6");
    fwk::add_action(m_actionGroup,
                    "SetLabel7",
                    sigc::bind(sigc::mem_fun(*m_selection_controller,
                                             &SelectionController::set_label),
                               2),
                    submenu, _("Label 7"), "shell", "<Primary>7");
    fwk::add_action(m_actionGroup,
                    "SetLabel8",
                    sigc::bind(sigc::mem_fun(*m_selection_controller,
                                             &SelectionController::set_label),
                               3),
                    submenu, _("Label 8"), "shell", "<Primary>8");
    fwk::add_action(m_actionGroup,
                    "SetLabel9",
                    sigc::bind(sigc::mem_fun(*m_selection_controller,
                                             &SelectionController::set_label),
                               4),
                    submenu, _("Label 9"), "shell", "<Primary>9");


    submenu = Gio::Menu::create();
    section->append_submenu(_("Set Rating"), submenu);

    fwk::add_action(m_actionGroup,
                    "SetRating0",
                    sigc::bind(sigc::mem_fun(*m_selection_controller,
                                             &SelectionController::set_rating),
                               0),
                    submenu, _("Unrated"), "shell", "<Primary>0");
    fwk::add_action(m_actionGroup,
                    "SetRating1",
                    sigc::bind(sigc::mem_fun(*m_selection_controller,
                                             &SelectionController::set_rating),
                               1),
                    submenu, _("Rating 1"), "shell", "<Primary>1");
    fwk::add_action(m_actionGroup,
                    "SetRating2",
                    sigc::bind(sigc::mem_fun(*m_selection_controller,
                                             &SelectionController::set_rating),
                               2),
                    submenu, _("Rating 2"), "shell", "<Primary>2");
    fwk::add_action(m_actionGroup,
                    "SetRating3",
                    sigc::bind(sigc::mem_fun(*m_selection_controller,
                                             &SelectionController::set_rating),
                               3),
                    submenu, _("Rating 3"), "shell", "<Primary>3");
    fwk::add_action(m_actionGroup,
                    "SetRating4",
                    sigc::bind(sigc::mem_fun(*m_selection_controller,
                                             &SelectionController::set_rating),
                               4),
                    submenu, _("Rating 4"), "shell", "<Primary>4");
    fwk::add_action(m_actionGroup,
                    "SetRating5",
                    sigc::bind(sigc::mem_fun(*m_selection_controller,
                                             &SelectionController::set_rating),
                               5),
                    submenu, _("Rating 5"), "shell", "<Primary>5");

    submenu = Gio::Menu::create();
    section->append_submenu(_("Set Flag"), submenu);

    fwk::add_action(m_actionGroup,
                    "SetFlagReject",
                    sigc::bind(
                           sigc::mem_fun(*m_selection_controller,
                                         &SelectionController::set_flag),
                           -1),
                    submenu, _("Flag as Rejected"), "shell", "<Primary><Shift>x");
    fwk::add_action(m_actionGroup,
                    "SetFlagNone",
                    sigc::bind(
                           sigc::mem_fun(*m_selection_controller,
                                         &SelectionController::set_flag),
                           0),
                    submenu, _("Unflagged"), "shell", "<Primary><Shift>u");
    fwk::add_action(m_actionGroup,
                    "SetFlagPick",
                    sigc::bind(
                           sigc::mem_fun(*m_selection_controller,
                                         &SelectionController::set_flag),
                           1),
                    submenu, _("Flag as Pick"), "shell", "<Primary><Shift>p");

    section = Gio::Menu::create();
    m_menu->append_section(section);

    fwk::add_action(m_actionGroup,
                    "WriteMetadata",
                    sigc::mem_fun(*m_selection_controller,
                                  &SelectionController::write_metadata),
                    submenu, _("Write metadata"), "shell", nullptr);

    // Module menu placeholder
    m_module_menu = Gio::Menu::create();
    m_menu->append_section(m_module_menu);

    m_shell.getMenuButton().set_menu_model(m_menu);

    m_gridview = GridViewModule::Ptr(
        new GridViewModule(*this, m_selection_controller->get_list_store()));
    add_library_module(m_gridview, _("Library"));

    m_selection_controller->add_selectable(m_gridview);
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
    auto w = module->buildWidget();
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
    m_module_menu->remove_all();
    m_modules[idx]->set_active(false);
}

void ModuleShell::on_module_activated(int idx)
{
    DBG_ASSERT((idx >= 0) && ((unsigned)idx < m_modules.size()), "wrong module index");
    auto menu = m_modules[idx]->getMenu();
    if (menu) {
        m_module_menu->append_section(menu);
    }
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
