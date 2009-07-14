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

#include <libopenraw/libopenraw.h>

#include <geglmm/node.h>
#include <geglmm/operation.h>

#include "fwk/base/debug.hpp"
#include "ncr.hpp"
#include "image.hpp"

namespace ncr {

namespace {

const Babl * format_for_cairo_argb32()
{
    // TODO not endian neutral
    return babl_format_new(babl_model("R'G'B'A"),
                           babl_type ("u8"),
#if G_BYTE_ORDER == G_LITTLE_ENDIAN
                           babl_component ("B'"),
                           babl_component ("G'"),
                           babl_component ("R'"),
                           babl_component ("A"),
#elif G_BYTE_ORDER == G_BIG_ENDIAN
                           babl_component ("A"),
                           babl_component ("R'"),
                           babl_component ("G'"),
                           babl_component ("B'"),
#else
#error unknown endian
#endif
                           NULL);
}


}

struct Image::Private {
    Private()
        : m_width(0)
        , m_height(0)
        , m_orientation(0)
        , m_flip(false)
        , m_tilt(0.0)
        {
        }

    int m_width, m_height; /**< the native dimension, with orientation */
    int m_orientation;     /**< EXIF orientation in degrees */
    bool m_flip;           /**< EXIF flip */
    double m_tilt;         /**< User rotation */
    Glib::RefPtr<Gegl::Node> m_node;
    Glib::RefPtr<Gegl::Node> m_rgb;    /**< RGB pixmap */
    Glib::RefPtr<Gegl::Node> m_rotate_n;
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

void Image::reload(const std::string & p, bool is_raw,
    int orientation)
{
    Glib::RefPtr<Gegl::Node> load_file;

    priv->m_node = Gegl::Node::create();
    priv->m_node->set("format", babl_format("RGB u16"));

    if(!is_raw) {
        load_file = priv->m_node->new_child("operation", "gegl:load");
        load_file->set("path", p);
        priv->m_rgb = load_file;
    }
    else {
        ORRawDataRef rawdata;
        or_get_extract_rawdata(p.c_str(), 0, &rawdata);
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
    bool vertical = false;
    switch(orientation) {
    case 0:
    case 1:
        break;
    case 2:
        priv->m_flip = true;
        break;
    case 4:
        priv->m_flip = true;
        // fall through
    case 3:
        priv->m_orientation = 180;
        break;
    case 5:
        priv->m_flip = true;
        // fall through
    case 6:
        priv->m_orientation = 270;
        vertical = true;
        break;
    case 7:
        priv->m_flip = true;
        // fall through
    case 8:
        priv->m_orientation = 90;
        vertical = true;
        break;
    }
    // @todo ideally we would have a plain GEGL op for that.
    if(priv->m_flip) {
        // @todo find a test case.
        priv->m_rotate_n =  priv->m_node->new_child("operation", 
                                                    "gegl:reflect");
        priv->m_rotate_n->set("x", -1.0);
        current = priv->m_rgb->link(priv->m_rotate_n);
    }
    else {
        current = priv->m_rgb;
    }
    priv->m_rotate_n = priv->m_node->new_child("operation", "gegl:rotate");
    priv->m_rotate_n->set("degrees", priv->m_orientation + priv->m_tilt);
    current = current->link(priv->m_rotate_n);

    priv->m_scale = priv->m_node->new_child("operation", "gegl:scale");
    current->link(priv->m_scale);
    priv->m_output = priv->m_scale;
    
    int width, height;
    Gegl::Rectangle rect;
    rect = load_file->get_bounding_box();
    width = rect.gobj()->width;
    height = rect.gobj()->height;
    DBG_OUT("width %d height %d", width, height);
    if(vertical) {
        priv->m_width = height;
        priv->m_height = width;
    }
    else {
        priv->m_width = width;
        priv->m_height = height;
    }

    signal_update();
}

void Image::set_output_scale(double scale)
{
    DBG_OUT("scale %f", scale);
    if(!priv->m_scale) {
        DBG_OUT("nothing loaded");
        return;
    }
    priv->m_scale->set("x", scale);
    priv->m_scale->set("y", scale);    

    signal_update();
}



void Image::set_tilt(double angle)
{
    if(!priv->m_rotate_n) {
        DBG_OUT("nothing loaded");
        return;
    }
    priv->m_tilt = angle;
    priv->m_rotate_n->set("degrees", priv->m_orientation + priv->m_tilt);

    signal_update();    
}


void Image::rotate_left()
{
    rotate_by(-90);
}

void Image::rotate_right()
{
    rotate_by(90);
}

void Image::rotate_half()
{
    rotate_by(180);
}


void Image::rotate_by(int degree)
{
    priv->m_orientation += degree;
    if(priv->m_orientation < 0) {
        // negative values aren't good
        priv->m_orientation += 360;
    }
    else {
        // within 0..359 degrees anyway
        priv->m_orientation %= 360;
    }
    priv->m_rotate_n->set("degrees", priv->m_orientation + priv->m_tilt);
    signal_update();
}



Cairo::RefPtr<Cairo::Surface> Image::cairo_surface_for_display()
{
    if(!priv->m_output) {
        DBG_OUT("nothing loaded");
        return Cairo::RefPtr<Cairo::Surface>();
    }
    priv->m_output->process();
    Gegl::Rectangle roi = priv->m_output->get_bounding_box();
    int w, h;
    w = roi.gobj()->width;
    h = roi.gobj()->height;

    const Babl * format = format_for_cairo_argb32();

    Cairo::RefPtr<Cairo::ImageSurface> surface 
        = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, w, h);
    priv->m_output->blit(1.0, roi, format,
                         (void*)surface->get_data(), surface->get_stride(),
                         (Gegl::BLIT_CACHE | Gegl::BLIT_DIRTY));
    return surface;
}

int Image::get_original_width() const
{
    return priv->m_width;
}


int Image::get_original_height() const
{
    return priv->m_height;
}


int Image::get_output_width() const
{
    Gegl::Rectangle roi = priv->m_output->get_bounding_box();
    return roi.gobj()->width;
}


int Image::get_output_height() const
{
    Gegl::Rectangle roi = priv->m_output->get_bounding_box();
    return roi.gobj()->height;
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
