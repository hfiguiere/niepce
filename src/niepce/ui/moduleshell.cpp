/*
 * niepce - niepce/ui/moduleshell.cpp
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


#include <glibmm/i18n.h>
#include <glibmm/ustring.h>

#include <gtkmm/celllayout.h>
#include <gtkmm/cellrenderer.h>
#include <gtkmm/stock.h>

#include "fwk/base/debug.hpp"
#include "niepce/notifications.hpp"
#include "niepce/stock.hpp"
#include "engine/db/library.hpp"
#include "engine/db/libfile.hpp"
#include "fwk/toolkit/application.hpp"
#include "fwk/toolkit/widgets/ratingaction.hpp"
#include "moduleshell.hpp"
#include "niepcewindow.hpp"
#include "metadatapanecontroller.hpp"

namespace ui {




Gtk::Widget * ModuleShell::buildWidget(const Glib::RefPtr<Gtk::UIManager> & manager)
{
    if(m_widget) {
        return m_widget;
    }
    DBG_ASSERT(manager, "manager is NULL");

    m_widget = &m_shell;
    m_ui_manager = manager;

    m_selection_controller = SelectionController::Ptr(new SelectionController);
    add(m_selection_controller);

    Glib::RefPtr<Gtk::Action> an_action;

    m_actionGroup->add(Gtk::Action::create("MenuImage", _("_Image")));

    m_actionGroup->add(Gtk::Action::create("PrevImage", Gtk::Stock::GO_BACK),
                          Gtk::AccelKey(GDK_KEY_Left, Gdk::ModifierType(0)),
                          sigc::mem_fun(*m_selection_controller,
                                        &SelectionController::select_previous));
    m_actionGroup->add(Gtk::Action::create("NextImage", Gtk::Stock::GO_FORWARD),
                          Gtk::AccelKey(GDK_KEY_Right, Gdk::ModifierType(0)),
                          sigc::mem_fun(*m_selection_controller,
                                        &SelectionController::select_next));
    
    an_action = Gtk::Action::create("RotateLeft", niepce::Stock::ROTATE_LEFT);
    m_actionGroup->add(an_action, sigc::bind(
                          sigc::mem_fun(*m_selection_controller,
                                        &SelectionController::rotate), -90));
    an_action = Gtk::Action::create("RotateRight", niepce::Stock::ROTATE_RIGHT);
    m_actionGroup->add(an_action, sigc::bind(
                          sigc::mem_fun(*m_selection_controller,
                                        &SelectionController::rotate), 90));
    
    m_actionGroup->add(Gtk::Action::create("SetLabel", _("Set _Label")));
    m_actionGroup->add(Gtk::Action::create("SetLabel6", _("Label _6")),
                          Gtk::AccelKey("6"), sigc::bind(
                              sigc::mem_fun(*m_selection_controller, 
                                            &SelectionController::set_label),
                              1));
    m_actionGroup->add(Gtk::Action::create("SetLabel7", _("Label _7")),
                          Gtk::AccelKey("7"), sigc::bind(
                              sigc::mem_fun(*m_selection_controller, 
                                            &SelectionController::set_label),
                              2));
    m_actionGroup->add(Gtk::Action::create("SetLabel8", _("Label _8")),
                          Gtk::AccelKey("8"), sigc::bind(
                              sigc::mem_fun(*m_selection_controller, 
                                            &SelectionController::set_label),
                              3));
    m_actionGroup->add(Gtk::Action::create("SetLabel9", _("Label _9")),
                          Gtk::AccelKey("9"), sigc::bind(
                              sigc::mem_fun(*m_selection_controller, 
                                            &SelectionController::set_label),
                              4));
    
    m_actionGroup->add(Gtk::Action::create("SetRating", _("Set _Rating")));
    m_actionGroup->add(fwk::RatingAction::create("SetRating0", 0),
                          Gtk::AccelKey("0"), sigc::bind(
                              sigc::mem_fun(*m_selection_controller,
                                            &SelectionController::set_rating),
                              0));
    m_actionGroup->add(fwk::RatingAction::create("SetRating1", 1),
                          Gtk::AccelKey("1"), sigc::bind(
                              sigc::mem_fun(*m_selection_controller,
                                            &SelectionController::set_rating),
                              1));
    m_actionGroup->add(fwk::RatingAction::create("SetRating2", 2),
                          Gtk::AccelKey("2"), sigc::bind(
                              sigc::mem_fun(*m_selection_controller,
                                            &SelectionController::set_rating),
                              2));
    m_actionGroup->add(fwk::RatingAction::create("SetRating3", 3),
                          Gtk::AccelKey("3"), sigc::bind(
                              sigc::mem_fun(*m_selection_controller,
                                            &SelectionController::set_rating),
                              3));
    m_actionGroup->add(fwk::RatingAction::create("SetRating4", 4),
                          Gtk::AccelKey("4"), sigc::bind(
                              sigc::mem_fun(*m_selection_controller,
                                            &SelectionController::set_rating),
                              4));
    m_actionGroup->add(fwk::RatingAction::create("SetRating5", 5),
                       Gtk::AccelKey("5"), sigc::bind(
                           sigc::mem_fun(*m_selection_controller,
                                         &SelectionController::set_rating),
                           5));

    m_actionGroup->add(Gtk::Action::create("SetFlag", _("Set _Flag")));
    m_actionGroup->add(Gtk::Action::create("SetFlagReject", _("Flag as _Rejected")),
                          Gtk::AccelKey("x"), sigc::bind(
                              sigc::mem_fun(*m_selection_controller,
                                            &SelectionController::set_flag),
                              -1));
    m_actionGroup->add(Gtk::Action::create("SetFlagNone", _("_Unflagged")),
                          Gtk::AccelKey("u"), sigc::bind(
                              sigc::mem_fun(*m_selection_controller,
                                            &SelectionController::set_flag),
                              0));
    m_actionGroup->add(Gtk::Action::create("SetFlagPick", _("Flag as _Pick")),
                          Gtk::AccelKey("p"), sigc::bind(
                              sigc::mem_fun(*m_selection_controller,
                                            &SelectionController::set_flag),
                              1));

    m_actionGroup->add(Gtk::Action::create("DeleteImage", Gtk::Stock::DELETE));

    manager->insert_action_group(m_actionGroup);


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
        "    </menu>"
        "  </menubar>"
        "</ui>";
    m_ui_merge_id = manager->add_ui_from_string(ui_info);
    DBG_ASSERT(m_ui_merge_id, "merge failed");

    m_gridview = GridViewModule::Ptr(
        new GridViewModule(*this, m_selection_controller->get_list_store()));
    add_library_module(m_gridview, _("Library"));

    m_selection_controller->add_selectable(m_gridview.get());
    m_selection_controller->signal_selected
        .connect(sigc::mem_fun(*this, &ModuleShell::on_selected));
    m_selection_controller->signal_activated
        .connect(sigc::mem_fun(*this, &ModuleShell::on_image_activated));

    m_darkroom = dr::DarkroomModule::Ptr(new dr::DarkroomModule(*this, m_actionGroup));
    add_library_module(m_darkroom, _("Darkroom"));

    m_shell.signal_activated.connect(sigc::mem_fun(*this, &ModuleShell::on_module_activated));

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
    Gtk::Widget * w = module->buildWidget(m_ui_manager);
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
    DBG_OUT("selected callback %Ld", id);
    if(id > 0) {
        m_libraryclient->requestMetadata(id);
    }		
    else  {
        m_gridview->display_none();
    }
}

void ModuleShell::on_image_activated(eng::library_id_t id)
{
    DBG_OUT("on image activated %Ld", id);
    Glib::RefPtr<ImageListStore> store = m_selection_controller->get_list_store();
    Gtk::TreeIter iter = store->get_iter_from_id(id);
    if(iter) {
        eng::LibFile::Ptr libfile = (*iter)[store->columns().m_libfile];
        m_darkroom->set_image(libfile);
        m_shell.activate_page(1);
    }
}

void ModuleShell::on_module_activated(int idx)
{
    DBG_ASSERT(idx < m_modules.size(), "wrong module index");
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
