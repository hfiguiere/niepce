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


#include "fwk/base/debug.hpp"
#include "fwk/toolkit/widgets/ratinglabel.hpp"
#include "librarycellrenderer.hpp"

#include <gdkmm/general.h>

#ifndef DATADIR
#error DATADIR is not defined
#endif

#define CELL_PADDING 4

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
    property_mode() = Gtk::CELL_RENDERER_MODE_ACTIVATABLE;
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
    }
    catch(const std::exception & e) {
        ERR_OUT("exception while creating emblems: %s", e.what());
        ERR_OUT("a - check if all the needed pixmaps are present in the filesystem");
    }
    catch(...) {
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
        x = r.x + r.width - CELL_PADDING - w;
        y = r.y + r.height - CELL_PADDING - h;
        cr->set_source(emblem, x, y);
        cr->paint();
    }
}


}


void 
LibraryCellRenderer::get_size_vfunc(Gtk::Widget& /*widget*/, 
                                    const Gdk::Rectangle* /*cell_area*/, 
                                    int* x_offset, int* y_offset, 
                                    int* width, int* height) const
{
    if(x_offset) {
        *x_offset = 0;
    }
    if(y_offset) {
        *y_offset = 0;
    }
    
    if(width || height) {
        // TODO this should just be a property
        //
        Glib::RefPtr<Gdk::Pixbuf> pixbuf = property_pixbuf();
        int maxdim = m_size + pad() * 2;
	
        if(width) {
            *width = maxdim;
        }
        if(height) {
            *height = maxdim;
        }
    }
}


void 
LibraryCellRenderer::render_vfunc(const Glib::RefPtr<Gdk::Drawable>& window, 
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
        double x, y;
        x = r.x + CELL_PADDING;
        y = r.y + r.height - CELL_PADDING;
        fwk::RatingLabel::draw_rating(cr, file->rating(), 
                                      fwk::RatingLabel::get_star(), 
                                      fwk::RatingLabel::get_unstar(), x, y);
    }
    
    if(m_drawemblem) {
        Cairo::RefPtr<Cairo::ImageSurface> emblem;
        
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


bool LibraryCellRenderer::activate_vfunc(GdkEvent *event,
                                         Gtk::Widget & widget,
                                         const Glib::ustring &	path,
                                         const Gdk::Rectangle&	background_area,
                                         const Gdk::Rectangle&	cell_area,
                                         Gtk::CellRendererState	flags)
{
    DBG_OUT("activated. Event type of %d", event->type);
    if(event->type == GDK_BUTTON_PRESS) {
        GdkEventButton *bevt = (GdkEventButton*)event;

        // hit test with the rating region
        unsigned int xpad = Gtk::CellRenderer::property_xpad();
        unsigned int ypad = Gtk::CellRenderer::property_ypad();
        GdkRectangle r = *(cell_area.gobj());
        r.x += xpad;
        r.y += ypad;

        double x, y;
        double rw, rh;
        fwk::RatingLabel::get_geometry(rw, rh);
        GdkRectangle rect;
        rect.x = r.x + CELL_PADDING;
        rect.y = r.y + r.height - rh - CELL_PADDING;
        rect.width = rw;
        rect.height = rh;
        x = bevt->x;
        y = bevt->y;
        DBG_OUT("r(%d, %d, %d, %d) p(%f, %f)", rect.x, rect.y,
                rect.width, rect.height, x, y);
        bool hit = (rect.x <= x) && (rect.x + rect.width >= x) 
            && (rect.y <= y) && (rect.y + rect.height >= y);
        if(!hit) {
            DBG_OUT("not a hit");
            return false;
        }
        // hit test for the rating value
        int new_rating = fwk::RatingLabel::rating_value_from_hit_x(x - rect.x);
        DBG_OUT("new_rating %d", new_rating);
        eng::LibFile::Ptr file = m_libfileproperty.get_value();
        if(file->rating() != new_rating) {
            // emit if changed
            signal_rating_changed.emit(file->id(), new_rating);
        }
        return true;
    }
    return false;
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
