/* -*- mode: C++; tab-width: 2; c-basic-offset: 2; indent-tabs-mode:nil; -*- */
/*
 * niepce - fwk/toolkit/thumbnail.cpp
 *
 * Copyright (C) 2017 Hubert Figuière
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
#include <libopenraw-gnome/gdkpixbuf.h>

#include "fwk/base/debug.hpp"
#include "fwk/toolkit/mimetype.hpp"
#include "fwk/toolkit/gdkutils.hpp"
#include "fwk/toolkit/movieutils.hpp"

namespace fwk {

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
    GdkPixbuf *pixbuf = or_gdkpixbuf_extract_rotated_thumbnail(filename.c_str(),
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
