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


#include <gdkmm/general.h>

#include "utils/debug.h"
#include "librarycellrenderer.h"

#ifndef DATADIR
#error DATADIR is not defined
#endif

namespace ui {

#define PAD 16

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
		m_star = Cairo::ImageSurface::create_from_png(
			std::string(DATADIR"/niepce/pixmaps/niepce-set-star.png"));
		m_unstar = Cairo::ImageSurface::create_from_png(
			std::string(DATADIR"/niepce/pixmaps/niepce-unset-star.png"));
	}
	catch(...)
	{
		ERR_OUT("exception");
	}
}

namespace {

	void drawThumbnail(const Cairo::RefPtr<Cairo::Context> & cr, 
					   Glib::RefPtr<Gdk::Pixbuf> & pixbuf,
					   const GdkRectangle & r)
	{
		double x, y;
		x = r.x + PAD;
		y = r.y + PAD;
		int w = pixbuf->get_width();
		int h = pixbuf->get_height();
		int min = std::min(w,h);
		int max = std::max(w,h);
		int offset = (max - min) / 2;
		if(w > h) {
			y += offset;
		}
		else {
			x += offset;
		}
	
// draw the shadow...
//		cr->set_source_rgb(0.0, 0.0, 0.0);
//		cr->rectangle(x + 3, y + 3, w, h);
//		cr->fill();

// draw the white border
		cr->set_source_rgb(1.0, 1.0, 1.0);
  		cr->rectangle(x, y, w, h);
		cr->stroke();
		
		Gdk::Cairo::set_source_pixbuf(cr, pixbuf, x, y);
		cr->paint();
	}

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

	void drawRating(const Cairo::RefPtr<Cairo::Context> & cr, 
					int32_t rating,
					const Cairo::RefPtr<Cairo::ImageSurface> & star,
					const Cairo::RefPtr<Cairo::ImageSurface> & unstar,
					const GdkRectangle & r)
	{
		if(rating == -1 || !star || !unstar) {
			return;
		}
		int w = star->get_width();
		int h = star->get_height();
		double x, y;
		x = r.x + 4;
		y = r.y + r.height - 4 - h;
		for(int32_t i = 1; i <= 5; i++) {
			if(i <= rating) {
				cr->set_source(star, x, y);
			}
			else {
				cr->set_source(unstar, x, y);
			}
			cr->paint();
			x += w;
		}
	}

}


void 
LibraryCellRenderer::get_size_vfunc (Gtk::Widget& /*widget*/, 
									 const Gdk::Rectangle* /*cell_area*/, 
									 int* x_offset, int* y_offset, 
									 int* width, int* height) const
{
	if(x_offset)
		*x_offset = 0;
	if(y_offset)
		*y_offset = 0;

	if(width || height) {
		int w, h;
		// TODO this should just be a property
		//
		Glib::RefPtr<Gdk::Pixbuf> pixbuf = property_pixbuf();
		w = pixbuf->get_width();
		h = pixbuf->get_height();
		int maxdim = std::max(w, h) + PAD * 2;
		
		if(width) 
			*width = maxdim;
		if(height) 
			*height = maxdim;
	}
}


void 
LibraryCellRenderer::render_vfunc (const Glib::RefPtr<Gdk::Drawable>& window, 
								   Gtk::Widget& widget, 
								   const Gdk::Rectangle& /*background_area*/, 
								   const Gdk::Rectangle& cell_area, 
								   const Gdk::Rectangle& /*expose_area*/, 
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

	Gdk::Cairo::set_source_color(cr, color);
	cr->rectangle(r.x, r.y, r.width, r.height);
	cr->fill();

	color = widget.get_style()->get_dark(Gtk::STATE_SELECTED);
	Gdk::Cairo::set_source_color(cr, color);
	cr->set_line_width(1.0);
	cr->rectangle(r.x, r.y, r.width, r.height);
	cr->stroke();

	Glib::RefPtr<Gdk::Pixbuf> pixbuf = property_pixbuf();
	drawThumbnail(cr, pixbuf, r);

	Cairo::RefPtr<Cairo::ImageSurface> emblem = m_raw_format_emblem;
	drawRating(cr, file->rating(), m_star, m_unstar, r);
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


