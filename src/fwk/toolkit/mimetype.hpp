/*
 * niepce - fwk/mimetype.h
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


#ifndef _FWK_MIMETYPE_H_
#define _FWK_MIMETYPE_H_

#include <string>

#include <giomm/file.h>
#include <giomm/fileinfo.h>

namespace fwk {

	class MimeType
	{
	public:
		MimeType(const std::string & filename);
    MimeType(const Glib::RefPtr<Gio::File> & file);

		bool isDigicamRaw() const;
		bool isImage() const;
		bool isUnknown() const;
		bool isXmp() const;

		const std::string & string() const
			{ return m_type; }
	private:
    Glib::RefPtr<Gio::FileInfo> m_fileinfo;
		std::string m_type;
	};

}


#endif
