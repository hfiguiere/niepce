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
#include <gegl.h>
}

#include <libopenraw/libopenraw.h>

#include "fwk/base/debug.hpp"
#include "ncr.hpp"
#include "image.hpp"

namespace ncr {

struct Image::Private {
    Private()
        : m_status(Image::STATUS_UNSET)
        , m_width(0)
        , m_height(0)
        , m_orientation(0), m_vertical(false)
        , m_flip(false)
        , m_tilt(0.0)
        , m_graph(NULL), m_rgb(NULL), m_rotate_n(NULL)
        , m_scale(NULL)
        , m_sink_buffer(NULL)
        {
        }
    ~Private()
        {
            if(m_graph) {
                g_object_unref(m_graph);
            }
            if(m_sink_buffer) {
                g_object_unref(m_sink_buffer);
            }
        }

    Image::status_t m_status;
    int m_width, m_height; /**< the native dimension, with orientation */
    int m_orientation;     /**< EXIF orientation in degrees */
    bool m_vertical;
    bool m_flip;           /**< EXIF flip */
    double m_tilt;         /**< User rotation */
    GeglNode* m_graph;
    GeglNode* m_rgb;    /**< RGB pixmap */
    GeglNode* m_rotate_n;
    GeglNode* m_scale;
    // sink
    GeglNode*   m_sink;
    GeglBuffer* m_sink_buffer;

