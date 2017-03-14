/*
 * niepce - ui/librarycellrenderer.cpp
 *
 * Copyright (C) 2008-2013 Hubert Figuiere
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


#include <stdint.h>

#include "fwk/base/debug.hpp"
#include "fwk/toolkit/widgets/ratinglabel.hpp"
#include "fwk/toolkit/gdkutils.hpp"
#include "engine/db/label.hpp"
#include "libraryclient/uidataprovider.hpp"
#include "librarycellrenderer.hpp"

#include <gdkmm/general.h>

#ifndef DATADIR
#error DATADIR is not defined
#endif

#define CELL_PADDING 4

namespace ui {

LibraryCellRenderer::LibraryCellRenderer(libraryclient::UIDataProvider *provider)
    : Glib::ObjectBase(typeid(LibraryCellRenderer)),
      Gtk::CellRendererPixbuf(),
      m_size(160),
      m_pad(16),
      m_drawborder(true),
      m_drawemblem(true),
      m_drawrating(true),
      m_drawlabel(true),
      m_drawflag(true),
      m_uiDataProvider(provider),
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

        m_flag_reject
            = Cairo::ImageSurface::create_from_png(
                std::string(DATADIR"/niepce/pixmaps/niepce-flag-reject.png"));
        m_flag_pick
            = Cairo::ImageSurface::create_from_png(
                std::string(DATADIR"/niepce/pixmaps/niepce-flag-pick.png"));
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
    if(!pixbuf) {
        ERR_OUT("NULL pixbuf");
        return;
    }
    int w = pixbuf->get_width();
    int h = pixbuf->get_height();
    int offset_x = (m_size - w) / 2;
    int offset_y = (m_size - h) / 2;
    double x, y;
    x = r.x + pad() + offset_x;
    y = r.y + pad() + offset_y;
	
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


void LibraryCellRenderer::_drawFlag(const Cairo::RefPtr<Cairo::Context> & cr, 
                   int flag_value, double x, double y)
{
    if(flag_value == 0) {
        return;
    }

    Cairo::RefPtr<Cairo::ImageSurface> pixbuf;
    if(flag_value == -1) {
        pixbuf = m_flag_reject;
    }
    else if(flag_value == 1) {
        pixbuf = m_flag_pick;
    }
    else {
        ERR_OUT("wrong flag value %d", flag_value);
        return ;
    }
    int w = pixbuf->get_width();

    cr->set_source(pixbuf, x - w, y);
    cr->paint();
}



namespace {

int drawFormatEmblem(const Cairo::RefPtr<Cairo::Context> & cr, 
                      const Cairo::RefPtr<Cairo::ImageSurface> & emblem,
                      const GdkRectangle & r)		
{	
    int left = 0;
    if(emblem) {
        int w, h;
        w = emblem->get_width();
        h = emblem->get_height();
        double x, y;
        left = CELL_PADDING + w;
        x = r.x + r.width - left;
        y = r.y + r.height - CELL_PADDING - h;
        cr->set_source(emblem, x, y);
        cr->paint();
    }
    return left;
}

void drawLabel(const Cairo::RefPtr<Cairo::Context> & cr, 
               int right, const fwk::RgbColour & colour,
               const GdkRectangle & r)
{
    const int label_size = 15;
    double x, y;
    x = r.x + r.width - CELL_PADDING - right - CELL_PADDING - label_size;
    y = r.y + r.height - CELL_PADDING - label_size;

    cr->rectangle(x, y, label_size, label_size);
    cr->set_source_rgb(1.0, 1.0, 1.0);
    cr->stroke();
    cr->rectangle(x, y, label_size, label_size);
    Gdk::Cairo::set_source_rgba(cr, fwk::rgbcolour_to_gdkcolor(colour));
    cr->fill();
}

}

void 
LibraryCellRenderer::get_preferred_width_vfunc(Gtk::Widget& /*widget*/, int& minimum_width, int& natural_width) const
{
    Glib::RefPtr<Gdk::Pixbuf> pixbuf = property_pixbuf();
    int maxdim = m_size + pad() * 2;
    minimum_width = natural_width = maxdim;
}

