/*
 * niepce - ui/darkroommodule.cpp
 *
 * Copyright (C) 2008 Hubert Figuiere
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

#include <gdkmm/pixbuf.h>
#include <gtkmm/toolbar.h>
#include <gtkmm/stock.h>

#include "fwk/base/debug.hpp"
#include "fwk/toolkit/application.hpp"
#include "fwk/toolkit/configdatabinder.hpp"
#include "fwk/toolkit/widgets/dock.hpp"
#include "ncr/init.hpp"
#include "darkroommodule.hpp"

namespace dr {

DarkroomModule::DarkroomModule(const ui::IModuleShell & shell, 
                               const Glib::RefPtr<Gtk::ActionGroup> & action_group)
    : m_shell(shell)
    , m_actionGroup(action_group)
    , m_image(new ncr::Image)
{
    m_shell.get_selection_controller()->signal_selected.connect(
        sigc::mem_fun(*this, &DarkroomModule::on_selected));
}


void DarkroomModule::set_image(const eng::LibFile::Ptr & file)
{
    if(file) {
        m_image->reload(file->path(), 
                        file->fileType() == eng::LibFile::FILE_TYPE_RAW,
                        file->orientation());
    }
    else {
        // clear out
    }
}

void DarkroomModule::dispatch_action(const std::string & /*action_name*/)
{
}


void DarkroomModule::set_active(bool active)
{
    m_active = active;
    if(active) {
        // if activated, force the refresh of the image.
        ui::SelectionController::Ptr sel = m_shell.get_selection_controller();
        eng::library_id_t id = sel->get_selection();
        eng::LibFile::Ptr file = sel->get_file(id);
        set_image(file);
    }
}


Gtk::Widget * DarkroomModule::buildWidget(const Glib::RefPtr<Gtk::UIManager> & manager)
{
    if(m_widget) {
        return m_widget;
    }
    ncr::init();
    m_widget = &m_dr_splitview;
    m_imagecanvas = Gtk::manage(new ImageCanvas());
// TODO set a proper canvas size
//    m_canvas_scroll.add(*m_imagecanvas);
    m_vbox.pack_start(*m_imagecanvas, Gtk::PACK_EXPAND_WIDGET);

    m_imagecanvas->set_image(m_image);

    // build the toolbar.
    Gtk::Toolbar * toolbar = Gtk::manage(new Gtk::Toolbar);

    Glib::RefPtr<Gtk::Action> an_action;
    Gtk::ToolItem * tool_item;
    an_action = m_actionGroup->get_action("PrevImage");
    tool_item = an_action->create_tool_item();
    toolbar->append(*manage(tool_item));

    an_action = m_actionGroup->get_action("NextImage");
    tool_item = an_action->create_tool_item();
    toolbar->append(*manage(tool_item));

    an_action = m_actionGroup->get_action("RotateLeft");
    tool_item = an_action->create_tool_item();
    toolbar->append(*manage(tool_item));

    an_action = m_actionGroup->get_action("RotateRight");
    tool_item = an_action->create_tool_item();
    toolbar->append(*manage(tool_item));

    m_vbox.pack_start(*toolbar, Gtk::PACK_SHRINK);
    m_dr_splitview.pack1(m_vbox, Gtk::EXPAND);
    m_dock = Gtk::manage(new fwk::Dock());
    m_dr_splitview.pack2(*m_dock, Gtk::SHRINK);

    m_databinders.add_binder(new fwk::ConfigDataBinder<int>(
                                 m_dr_splitview.property_position(),
                                 fwk::Application::app()->config(),
                                 "dr_toolbox_pane_splitter"));

    m_toolbox_ctrl = ToolboxController::Ptr(new ToolboxController);
    add(m_toolbox_ctrl);
    m_dock->vbox().pack_start(*m_toolbox_ctrl->buildWidget(manager));

    return m_widget;
}

void DarkroomModule::on_selected(eng::library_id_t id)
{
    eng::LibFile::Ptr file = m_shell.get_selection_controller()->get_file(id);
    set_image(file);
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