    GeglNode* _rotate_node(int orientation);
    GeglNode* _scale_node();
    GeglNode* _load_raw(const std::string &path);
};

Image::Image()
    : priv(new Private)
{
}

Image::~Image()
{
    delete priv;
}

GeglNode* Image::Private::_rotate_node(int orientation)
{
//    GeglNode* rotateNode = gegl_node_new_child(m_graph, NULL, NULL);

    DBG_OUT("rotation is %d", orientation);
    switch(orientation) {
    case 0:
    case 1:
        break;
    case 2:
        m_flip = true;
        break;
    case 4:
        m_flip = true;
        // fall through
    case 3:
        m_orientation = 180;
        break;
    case 5:
        m_flip = true;
        // fall through
    case 6:
        m_orientation = 270;
        m_vertical = true;
        break;
    case 7:
        m_flip = true;
        // fall through
    case 8:
        m_orientation = 90;
        m_vertical = true;
        break;
    }

    // @todo ideally we would have a plain GEGL op for that.
    // @todo find a test case.
//    GeglNode* flipNode = gegl_node_new_child(rotateNode,
//                                             "operation", "gegl:reflect",
//                                             "x", (m_flip ? -1.0 : 1.0), NULL);

    GeglNode* rotationNode = NULL;
    double rotate = m_orientation + m_tilt;
    rotationNode = gegl_node_new_child(m_graph,
                                       "operation", "gegl:rotate",
                                       "degrees", rotate, NULL);
//    gegl_node_link(flipNode, rotationNode);

    return rotationNode;
}

/** create the RAW pipeline */
GeglNode* Image::Private::_load_raw(const std::string &p)
{
    GeglNode* rgb = NULL;

    ORRawDataRef rawdata;
    or_get_extract_rawdata(p.c_str(), 0, &rawdata);
    GeglBuffer* buffer = ncr::load_rawdata(rawdata);
    // @todo can return a NULL buffer if load failed. Deal with that.
    GeglNode* load_file = gegl_node_new_child(m_graph,
                                              "operation", "gegl:buffer-source",
                                              "buffer", buffer, NULL);
    or_cfa_pattern pattern = or_rawdata_get_cfa_pattern(rawdata);
    or_rawdata_release(rawdata);

    GeglNode* stretch =
        gegl_node_new_child(m_graph,
                            "operation", "gegl:stretch-contrast",
                            NULL);
    GeglNode* demosaic =
        gegl_node_new_child(m_graph,
                            "operation", "gegl:demosaic-bimedian",
                            NULL);
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
    gegl_node_set(demosaic, "pattern", npattern, NULL);

    gegl_node_link_many(load_file, stretch, demosaic, NULL);

    rgb = demosaic;
    return rgb;
}

GeglNode* Image::Private::_scale_node()
{
    return gegl_node_new_child(m_graph, "operation", "gegl:scale", NULL);
}

void Image::reload(const std::string & p, bool is_raw,
    int orientation)
{
    GeglNode* load_file;

    priv->m_status = STATUS_LOADING;

    priv->m_graph = gegl_node_new();
//    priv->m_graph->set("format", babl_format("RGB u16"));

    DBG_OUT("loading file %s", p.c_str());

    if(!is_raw) {
        load_file = gegl_node_new_child(priv->m_graph,
                                        "operation", "gegl:load",
                                        "path", p.c_str(), NULL);
    }
    else {
        load_file = priv->_load_raw(p);
    }

    priv->m_rotate_n = priv->_rotate_node(orientation);
    priv->m_scale = priv->_scale_node();
    priv->m_sink = gegl_node_create_child (priv->m_graph, "gegl:display");

//        gegl_node_new_child(priv->m_graph,
//                            "operation", "gegl:buffer-sink",
//                            "format", babl_format("RGB u8"),
//                            "buffer", &(priv->m_sink_buffer), NULL);

    gegl_node_link_many(load_file, priv->m_rotate_n,
                        priv->m_scale, priv->m_sink, NULL);

//    gegl_node_process(priv->m_sink);
    // DEBUG
#if 0
    GeglNode* debugsink;
    GeglNode* graph =
        gegl_graph (debugsink=gegl_node ("gegl:save",
                                         "path", "/tmp/gegl.png", NULL,
                                         gegl_node ("gegl:buffer-source",
                                                    "buffer",
                                                    priv->m_sink_buffer,
                                                    NULL)
                        )
            );
    gegl_node_process (debugsink);
    g_object_unref (graph);
#endif
    // END DEBUG

    int width, height;
    GeglRectangle rect = gegl_node_get_bounding_box(load_file);
    width = rect.width;
    height = rect.height;
    DBG_OUT("width %d height %d", width, height);
    if(!width || !height) {
        priv->m_status = STATUS_ERROR;
    }
    if(priv->m_vertical) {
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
    gegl_node_set(priv->m_scale, "x", scale, "y", scale, NULL);

    signal_update();
}



void Image::set_tilt(double angle)
{
    if(!priv->m_rotate_n) {
        DBG_OUT("nothing loaded");
        return;
    }
    priv->m_tilt = angle;
    gegl_node_set(priv->m_rotate_n, "degrees",
                  priv->m_orientation + priv->m_tilt, NULL);

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
    gegl_node_set(priv->m_rotate_n, "degrees", priv->m_orientation + priv->m_tilt, NULL);
    signal_update();
}



Cairo::RefPtr<Cairo::ImageSurface> Image::cairo_surface_for_display()
{
    if(priv->m_status == STATUS_ERROR) {
        // return the error
        DBG_OUT("error");
        return Cairo::RefPtr<Cairo::ImageSurface>();
    }
    if(!priv->m_sink) {
        DBG_OUT("nothing loaded");
        return Cairo::RefPtr<Cairo::ImageSurface>();
    }
    DBG_ASSERT(priv->m_status == STATUS_LOADING, "wrong status for image");
    DBG_OUT("processing");
    gegl_node_process(priv->m_scale);
    GeglRectangle roi = gegl_node_get_bounding_box(priv->m_scale);
    int w, h;
    w = roi.width;
    h = roi.height;

    DBG_OUT("surface w=%d, h=%d", w, h);

    const Babl* format = babl_format("B'aG'aR'aA u8");

    Cairo::RefPtr<Cairo::ImageSurface> surface
        = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, w, h);
    gegl_node_blit(priv->m_scale, 1.0, &roi, format,
                   (void*)surface->get_data(), surface->get_stride(),
                   (GeglBlitFlags)(GEGL_BLIT_CACHE | GEGL_BLIT_DIRTY));

    // If you don't do that, it will never paint().
    // Thanks to mitch for the tip in #gegl
    surface->mark_dirty();
    priv->m_status = STATUS_LOADED;
    return surface;
}

Image::status_t Image::get_status() const
{
    return priv->m_status;
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
    GeglRectangle roi = gegl_node_get_bounding_box(priv->m_sink);
    return roi.width;
}


int Image::get_output_height() const
{
    GeglRectangle roi = gegl_node_get_bounding_box(priv->m_sink);
    return roi.height;
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
