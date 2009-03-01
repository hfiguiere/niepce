/*
 * niepce - framework/mimetype.h
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


#ifndef _FRAMEWORK_MIMETYPE_H_
#define _FRAMEWORK_MIMETYPE_H_

#include <string>
#include <boost/filesystem/path.hpp>

namespace framework {

	class MimeType
	{
	public:
		MimeType(const char *filename);
		MimeType(const boost::filesystem::path & filename);

		bool isDigicamRaw() const;
		bool isImage() const;
		bool isUnknown() const;
		bool isXmp() const;

		const std::string & string() const
			{ return m_type; }
	private:
		boost::filesystem::path m_path;
		std::string m_type;
	};

}


#endif
