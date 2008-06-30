/*
 * niepce - modules/darkroom/image.cpp
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


extern "C" {
#include <babl/babl.h>
}

#include <boost/bind.hpp>

#include <geglmm/node.h>
#include <geglmm/operation.h>

#include "utils/debug.h"
#include "image.h"

namespace darkroom {

struct Image::Private {
    Private()
        : m_width(0),
          m_height(0)
        {
        }

    int m_width, m_height; /**< the native dimension */
    Glib::RefPtr<Gegl::Node> m_node;
    Glib::RefPtr<Gegl::Node> m_scale;
    Glib::RefPtr<Gegl::Node> m_output;
};

Image::Image()
    : priv(new Private)
{
}

Image::~Image()
{
    delete priv;
}

void Image::reload(const boost::filesystem::path & p)
{
    Glib::RefPtr<Gegl::Node> load_file;

    priv->m_node = Gegl::Node::create();
    load_file = priv->m_node->new_child("operation", "load");
    load_file->set("path", p.string());
    
    priv->m_scale = priv->m_node->new_child("operation", "scale");
    set_scale(0.25);
    load_file->link(priv->m_scale);
    priv->m_output = priv->m_scale;
    
    int width, height;
    Gegl::Rectangle rect;
    rect = load_file->get_bounding_box();
    width = rect.gobj()->width;
    height = rect.gobj()->height;
    DBG_OUT("width %d height %d", width, height);
    priv->m_width = width;
    priv->m_height = height;
}

void Image::set_scale(double scale)
{
    DBG_OUT("scale %f", scale);
    priv->m_scale->set("x", scale);
    priv->m_scale->set("y", scale);    
}

void Image::set_scale_to_dim(int w, int h)
{
    double in_w = priv->m_width;
    double in_h = priv->m_height;
    double scale_w = w / in_w;
    DBG_OUT("w %d in_w %f", w, in_w);
    double scale_h = h / in_h;
    DBG_OUT("h %d in_h %f", h, in_h);
    DBG_OUT("scale w %f h %f", scale_w, scale_h);
    double scale = std::min(scale_w, scale_h);
    set_scale(scale);
}

namespace {

/** callback to free the buffer */
void _free_buf(const guint8* p) 
{
    g_free(const_cast<guint8*>(p));
}

}

Glib::RefPtr<Gdk::Pixbuf> Image::pixbuf_for_display()
{
    priv->m_output->process();
    Gegl::Rectangle roi = priv->m_output->get_bounding_box();
    int w, h;
    w = roi.gobj()->width;
    h = roi.gobj()->height;
    guint8* buf = (guint8*)g_malloc (w * h * 3);
    priv->m_output->blit(1.0, roi, babl_format ("R'G'B' u8"),
                       (void*)buf, GEGL_AUTO_ROWSTRIDE,
                       (Gegl::BlitFlags)(GEGL_BLIT_CACHE | GEGL_BLIT_DIRTY));
    Glib::RefPtr<Gdk::Pixbuf> pix 
        = Gdk::Pixbuf::create_from_data(buf,
                                        Gdk::COLORSPACE_RGB,
                                        false, 8, w, h,
                                        w * 3, boost::bind(&_free_buf, _1));
    return pix;
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
