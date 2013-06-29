/*
 * niepce - darkroom/imagecanvas.cpp
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

#include <cairomm/context.h>
#include <gdkmm.h>


#include "fwk/base/debug.hpp"
#include "fwk/base/geometry.hpp"

#include "imagecanvas.hpp"

#include <gdkmm/general.h>

#ifndef DATADIR
#error DATADIR is not defined
#endif

namespace dr {

#define IMAGE_INSET 6
#define SHADOW_OFFSET 3


ImageCanvas::ImageCanvas()
    : m_need_redisplay(true),
      m_zoom_mode(ZOOM_MODE_FIT)
{
}


void ImageCanvas::set_image(const ncr::Image::Ptr & img)
{
    m_need_redisplay = true;
    m_image_reloaded_cid.disconnect();
    m_image = img;
    m_image_reloaded_cid = m_image->signal_update.connect(
        sigc::mem_fun(*this, &ImageCanvas::on_image_reloaded));
}


void ImageCanvas::on_image_reloaded()
{
    m_need_redisplay = true;
    invalidate();
}

void ImageCanvas::invalidate()
{
    Glib::RefPtr<Gdk::Window> w = get_window();
    if(w) {
        w->invalidate(false);
    }
}

double ImageCanvas::_calc_image_scale(int img_w, int img_h)
{
    double b_w, b_h;
    b_w = get_width() - (IMAGE_INSET * 2);
    b_h = get_height() - (IMAGE_INSET * 2);

    double scale_w = b_w / img_w;
    double scale_h = b_h / img_h;
    return std::min(scale_w, scale_h);
}


void ImageCanvas::_calc_image_frame(int img_w, int img_h,
                                   double & x, double & y,
                                   double & width, double & height)
{
    double b_w, b_h;
    b_w = get_width();
    b_h = get_height();
//    DBG_OUT("bounds %f %f", b_w, b_h);
    x = (b_w - img_w) / 2;
    y = (b_h - img_h) / 2;
    width = img_w;
    height = img_h;
//    DBG_OUT("image frame %f %f %f %f", x, y, width, height);  
}

Cairo::RefPtr<Cairo::ImageSurface> ImageCanvas::_get_error_placeholder()
{
    Cairo::RefPtr<Cairo::ImageSurface> s;
    try {
        s = Cairo::ImageSurface::create_from_png(
                std::string(DATADIR"/niepce/pixmaps/niepce-image-generic.png"));
    }
    catch(...) {
    }

    return s;
}

bool ImageCanvas::on_draw(const Cairo::RefPtr<Cairo::Context>& context)
{
    // no image, just pass.
    if(!m_image) {
        DBG_OUT("no image");
        return false;
    }

    if(m_need_redisplay) {
        _redisplay();

        Cairo::RefPtr<Cairo::ImageSurface> img_s;

        int img_w, img_h;
        double scale = 1.0;

        if(m_image->get_status() != ncr::Image::STATUS_ERROR) {

            // calculate the image scale
            img_w = m_image->get_original_width();
            img_h = m_image->get_original_height();
            DBG_OUT("image w = %d ; h = %d", img_w, img_h);
            scale = _calc_image_scale(img_w, img_h);
            DBG_OUT("scale = %f", scale);
            m_image->set_output_scale(scale);


            // query the image.
            img_s = m_image->cairo_surface_for_display();
        }
        if(!img_s) {
            DBG_OUT("no image loaded");
            img_s = _get_error_placeholder();
            img_w = img_s->get_width();
            img_h = img_s->get_height();
        }

        int canvas_h, canvas_w;
        canvas_h = get_height();
        canvas_w = get_width();
        DBG_OUT("canvas w = %d ; h = %d", canvas_w, canvas_h);

        m_backingstore
            = Cairo::Surface::create(img_s, Cairo::CONTENT_COLOR,
                                     canvas_w, canvas_h);
        Cairo::RefPtr<Cairo::Context> sc
            = Cairo::Context::create(m_backingstore);


//        sc->set_antialias(Cairo::ANTIALIAS_NONE);

        // paint the background
        sc->rectangle(0, 0, canvas_w, canvas_h);
        Gdk::RGBA color = get_style_context()->get_background_color(Gtk::STATE_FLAG_NORMAL);
        Gdk::Cairo::set_source_rgba(sc, color);
        sc->fill();


        double out_w = (img_w * scale);
        double out_h = (img_h * scale);
        double x = (canvas_w - out_w) / 2;
        double y = (canvas_h - out_h) / 2;
        DBG_OUT("x = %f ; y = %f", x, y);

        sc->rectangle(x + SHADOW_OFFSET, y + SHADOW_OFFSET + 1, out_w, out_h);
        sc->set_source_rgb(0.0, 0.0, 0.0);
        sc->fill();

        sc->set_source(img_s, x, y);
        sc->paint();

//        sc->set_source_rgb(1.0, 1.0, 1.0);
//        sc->set_line_width(1.0);
//        sc->rectangle(x, y, out_w, out_h);
//        sc->stroke();

        m_need_redisplay = false;
    }

    context->set_source(m_backingstore, 0, 0);
    context->paint();

    return true;
}


void ImageCanvas::_redisplay()
{
    if (m_image->get_status() == ncr::Image::STATUS_ERROR) {
        ERR_OUT("Image is in error");
        return;
    }
    int img_w, img_h;
    img_w = m_image->get_original_width();
    img_h = m_image->get_original_height();
    DBG_OUT("set image w %d h %d", img_w, img_h);

    fwk::Rect dest(0,0, get_width() - 8, get_height() - 8);
    fwk::Rect source(0,0, img_w, img_h);
    fwk::Rect frame;
    switch(m_zoom_mode)
    {
    case ZOOM_MODE_FIT:
        frame = source.fit_into(dest);
        break;
    case ZOOM_MODE_FILL:
        frame = source.fill_into(dest);
        break;
    default:
        frame = source;
        break;
    }
}

}


/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
