/*
 * niepce - fwk/toolkit/gdkutils.cpp
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

#include "gdkutils.hpp"



namespace fwk {

	Glib::RefPtr<Gdk::Pixbuf> gdkpixbuf_scale_to_fit(const Glib::RefPtr<Gdk::Pixbuf> & pix,
													 int dim)
	{
		int height, width;
		int orig_h, orig_w;
		orig_h = pix->get_height();
		orig_w = pix->get_width();
		int orig_dim = std::max(orig_h, orig_w);
		double ratio = (double)dim / (double)orig_dim;
		width = (int)(ratio * orig_w);
		height = (int)(ratio * orig_h);
		return pix->scale_simple(width, height, 
								 Gdk::INTERP_BILINEAR);
	}

	Glib::RefPtr<Gdk::Pixbuf> gdkpixbuf_exif_rotate(const Glib::RefPtr<Gdk::Pixbuf> & tmp,
													int exif_orientation)
	{
		Glib::RefPtr<Gdk::Pixbuf> pixbuf;
		switch(exif_orientation) {
		case 0:
		case 1:
			pixbuf = tmp;
			break;
		case 2:
			pixbuf = tmp->flip(TRUE);
			break;
		case 3:
			pixbuf = tmp->rotate_simple(Gdk::PIXBUF_ROTATE_UPSIDEDOWN);
			break;
		case 4:
			pixbuf = tmp->rotate_simple(Gdk::PIXBUF_ROTATE_UPSIDEDOWN)->flip(TRUE);
			break;
		case 5:
			pixbuf = tmp->rotate_simple(Gdk::PIXBUF_ROTATE_CLOCKWISE)->flip(FALSE);
			break;
		case 6:
			pixbuf =  tmp->rotate_simple(Gdk::PIXBUF_ROTATE_CLOCKWISE);
			break;
		case 7:
			pixbuf =  tmp->rotate_simple(Gdk::PIXBUF_ROTATE_COUNTERCLOCKWISE)->flip(FALSE);
			break;		
		case 8:
			pixbuf =  tmp->rotate_simple(Gdk::PIXBUF_ROTATE_COUNTERCLOCKWISE);
			break;		
		default:
			break;
		}
		return pixbuf;
	}


  Gdk::Color rgbcolor_to_gdkcolor(const fwk::RgbColor & color)
  {
    Gdk::Color gdkcolor;
    gdkcolor.set_rgb(color[0], color[1], color[2]);
    return gdkcolor;
  }


  fwk::RgbColor gdkcolor_to_rgbcolor(const Gdk::Color & color)
  {
    fwk::RgbColor rgbcolor(color.get_red(), color.get_green(), color.get_blue());
    return rgbcolor;
  }


}
