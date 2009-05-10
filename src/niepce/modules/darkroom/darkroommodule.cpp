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
#include "ncr/ncr.hpp"
#include "darkroommodule.hpp"

namespace darkroom {


void DarkroomModule::set_image(const eng::LibFile::Ptr & file)
{
    m_image->reload(file->path(), 
                    file->fileType() == eng::LibFile::FILE_TYPE_RAW,
                    file->orientation());
/*    int w, h;
    w = m_imagecanvas->get_width();
    h = m_imagecanvas->get_height();
    m_image->set_scale_to_dim(w, h);
*/
    m_imagecanvas->set_image(m_image->pixbuf_for_display());
}


Gtk::Widget * DarkroomModule::buildWidget()
{
    ncr::init();
    m_imagecanvas = Gtk::manage(new ImageCanvas());
// TODO set a proper canvas size
//    m_canvas_scroll.add(*m_imagecanvas);
	m_vbox.pack_start(*m_imagecanvas, Gtk::PACK_EXPAND_WIDGET);

//    int w,h;
//    w = h = 0;
//    m_canvas_scroll.get_size_request(w, h);
//    DBG_OUT("scroll size %d %d", w, h);
//    m_imagecanvas->set_size_request(w, h);
    m_imagecanvas->set_bounds(0, 0, 1000, 1000);

	// build the toolbar.
	Gtk::Toolbar * toolbar = Gtk::manage(new Gtk::Toolbar);

	Glib::RefPtr<Gtk::Action> an_action;
	an_action = m_actionGroup->get_action("PrevImage");
	toolbar->append(*(an_action->create_tool_item()));
	an_action = m_actionGroup->get_action("NextImage");
	toolbar->append(*(an_action->create_tool_item()));
	an_action = m_actionGroup->get_action("RotateLeft");
	toolbar->append(*(an_action->create_tool_item()));
	an_action = m_actionGroup->get_action("RotateRight");
	toolbar->append(*(an_action->create_tool_item()));

	m_vbox.pack_start(*toolbar, Gtk::PACK_SHRINK);
	m_dr_splitview.pack1(m_vbox, Gtk::EXPAND);
    m_dock = new fwk::Dock();
    m_dr_splitview.pack2(m_dock->getWidget(), Gtk::SHRINK);

    m_databinders.add_binder(new fwk::ConfigDataBinder<int>(
                                 m_dr_splitview.property_position(),
                                 fwk::Application::app()->config(),
                                 "dr_toolbox_pane_splitter"));

    m_toolbox_ctrl = ToolboxController::Ptr(new ToolboxController(*m_dock));
    add(m_toolbox_ctrl);
    (void)m_toolbox_ctrl->buildWidget();

	m_widget = &m_dr_splitview;
	return m_widget;
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
