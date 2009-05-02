/*
 * niepce - fwk/mimetype.cpp
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

#include <string>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <giomm/contenttype.h>

#include "config.h"

#include "mimetype.hpp"

namespace fwk {

MimeType::MimeType(const std::string & filename)
{
    Glib::RefPtr<Gio::File> file = Gio::File::create_for_path(filename);
    m_fileinfo = file->query_info();
		m_type = m_fileinfo->get_content_type();
}

MimeType::MimeType(const Glib::RefPtr<Gio::File> & file)
{
    m_fileinfo = file->query_info();
		m_type = m_fileinfo->get_content_type();
}

bool MimeType::isDigicamRaw() const
{
		return Gio::content_type_is_a(m_type, "image/x-dcraw");
}


bool MimeType::isImage() const
{
		return Gio::content_type_is_a(m_type, "image/*");
}
	

bool MimeType::isUnknown() const
{
		return Gio::content_type_is_unknown(m_type);
}


bool MimeType::isXmp() const
{
    boost::filesystem::path path = m_fileinfo->get_name();
    
#if BOOST_VERSION >= 103600
    return path.extension() == ".xmp";
#else
    return extension(path) == ".xmp";
#endif
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
