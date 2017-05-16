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

#pragma once

#include <stdint.h>

#include <string>

#include <gdkmm/pixbuf.h>

namespace fwk {

class Thumbnail {
public:
  Thumbnail() = default;
  Thumbnail(const Glib::RefPtr<Gdk::Pixbuf>& pixbuf);

  bool ok() const {
    return !!m_pixbuf;
  }

  int get_width() const {
    return m_pixbuf->get_width();
  }
  int get_height() const {
    return m_pixbuf->get_height();
  }
  const Glib::RefPtr<Gdk::Pixbuf>& pixbuf() const {
    return m_pixbuf;
  }

  void save(const std::string& path, const std::string& format);

  static Thumbnail thumbnail_file(const std::string& path, int w, int h, int32_t orientation);

private:
  Glib::RefPtr<Gdk::Pixbuf> m_pixbuf;
};

}
