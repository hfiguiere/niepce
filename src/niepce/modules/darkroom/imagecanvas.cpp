/*
 * niepce - darkroom/imagecanvas.cpp
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

/* remove this when we require a version that does not barf warnings */
#include "fwk/toolkit/goocanvas_proxy_header.hpp"
//#include <goocanvasmm/canvas.h>
#include "fwk/utils/debug.h"
#include "fwk/utils/geometry.h"

#include "imagecanvas.h"

namespace darkroom {

ImageCanvas::ImageCanvas()
    : Goocanvas::Canvas(),
      m_need_redisplay(true),
      m_zoom_mode(ZOOM_MODE_FIT)
{
    m_imagecanvas = this;
    Glib::RefPtr<Goocanvas::Item> root;
    root = m_imagecanvas->get_root_item ();
    
    m_frameitem = Goocanvas::Rect::create(0,0,0,0);
    m_imageitem = Goocanvas::Image::create(0,0);
    
    root->add_child(m_frameitem);
    root->add_child(m_imageitem);
}


void ImageCanvas::set_image(const Glib::RefPtr<Gdk::Pixbuf> & img)
{
    m_need_redisplay = true;
    m_image = img;
    _redisplay();
}


void ImageCanvas::_calc_image_frame(int img_w, int img_h,
                                   double & x, double & y,
                                   double & width, double & height)
{
    double b_w, b_h;
    b_w = m_imagecanvas->get_width();
    b_h = m_imagecanvas->get_height();
//    DBG_OUT("bounds %f %f", b_w, b_h);
    x = (b_w - img_w) / 2;
    y = (b_h - img_h) / 2;
    width = img_w;
    height = img_h;
//    DBG_OUT("image frame %f %f %f %f", x, y, width, height);  
}


void ImageCanvas::_redisplay(bool force)
{
    if(m_need_redisplay || force) {
        Glib::RefPtr<Gdk::Pixbuf> img = m_image;
        DBG_OUT("set image w %d h %d", img->get_width(), img->get_height());
        
        // the position and dimension of the image frame
        double x, y, w, h;

        utils::Rect dest(0,0, m_imagecanvas->get_width() - 8,
            m_imagecanvas->get_height() - 8);
        utils::Rect source(0,0, img->get_width(), img->get_height());
        utils::Rect frame;
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
        
        _calc_image_frame(frame.w(), frame.h(),
                          x,y,w,h);

        m_imageitem->property_x() = x;
        m_imageitem->property_y() = y;
        m_imageitem->property_width() = w;
        m_imageitem->property_height() = h;
        m_imageitem->property_pixbuf() = img->scale_simple(w, h, 
                                                           Gdk::INTERP_BILINEAR);

        m_frameitem->property_x() = x;
        m_frameitem->property_y() = y;
        m_frameitem->property_width() = w;
        m_frameitem->property_height() = h;        

        m_need_redisplay = false;
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
