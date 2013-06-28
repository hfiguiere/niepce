/*
 * niepce - utils/exempi.cpp
 *
 * Copyright (C) 2007-2013 Hubert Figuiere
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


#include <string.h>
#include <time.h>

#include <boost/lexical_cast.hpp>

#include <glib.h>
#include <giomm/file.h>

#include <exempi/xmp.h>
#include <exempi/xmpconsts.h>

#include "fwk/base/debug.hpp"
#include "fwk/base/date.hpp"
#include "exempi.hpp"
#include "pathutils.hpp"

namespace xmp {

const char * NIEPCE_XMP_NAMESPACE = "http://xmlns.figuiere.net/ns/niepce/1.0";
const char * NIEPCE_XMP_NS_PREFIX = "niepce";

const char * UFRAW_INTEROP_NAMESPACE = "http://xmlns.figuiere.net/ns/ufraw_interop/1.0";
const char * UFRAW_INTEROP_NS_PREFIX = "ufrint";

}

namespace fwk {

ExempiManager::ExempiManager(const ns_defs_t* namespaces)
{
    if(xmp_init()) {
        xmp_register_namespace(xmp::UFRAW_INTEROP_NAMESPACE,
                               xmp::UFRAW_INTEROP_NS_PREFIX, nullptr);
        xmp_register_namespace(xmp::NIEPCE_XMP_NAMESPACE,
                               xmp::NIEPCE_XMP_NS_PREFIX, nullptr);
        
        if(namespaces != nullptr) {
            for(auto iter = namespaces; iter->ns != nullptr; iter++)
            {
                // TODO check the return code
                xmp_register_namespace(iter->ns,
                                       iter->prefix, nullptr);
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
XmpMeta::XmpMeta(const std::string & file, bool sidecar_only)
    : m_xmp(nullptr),
      m_keyword_fetched(false)
{
    if(!sidecar_only) {
        DBG_OUT("trying to load the XMP from the file");
        xmp::ScopedPtr<XmpFilePtr> 
            xmpfile(xmp_files_open_new(file.c_str(), XMP_OPEN_READ));
        if(xmpfile != nullptr) {
            m_xmp = xmp_files_get_new_xmp(xmpfile);
            if(xmpfile == nullptr) {
                ERR_OUT("xmpfile is nullptr");
            }
        }
    }
		
    if(m_xmp == nullptr) {
        gsize len = 0;
        char * buffer = nullptr;
        std::string sidecar = fwk::path_replace_extension(file, ".xmp");
			
        try {
            DBG_OUT("creating xmpmeta from %s", sidecar.c_str());
            Glib::RefPtr<Gio::File> f = Gio::File::create_for_path(sidecar);
            std::string etag_out;
            f->load_contents(buffer, len, etag_out);
        }
        catch(const Glib::Exception & e) {
            ERR_OUT("loading XMP failed: %s", e.what().c_str());
        }
        if(buffer) {
            m_xmp = xmp_new_empty();
            if(!xmp_parse(m_xmp, buffer, len)) {
                xmp_free(m_xmp);
                m_xmp = nullptr;
            }
            g_free(buffer);
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
   
    if(!xmp_get_property_int32(m_xmp, NS_TIFF, "Orientation", &_orientation, nullptr)) {
        ERR_OUT("get \"Orientation\" property failed: %d", xmp_get_error());
    }
    return _orientation;
}


std::string XmpMeta::label() const
{
    std::string _label;
    xmp::ScopedPtr<XmpStringPtr> value(xmp_string_new());
    if(xmp_get_property(m_xmp, NS_XAP, "Label", value, nullptr)) {
        _label = xmp_string_cstr(value);
    }
    return _label;
}


int32_t XmpMeta::rating() const
{
    int32_t _rating = -1;
    if(!xmp_get_property_int32(m_xmp, NS_XAP, "Rating", &_rating, nullptr)) {
        ERR_OUT("get \"Rating\" property failed: %d", xmp_get_error());
    }
    return _rating;
}

int32_t XmpMeta::flag() const
{
    int32_t _flag = 0;
    if(!xmp_get_property_int32(m_xmp, xmp::NIEPCE_XMP_NAMESPACE, "Flag", 
                               &_flag, nullptr)) {
        ERR_OUT("get \"Flag\" property failed: %d", xmp_get_error());
    }
    return _flag;
}

fwk::Date XmpMeta::creation_date() const
{
    XmpDateTime value;
    if(xmp_get_property_date(m_xmp, NS_EXIF, "DateTimeOriginal", 
                             &value, nullptr)) {
        return fwk::Date(value);
    }
    else {
        ERR_OUT("get \"DateTimeOriginal\" property failed: %d", xmp_get_error());
    }
    return Date(0);
}

std::string XmpMeta::creation_date_str() const
{
    std::string s;
    xmp::ScopedPtr<XmpStringPtr> value(xmp_string_new());
    if(xmp_get_property(m_xmp, NS_EXIF, "DateTimeOriginal", 
                        value, nullptr)) {
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
        while(xmp_iterator_next(iter, nullptr, nullptr, value, nullptr)) {
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
