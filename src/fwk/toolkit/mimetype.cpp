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

#include <libgnomevfs/gnome-vfs-mime-utils.h>

#include "config.h"

#include "mimetype.hpp"

namespace bfs = boost::filesystem;

namespace fwk {

MimeType::MimeType(const char * filename)
    : m_path(filename)
{
#if HAVE_GNOME_VFS_2_14
		m_type = gnome_vfs_get_mime_type_for_name(filename);
#else
		std::string f("file:///");
		f += filename;
		m_type = gnome_vfs_get_mime_type(f.c_str());
#endif
}

MimeType::MimeType(const boost::filesystem::path & filename)
    : m_path(filename)
{
#if HAVE_GNOME_VFS_2_14
		m_type = gnome_vfs_get_mime_type_for_name(filename.string().c_str());
#else
		std::string f("file:///");
		f += filename.string();
		m_type = gnome_vfs_get_mime_type(f.c_str());
#endif
}

bool MimeType::isDigicamRaw() const
{
		return (gnome_vfs_mime_type_get_equivalence(m_type.c_str(), 
                                                "image/x-dcraw") 
            != GNOME_VFS_MIME_UNRELATED);
}


bool MimeType::isImage() const
{
		return (gnome_vfs_mime_type_get_equivalence(m_type.c_str(), "image/*") 
            != GNOME_VFS_MIME_UNRELATED);
}
	

bool MimeType::isUnknown() const
{
		return (m_type == GNOME_VFS_MIME_TYPE_UNKNOWN);
}


bool MimeType::isXmp() const
{
    return m_path.extension() == ".xmp";
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
