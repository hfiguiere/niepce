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

#include "framework/goocanvas_proxy_header.h"

#include "utils/debug.h"

#include "imagecanvas.h"

namespace darkroom {

ImageCanvas::ImageCanvas()
    : Goocanvas::Canvas()
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
    DBG_OUT("set image w %d h %d", img->get_width(), img->get_height());
    m_imageitem->property_pixbuf() = img;
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
