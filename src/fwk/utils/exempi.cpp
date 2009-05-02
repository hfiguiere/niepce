/*
 * niepce - utils/exempi.cpp
 *
 * Copyright (C) 2007-2008 Hubert Figuiere
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

#include "debug.hpp"
#include "exempi.hpp"

namespace bfs = boost::filesystem;

namespace xmp {

const char * UFRAW_INTEROP_NAMESPACE = "http://xmlns.figuiere.net/ns/ufraw_interop/1.0";
const char * UFRAW_INTEROP_NS_PREFIX = "ufrint";

}

namespace utils {

ExempiManager::ExempiManager(const ns_defs_t* namespaces)
{
    if(xmp_init()) {
        xmp_register_namespace(xmp::UFRAW_INTEROP_NAMESPACE,
                               xmp::UFRAW_INTEROP_NS_PREFIX, NULL);
        
        if(namespaces != NULL) {
            const ns_defs_t* iter = namespaces;
            for( ; iter->ns != NULL; iter++)	
            {
                // TODO check the return code
                xmp_register_namespace(iter->ns,
                                       iter->prefix, NULL);
            }
        }
    }
}


ExempiManager::~ExempiManager()
{
    xmp_terminate();
}


XmpMeta::XmpMeta()
    : m_xmp(),
      m_keyword_fetched(false)		  
{
    m_xmp = xmp_new_empty();
}


/** @param file the path to the file to open 
 * @param sidecar_only we only want the sidecar.
 * It will locate the XMP sidecar for the file.
 */
XmpMeta::XmpMeta(const bfs::path & file, bool sidecar_only)
    : m_xmp(NULL),
      m_keyword_fetched(false)
{
    if(!sidecar_only) {
        DBG_OUT("trying to load the XMP from the file");
        xmp::ScopedPtr<XmpFilePtr> 
            xmpfile(xmp_files_open_new(file.string().c_str(), XMP_OPEN_READ));
        if(xmpfile != NULL) {
            m_xmp = xmp_files_get_new_xmp(xmpfile);
            if(xmpfile == NULL) {
                ERR_OUT("xmpfile is NULL");
            }
        }
    }
		
    if(m_xmp == NULL) {
        size_t len;
        char * buffer;
        bfs::path sidecar = file.branch_path()
            / (basename(file) + ".xmp");
			
        DBG_OUT("creating xmpmeta from %s", sidecar.string().c_str());
        FILE * f = fopen(sidecar.string().c_str(), "rb");
			
        if (f != NULL) {
            fseek(f, 0, SEEK_END);
            len = ftell(f);
            fseek(f, 0, SEEK_SET);
				
            buffer = (char*)malloc(len + 1);
            /*size_t rlen =*/ fread(buffer, 1, len, f);
            m_xmp = xmp_new_empty();
            if(!xmp_parse(m_xmp, buffer, len)) {
                xmp_free(m_xmp);
                m_xmp = NULL;
            }
            free(buffer);
        }
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
    xmp::ScopedPtr<XmpStringPtr> output(xmp_string_new());
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
    xmp::ScopedPtr<XmpStringPtr> output(xmp_string_new());
    if(xmp_serialize_and_format(m_xmp, output, 
                                XMP_SERIAL_OMITPACKETWRAPPER, 
                                0, "\n", " ", 0)) {
        buf = xmp_string_cstr(output);
    }
    return buf;
}

void XmpMeta::unserialize(const char * buffer)
{
    if(!buffer) 
        return;
    xmp_parse(m_xmp, buffer, strlen(buffer));
}


int32_t XmpMeta::orientation() const
{
    int32_t _orientation = 0;
   
    if(!xmp_get_property_int32(m_xmp, NS_TIFF, "Orientation", &_orientation, NULL)) {
        ERR_OUT("get \"Orientation\" property failed: %d", xmp_get_error());
    }
    return _orientation;
}


std::string XmpMeta::label() const
{
    std::string _label;
    xmp::ScopedPtr<XmpStringPtr> value(xmp_string_new());
    if(xmp_get_property(m_xmp, NS_XAP, "Label", value, NULL)) {
        _label = xmp_string_cstr(value);
    }
    return _label;
}


int32_t XmpMeta::rating() const
{
    int32_t _rating = -1;
    if(!xmp_get_property_int32(m_xmp, NS_XAP, "Rating", &_rating, NULL)) {
        ERR_OUT("get \"Rating\" property failed: %d", xmp_get_error());
    }
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
    else {
        ERR_OUT("get \"DateTimeOriginal\" property failed: %d", xmp_get_error());
    }
    return date;
}

std::string XmpMeta::creation_date_str() const
{
    std::string s;
    xmp::ScopedPtr<XmpStringPtr> value(xmp_string_new());
    if(xmp_get_property(m_xmp, NS_EXIF, "DateTimeOriginal", 
                        value, NULL)) {
        s = xmp_string_cstr(value);
    }
    return s;
}


const std::vector< std::string > & XmpMeta::keywords() const
{
    if(!m_keyword_fetched) {
        xmp::ScopedPtr<XmpIteratorPtr> iter(xmp_iterator_new(m_xmp, NS_DC, "subject", 
                                                             XMP_ITER_JUSTLEAFNODES));
        xmp::ScopedPtr<XmpStringPtr> value(xmp_string_new());
        while(xmp_iterator_next(iter, NULL, NULL, value, NULL)) {
            m_keywords.push_back(xmp_string_cstr(value));
        }
        m_keyword_fetched = true;
    }
    return m_keywords;
}

}

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:80
  End:
*/
