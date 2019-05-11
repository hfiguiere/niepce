/* -*- mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode:nil; -*- */
/*
 * niepce - fwk/toolkit/thumbnail.cpp
 *
 * Copyright (C) 2017-2019 Hubert Figuière
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

#include "thumbnail.hpp"

#include <glibmm/miscutils.h>
#include <libopenraw/libopenraw.h>

#include "fwk/base/debug.hpp"
#include "fwk/toolkit/mimetype.hpp"
#include "fwk/toolkit/gdkutils.hpp"
#include "fwk/toolkit/movieutils.hpp"

namespace fwk {

namespace {

static void pixbuf_free(guchar* data, gpointer u)
{
    (void)u;
    free(data);
}

/**
 * Returns a retained GdkPixbuf
 */
static GdkPixbuf* rotate_pixbuf(GdkPixbuf* tmp, int32_t orientation)
{
    GdkPixbuf* pixbuf = nullptr;
    switch (orientation) {
    case 0:
    case 1:
        pixbuf = (GdkPixbuf*)g_object_ref(tmp);
        break;
    case 2:
        pixbuf = gdk_pixbuf_flip(tmp, TRUE);
        break;
    case 3:
        pixbuf = gdk_pixbuf_rotate_simple(tmp, GDK_PIXBUF_ROTATE_UPSIDEDOWN);
        break;
    case 4: {
        GdkPixbuf* rotated =
            gdk_pixbuf_rotate_simple(tmp, GDK_PIXBUF_ROTATE_UPSIDEDOWN);
        pixbuf = gdk_pixbuf_flip(rotated, TRUE);
        g_object_unref(rotated);
        break;
    }
    case 5: {
        GdkPixbuf* rotated =
            gdk_pixbuf_rotate_simple(tmp, GDK_PIXBUF_ROTATE_CLOCKWISE);
        pixbuf = gdk_pixbuf_flip(rotated, FALSE);
        g_object_unref(rotated);
        break;
    }
    case 6:
        pixbuf = gdk_pixbuf_rotate_simple(tmp, GDK_PIXBUF_ROTATE_CLOCKWISE);
        break;
    case 7: {
        GdkPixbuf* rotated =
            gdk_pixbuf_rotate_simple(tmp, GDK_PIXBUF_ROTATE_COUNTERCLOCKWISE);
        pixbuf = gdk_pixbuf_flip(rotated, FALSE);
        g_object_unref(rotated);
        break;
    }
    case 8:
        pixbuf =
            gdk_pixbuf_rotate_simple(tmp, GDK_PIXBUF_ROTATE_COUNTERCLOCKWISE);
        break;
    default:
        break;
    }
    return pixbuf;
}

static GdkPixbuf* thumbnail_to_pixbuf(ORThumbnailRef thumbnail,
                                      int32_t orientation)
{
    GdkPixbuf* tmp = nullptr;
    GdkPixbuf* pixbuf = nullptr;

    const guchar* buf;
    or_data_type format = or_thumbnail_format(thumbnail);
    buf = (const guchar*)or_thumbnail_data(thumbnail);

    switch (format) {
    case OR_DATA_TYPE_PIXMAP_8RGB: {
        uint32_t x, y;
        size_t buf_size;
        guchar* data;

        buf_size = or_thumbnail_data_size(thumbnail);
        data = (guchar*)malloc(buf_size);
        memcpy(data, buf, buf_size);
        or_thumbnail_dimensions(thumbnail, &x, &y);

        tmp = gdk_pixbuf_new_from_data(data, GDK_COLORSPACE_RGB, FALSE, 8, x, y,
                                       x * 3, pixbuf_free, nullptr);
        break;
    }
    case OR_DATA_TYPE_JPEG:
    case OR_DATA_TYPE_TIFF:
    case OR_DATA_TYPE_PNG: {
        GdkPixbufLoader* loader = nullptr;
        size_t count = or_thumbnail_data_size(thumbnail);
        loader = gdk_pixbuf_loader_new();
        if (loader != nullptr) {
            GError* error = nullptr;
            if (!gdk_pixbuf_loader_write(loader, buf, count, &error) && error) {
                ERR_OUT("loader write error: %s", error->message);
                g_error_free(error);
                error = nullptr;
            }
            if (!gdk_pixbuf_loader_close(loader, &error) && error) {
                ERR_OUT("loader close error: %s", error->message);
                g_error_free(error);
            }
            tmp = gdk_pixbuf_loader_get_pixbuf(loader);
            g_object_ref(tmp);
            g_object_unref(loader);
        }
        break;
    }
    default:
        break;
    }
    pixbuf = rotate_pixbuf(tmp, orientation);
    g_object_unref(tmp);
    return pixbuf;
}

