/*
 * niepce - ui/librarycellrenderer.cpp
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


#include "utils/debug.h"
#include "librarycellrenderer.h"

#ifndef DATADIR
#error DATADIR is not defined
#endif

namespace ui {

LibraryCellRenderer::LibraryCellRenderer()
	: Glib::ObjectBase(typeid(LibraryCellRenderer)),
	  Gtk::CellRendererPixbuf(),
	  m_libfileproperty(*this, "libfile")
{
	try {
		m_raw_format_emblem 
			= Cairo::ImageSurface::create_from_png(
				std::string(DATADIR"/niepce/pixmaps/niepce-raw-fmt.png"));
		m_jpeg_format_emblem 
			= Cairo::ImageSurface::create_from_png(
				std::string(DATADIR"/niepce/pixmaps/niepce-jpg-fmt.png"));
	}
	catch(...)
	{
		ERR_OUT("exception");
	}
}

namespace {

	void drawFormatEmblem(const Cairo::RefPtr<Cairo::Context> & cr, 
						  const Cairo::RefPtr<Cairo::ImageSurface> & emblem,
						  const GdkRectangle & r)
		
	{	
		if(emblem) {
			int w, h;
			w = emblem->get_width();
			h = emblem->get_height();
			double x, y;
			x = r.x + r.width - 4 - w;
			y = r.y + r.height - 4 - h;
			cr->set_source(emblem, x, y);
			cr->paint();
		}
	}

}


void LibraryCellRenderer::get_size_vfunc (Gtk::Widget& widget, 
										const Gdk::Rectangle* cell_area, 
										int* x_offset, int* y_offset, 
										int* width, int* height) const
{
	Gtk::CellRendererPixbuf::get_size_vfunc(widget, cell_area, x_offset, y_offset,
											width, height);
	// make sure it is square.
	if(width && height) {
		int maxdim = std::max(*width, *height);
		*width = maxdim;
		*height = maxdim;
	}
}


void LibraryCellRenderer::render_vfunc (const Glib::RefPtr<Gdk::Drawable>& window, 
									  Gtk::Widget& widget, 
									  const Gdk::Rectangle& background_area, 
									  const Gdk::Rectangle& cell_area, 
									  const Gdk::Rectangle& expose_area, 
									  Gtk::CellRendererState flags)
{
	unsigned int xpad = Gtk::CellRenderer::property_xpad();
	unsigned int ypad = Gtk::CellRenderer::property_ypad();
	
	Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();
	GdkRectangle r = *(cell_area.gobj());
	r.x += xpad;
	r.y += ypad;

	db::LibFile::Ptr file = m_libfileproperty.get_value();

	Gdk::Color color;
	if(flags & Gtk::CELL_RENDERER_SELECTED) {
		color = widget.get_style()->get_bg(Gtk::STATE_SELECTED);
	}
	else {
		color = widget.get_style()->get_bg(Gtk::STATE_NORMAL);
	}
		
	cr->set_source_rgb(color.get_red_p(), color.get_green_p(), 
					   color.get_blue_p());
	cr->rectangle(r.x, r.y, r.width, r.height);
	cr->fill();
	
	Gtk::CellRendererPixbuf::render_vfunc(window, widget, background_area,
										  cell_area, expose_area, flags);

	Cairo::RefPtr<Cairo::ImageSurface> emblem = m_raw_format_emblem;
	drawFormatEmblem(cr, emblem, r);
}



Glib::PropertyProxy_ReadOnly<db::LibFile::Ptr> 	
LibraryCellRenderer::property_libfile() const
{
	return Glib::PropertyProxy_ReadOnly<db::LibFile::Ptr>(this, "libfile");
}


Glib::PropertyProxy<db::LibFile::Ptr> 	
LibraryCellRenderer::property_libfile()
{
	return Glib::PropertyProxy<db::LibFile::Ptr>(this, "libfile");
}


}


