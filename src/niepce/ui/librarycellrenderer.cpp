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

#include "fwk/base/debug.hpp"
#include "librarycellrenderer.hpp"

#ifndef DATADIR
#error DATADIR is not defined
#endif

namespace ui {

LibraryCellRenderer::LibraryCellRenderer()
	: Glib::ObjectBase(typeid(LibraryCellRenderer)),
	  Gtk::CellRendererPixbuf(),
    m_size(160),
    m_pad(16),
    m_drawborder(true),
    m_drawemblem(true),
    m_drawrating(true),
	  m_libfileproperty(*this, "libfile")
{
	try {
        m_raw_format_emblem 
            = Cairo::ImageSurface::create_from_png(
                std::string(DATADIR"/niepce/pixmaps/niepce-raw-fmt.png"));
        m_rawjpeg_format_emblem 
            = Cairo::ImageSurface::create_from_png(
                std::string(DATADIR"/niepce/pixmaps/niepce-rawjpeg-fmt.png"));
        m_img_format_emblem 
            = Cairo::ImageSurface::create_from_png(
                std::string(DATADIR"/niepce/pixmaps/niepce-img-fmt.png"));     
        m_video_format_emblem 
            = Cairo::ImageSurface::create_from_png(
                std::string(DATADIR"/niepce/pixmaps/niepce-video-fmt.png"));
        m_unknown_format_emblem 
            = Cairo::ImageSurface::create_from_png(
                std::string(DATADIR"/niepce/pixmaps/niepce-unknown-fmt.png"));
        
        m_star = Cairo::ImageSurface::create_from_png(
            std::string(DATADIR"/niepce/pixmaps/niepce-set-star.png"));
        m_unstar = Cairo::ImageSurface::create_from_png(
            std::string(DATADIR"/niepce/pixmaps/niepce-unset-star.png"));
	}
	catch(const std::exception & e)
	{
            ERR_OUT("exception while creating emblems: %s", e.what());
            ERR_OUT("a - check if all the needed pixmaps are present in the filesystem");
	}
    catch(...)
    {
        ERR_OUT("uncatched exception");
    }
        
}


void LibraryCellRenderer::_drawThumbnail(const Cairo::RefPtr<Cairo::Context> & cr, 
                                         Glib::RefPtr<Gdk::Pixbuf> & pixbuf,
                                         const GdkRectangle & r)
{
    double x, y;
    x = r.x + pad();
    y = r.y + pad();
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

void drawRating(const Cairo::RefPtr<Cairo::Context> & cr, 
                int32_t rating,
                const Cairo::RefPtr<Cairo::ImageSurface> & star,
                const Cairo::RefPtr<Cairo::ImageSurface> & unstar,
                const GdkRectangle & r)
{
    if(!star || !unstar) {
        return;
    }
    if(rating == -1) {
        rating = 0;
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
		// TODO this should just be a property
		//
		Glib::RefPtr<Gdk::Pixbuf> pixbuf = property_pixbuf();
		int maxdim = m_size + pad() * 2;
		
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

	eng::LibFile::Ptr file = m_libfileproperty.get_value();

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

  if(m_drawborder) {
      color = widget.get_style()->get_dark(Gtk::STATE_SELECTED);
      Gdk::Cairo::set_source_color(cr, color);
      cr->set_line_width(1.0);
      cr->rectangle(r.x, r.y, r.width, r.height);
      cr->stroke();
  }

	Glib::RefPtr<Gdk::Pixbuf> pixbuf = property_pixbuf();
	_drawThumbnail(cr, pixbuf, r);
  if(m_drawrating) {
      drawRating(cr, file->rating(), m_star, m_unstar, r);
  }

  if(m_drawemblem) {
    Cairo::RefPtr<Cairo::ImageSurface> emblem;
    
//    DBG_OUT("the filetype: %i", file->fileType());
    
    switch(file->fileType()) {
    case eng::LibFile::FILE_TYPE_RAW:
        emblem = m_raw_format_emblem;
        break;
    case eng::LibFile::FILE_TYPE_RAW_JPEG:
        emblem = m_rawjpeg_format_emblem;
        break;
    case eng::LibFile::FILE_TYPE_IMAGE:
        emblem = m_img_format_emblem;
        break;
    case eng::LibFile::FILE_TYPE_VIDEO:
        emblem = m_video_format_emblem;
        break;
    default:
        emblem = m_unknown_format_emblem;
        break;
    }

    drawFormatEmblem(cr, emblem, r);
  }
}



Glib::PropertyProxy_ReadOnly<eng::LibFile::Ptr> 	
LibraryCellRenderer::property_libfile() const
{
	return Glib::PropertyProxy_ReadOnly<eng::LibFile::Ptr>(this, "libfile");
}


Glib::PropertyProxy<eng::LibFile::Ptr> 	
LibraryCellRenderer::property_libfile()
{
	return Glib::PropertyProxy<eng::LibFile::Ptr>(this, "libfile");
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