static GdkPixbuf* gdkpixbuf_extract_rotated_thumbnail(const char* path,
                                                      uint32_t preferred_size)
{
    ORRawFileRef rf;
    int32_t orientation = 0;
    GdkPixbuf* pixbuf = nullptr;
    or_error err = OR_ERROR_NONE;
    ORThumbnailRef thumbnail = nullptr;
    gboolean rotate = TRUE;

    rf = or_rawfile_new(path, OR_RAWFILE_TYPE_UNKNOWN);
    if (rf) {
        if (rotate) {
            orientation = or_rawfile_get_orientation(rf);
        }
        thumbnail = or_thumbnail_new();
        err = or_rawfile_get_thumbnail(rf, preferred_size, thumbnail);
        if (err == OR_ERROR_NONE) {
            pixbuf = thumbnail_to_pixbuf(thumbnail, orientation);
        } else {
            ERR_OUT("or_get_extract_thumbnail() failed with %d.", err);
        }
        err = or_thumbnail_release(thumbnail);
        if (err != OR_ERROR_NONE) {
            ERR_OUT("or_thumbnail_release() failed with %d", err);
        }
        or_rawfile_release(rf);
    }

    return pixbuf;
}

}

Thumbnail::Thumbnail(const Glib::RefPtr<Gdk::Pixbuf>& pixbuf)
    : m_pixbuf(pixbuf)
{
}

void Thumbnail::save(const std::string& path, const std::string& format)
{
    if (m_pixbuf) {
        m_pixbuf->save(path, format);
    }
}

Thumbnail Thumbnail::thumbnail_file(const std::string& filename,
                                    int w, int h, int32_t orientation)
{
    fwk::MimeType mime_type(filename);
    DBG_OUT("MIME type %s", mime_type.string().c_str());

    Glib::RefPtr<Gdk::Pixbuf> pix;

    if(mime_type.isUnknown()) {
        DBG_OUT("unknown file type %s", filename.c_str());
    } else if(mime_type.isMovie()) {
        try {
            // XXX FIXME
            std::string cached = Glib::get_tmp_dir() + "/temp-1234";
            if(fwk::thumbnail_movie(filename, w, h, cached)) {
                pix = Gdk::Pixbuf::create_from_file(cached, w, h, true);
                unlink(cached.c_str());
            }
        }
        catch(const Glib::Error & e) {
            ERR_OUT("exception thumbnailing video %s", e.what().c_str());
        }
    } else if(!mime_type.isImage()) {
        DBG_OUT("not an image type");
    } else if(!mime_type.isDigicamRaw()) {
        DBG_OUT("not a raw type, trying GdkPixbuf loaders");
        try {
            pix = Gdk::Pixbuf::create_from_file(filename, w, h, true);
            if(pix) {
                pix = fwk::gdkpixbuf_exif_rotate(pix, orientation);
            }
        }
        catch(const Glib::Error & e) {
            ERR_OUT("exception thumbnailing image %s", e.what().c_str());
        }
    } else {
        GdkPixbuf *pixbuf = gdkpixbuf_extract_rotated_thumbnail(filename.c_str(),
                                                                std::min(w, h));
        if(pixbuf) {
            pix = Glib::wrap(pixbuf, true); // take ownership
            if((w < pix->get_width()) || (h < pix->get_height())) {
                pix = fwk::gdkpixbuf_scale_to_fit(pix, std::min(w,h));
            }
        }
    }

    return Thumbnail(pix);
}

}
