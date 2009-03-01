/*
 * niepce - ncr/image.cpp
 *
 * Copyright (C) 2008-2009 Hubert Figuiere
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, see 
 * <http://www.gnu.org/licenses/>.
 */


extern "C" {
#include <babl/babl.h>
}

#include <boost/bind.hpp>

#include <libopenraw/libopenraw.h>

#include <geglmm/node.h>
#include <geglmm/operation.h>

#include "fwk/utils/debug.h"
#include "ncr.h"
#include "image.h"

namespace ncr {

struct Image::Private {
    Private()
        : m_width(0),
          m_height(0)
        {
        }

    int m_width, m_height; /**< the native dimension */
    Glib::RefPtr<Gegl::Node> m_node;
    Glib::RefPtr<Gegl::Node> m_rgb;    /**< RGB pixmap */
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

void Image::reload(const boost::filesystem::path & p, bool is_raw,
    int orientation)
{
    Glib::RefPtr<Gegl::Node> load_file;

    priv->m_node = Gegl::Node::create();
    priv->m_node->set("format", babl_format("RGB u16"));

    if(!is_raw) {
        load_file = priv->m_node->new_child("operation", "gegl:load");
        load_file->set("path", p.string());
        priv->m_rgb = load_file;
    }
    else {
        ORRawDataRef rawdata;
        or_get_extract_rawdata(p.string().c_str(), 0, &rawdata);
        Glib::RefPtr<Gegl::Buffer> buffer = ncr::load_rawdata(rawdata);
        // @todo can return a NULL buffer if load failed. Deal with that.
        load_file = priv->m_node->new_child("operation", "gegl:load-buffer");
        load_file->set("buffer", buffer);
        or_cfa_pattern pattern = or_rawdata_get_cfa_pattern(rawdata);
        or_rawdata_release(rawdata);

        Glib::RefPtr<Gegl::Node> stretch = priv->m_node->new_child(
            "operation", "gegl:stretch-contrast");
        
        Glib::RefPtr<Gegl::Node> demosaic = priv->m_node->new_child(
            "operation", "gegl:demosaic-bimedian");
        // @todo refactor somewhere.
        int npattern = 0;
        switch(pattern) {
        case OR_CFA_PATTERN_GRBG:
            npattern = 0;
            break;
        case OR_CFA_PATTERN_BGGR:
            npattern = 1;
            break;
        case OR_CFA_PATTERN_GBRG:
            npattern = 2;
            break;
        case OR_CFA_PATTERN_RGGB:
            npattern = 3;
            break;
        default:
            break;
        }

        demosaic->set("pattern", npattern);

        priv->m_rgb = load_file->link(stretch)->link(demosaic);
    }
    
    Glib::RefPtr<Gegl::Node> current;

    DBG_OUT("rotation is %d", orientation);
    int degrees = 0;
    bool flip = false;
    switch(orientation) {
    case 0:
    case 1:
        break;
    case 2:
        flip = true;
        break;
    case 4:
        flip = true;
        // fall through
    case 3:
        degrees = 180;
        break;
    case 5:
        flip = true;
        // fall through
    case 6:
        degrees = 270;
        break;
    case 7:
        flip = true;
        // fall through
    case 8:
        degrees = 90;
        break;
    }
    // @todo ideally we would have a plain GEGL op for that.
    Glib::RefPtr<Gegl::Node> rotate;
    if(flip) {
        // @todo find a test case.
        rotate =  priv->m_node->new_child("operation", "gegl:reflect");
        rotate->set("x", -1.0);
        current = priv->m_rgb->link(rotate);
    }
    else {
        current = priv->m_rgb;
    }
    rotate = priv->m_node->new_child("operation", "gegl:rotate");
    rotate->set("degrees", degrees);
    current = current->link(rotate);

    priv->m_scale = priv->m_node->new_child("operation", "gegl:scale");
    set_scale(0.25);
    current->link(priv->m_scale);
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
  fill-column:80
  End:
*/