void 
LibraryCellRenderer::get_preferred_height_vfunc(Gtk::Widget& /*widget*/, int& minimum_height, int& natural_height) const
{
    Glib::RefPtr<Gdk::Pixbuf> pixbuf = property_pixbuf();
    int maxdim = m_size + pad() * 2;
    minimum_height = natural_height = maxdim;
}

void 
LibraryCellRenderer::render_vfunc(const Cairo::RefPtr<Cairo::Context>& cr, 
                                  Gtk::Widget& widget, 
                                  const Gdk::Rectangle& /*background_area*/, 
                                  const Gdk::Rectangle& cell_area, 
                                  Gtk::CellRendererState flags)
{
    unsigned int xpad = Gtk::CellRenderer::property_xpad();
    unsigned int ypad = Gtk::CellRenderer::property_ypad();

    GdkRectangle r = *(cell_area.gobj());
    r.x += xpad;
    r.y += ypad;

    eng::LibFile::Ptr file = m_libfileproperty.get_value();

    Glib::RefPtr<Gtk::StyleContext> style_context = widget.get_style_context();
    Gdk::RGBA colour;
    if(flags & Gtk::CELL_RENDERER_SELECTED) {
        colour = style_context->get_background_color(Gtk::STATE_FLAG_SELECTED);
    }
    else {
        colour = style_context->get_background_color(Gtk::STATE_FLAG_NORMAL);
    }

    Gdk::Cairo::set_source_rgba(cr, colour);
    cr->rectangle(r.x, r.y, r.width, r.height);
    cr->fill();

    if(m_drawborder) {
        colour = style_context->get_border_color(Gtk::STATE_FLAG_SELECTED);
        Gdk::Cairo::set_source_rgba(cr, colour);
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
    if(m_drawflag) {
        double x, y;
        x = r.x + r.width - CELL_PADDING;
        y = r.y + CELL_PADDING;
        _drawFlag(cr, file->flag(), x, y);
    }
    
    if(m_drawemblem) {
        Cairo::RefPtr<Cairo::ImageSurface> emblem;
        
        switch(file->fileType()) {
        case eng::LibFile::FileType::RAW:
            emblem = m_raw_format_emblem;
            break;
        case eng::LibFile::FileType::RAW_JPEG:
            emblem = m_rawjpeg_format_emblem;
            break;
        case eng::LibFile::FileType::IMAGE:
            emblem = m_img_format_emblem;
            break;
        case eng::LibFile::FileType::VIDEO:
            emblem = m_video_format_emblem;
            break;
        default:
            emblem = m_unknown_format_emblem;
            break;
        }

        int left = drawFormatEmblem(cr, emblem, r);
        if(m_drawlabel) {
            DBG_ASSERT(m_uiDataProvider, "no UIDataProvider");
            uint32_t label_id = file->label();
            if(label_id != 0) {
                auto result = m_uiDataProvider->colourForLabel(label_id);
                DBG_ASSERT(!result.empty(), "colour not found");
                if (!result.empty()) {
                    drawLabel(cr, left, result.unwrap(), r);
                }
            }
        }
    }
}


bool
LibraryCellRenderer::activate_vfunc(GdkEvent * /*event*/, Gtk::Widget & ,
                                    const Glib::ustring &, const Gdk::Rectangle& /*bg*/,
                                    const Gdk::Rectangle & cell_area, Gtk::CellRendererState)
{
    DBG_OUT("activate event");
    if(this->ClickableCellRenderer::is_hit()) {

        this->ClickableCellRenderer::reset_hit();

        // hit test with the rating region
        unsigned int xpad = Gtk::CellRenderer::property_xpad();
        unsigned int ypad = Gtk::CellRenderer::property_ypad();
        GdkRectangle r = *cell_area.gobj();
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
        x = this->ClickableCellRenderer::x();
        y = this->ClickableCellRenderer::y();
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
    return m_libfileproperty.get_proxy();
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
