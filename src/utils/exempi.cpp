/*
 * niepce - utils/exempi.cpp
 *
 * Copyright (C) 2007 Hubert Figuiere
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
#include <string.h>
#include <time.h>

#include <boost/filesystem/convenience.hpp>
#include <boost/lexical_cast.hpp>

#include <exempi/xmp.h>
#include <exempi/xmpconsts.h>

#include "debug.h"
#include "exempi.h"

namespace bfs = boost::filesystem;

namespace utils {

	ExempiManager::ExempiManager(const ns_defs_t* namespaces)
	{
		if(xmp_init() && (namespaces != NULL)) {
			const ns_defs_t* iter = namespaces;
			for( ; iter->ns != NULL; iter++)	
			{
				// TODO check the return code
				xmp_register_namespace(iter->ns,
									   iter->prefix, NULL);
			}
		}
	}


	ExempiManager::~ExempiManager()
	{
		xmp_terminate();
	}



	XmpMeta::XmpMeta(const bfs::path & file)
	{
        size_t len;
        char * buffer;
		bfs::path sidecar = file.branch_path()
			/ (basename(file) + ".xmp");
		m_xmp = xmp_new_empty();

		FILE * f = fopen(sidecar.string().c_str(), "rb");

        if (f != NULL) {
			fseek(f, 0, SEEK_END);
			len = ftell(f);
			fseek(f, 0, SEEK_SET);
			
			buffer = (char*)malloc(len + 1);
			/*size_t rlen =*/ fread(buffer, 1, len, f);
			xmp_parse(m_xmp, buffer, len);
			free(buffer);
        }
	}

	XmpMeta::~XmpMeta()
	{
		if(m_xmp) {
			xmp_free(m_xmp);
		}
	}

	std::string XmpMeta::serialize_inline() const
	{
		std::string buf;
		XmpStringPtr output = xmp_string_new();
		if(xmp_serialize_and_format(m_xmp, output, 
									XMP_SERIAL_OMITPACKETWRAPPER | XMP_SERIAL_OMITALLFORMATTING, 
									0, "", "", 0)) {
			buf = xmp_string_cstr(output);
		}
		return buf;
	}

	std::string XmpMeta::serialize() const
	{
		std::string buf;
		XmpStringPtr output = xmp_string_new();
		if(xmp_serialize_and_format(m_xmp, output, 
									XMP_SERIAL_OMITPACKETWRAPPER, 
									0, "\n", " ", 0)) {
			buf = xmp_string_cstr(output);
		}
		return buf;
	}


	int32_t XmpMeta::orientation() const
	{
		int32_t _orientation = 0;
		XmpStringPtr value = xmp_string_new();
		if(xmp_get_property(m_xmp, NS_TIFF, "Orientation", value, NULL)) {
			try {
				_orientation = boost::lexical_cast<int32_t>(xmp_string_cstr(value));
			}
			catch(const boost::bad_lexical_cast &)
			{
			}
		}
		xmp_string_free(value);
		return _orientation;
	}


	std::string XmpMeta::label() const
	{
		std::string _label;
		XmpStringPtr value = xmp_string_new();
		if(xmp_get_property(m_xmp, NS_XAP, "Label", value, NULL)) {
			_label = xmp_string_cstr(value);
		}
		xmp_string_free(value);
		return _label;
	}


	int32_t XmpMeta::rating() const
	{
		int32_t _rating = 0;
		XmpStringPtr value = xmp_string_new();
		if(xmp_get_property(m_xmp, NS_XAP, "Rating", value, NULL)) {
			try {
				_rating = boost::lexical_cast<int32_t>(xmp_string_cstr(value));
			}
			catch(const boost::bad_lexical_cast &)
			{
			}
		}
		xmp_string_free(value);
		return _rating;
	}


	time_t  XmpMeta::creation_date() const
	{
		time_t date = 0;
		XmpDateTime value;
		if(xmp_get_property_date(m_xmp, NS_EXIF, "DateTimeOriginal", 
								 &value, NULL)) {
			struct tm dt;
			dt.tm_sec = value.second;
			dt.tm_min = value.minute;
			dt.tm_hour = value.hour;
			dt.tm_mday = value.day;
			dt.tm_mon = value.month;
			dt.tm_year = value.year - 1900;
			dt.tm_isdst = -1;
			// this field is supposed to be a glibc extension. oh joy.
			dt.tm_gmtoff = value.tzSign * ((value.tzHour * 3600) +
										   (value.tzMinute * 60));
			date = mktime(&dt);
			DBG_ASSERT(date != -1, "date is -1");
		}
		return date;
	}
	
}
