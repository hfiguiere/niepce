/*
 * niepce - framework/imageloader.cpp
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

#include <stdio.h>

#include <gdkmm/pixbufloader.h>

#include "imageloader.h"
#include "fwk/toolkit/mimetype.h"

namespace framework {

ImageLoader::ImageLoader(const boost::filesystem::path & file)
	: m_file(file)
{
}

Glib::RefPtr<Gdk::Pixbuf> ImageLoader::get_pixbuf()
{
	// TODO split get_pixbuf and the real load.
	framework::MimeType mime_type(m_file);
	
	Glib::RefPtr<Gdk::PixbufLoader> loader =  
		Gdk::PixbufLoader::create(mime_type.string(), true);

	// TODO this code is ugly.
	FILE * f = fopen(m_file.string().c_str(), "rb");
    if(f) {
        size_t byte_read;
        guint8 buffer[128*1024];
        do {
            byte_read = fread((void*)buffer, 1, 128*1024, f);
            if(byte_read) {
                loader->write(buffer, byte_read);
            }
        } while(byte_read);
	}
    loader->close();

	return loader->get_pixbuf();
}


}

