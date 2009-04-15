/*
 * niepce - fwk/toolkit/gdkutils.hpp
 *
 * Copyright (C) 2008-2009 Hubert Figuiere
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


#ifndef __FWK_GDKUTILS_H__
#define __FWK_GDKUTILS_H__

#include <gdkmm/pixbuf.h>

#include "fwk/base/color.hpp"

namespace fwk {
	
	/** scale the pixbuf to fit in the square 
	 * @param dim the dimension of the square
	 */
	Glib::RefPtr<Gdk::Pixbuf> gdkpixbuf_scale_to_fit(const Glib::RefPtr<Gdk::Pixbuf> & pix,
													 int dim);
	/** Rotate a pixbuf following the Exif rotation (may mirror too) */
	Glib::RefPtr<Gdk::Pixbuf> gdkpixbuf_exif_rotate(const Glib::RefPtr<Gdk::Pixbuf> & pixbuf,
													int exif_orientation);

  Gdk::Color rgbcolor_to_gdkcolor(const fwk::RgbColor & color);
  fwk::RgbColor gdkcolor_to_rgbcolor(const Gdk::Color & color);

}

#endif
