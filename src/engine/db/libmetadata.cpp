/*
 * niepce - db/libmetadata.cpp
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

#include <string.h>
#include <time.h>
#include <exempi/xmpconsts.h>

#include "fwk/base/debug.hpp"
#include "libmetadata.hpp"
#include "xmpproperties.hpp"

namespace eng {


LibMetadata::LibMetadata(library_id_t _id)
	: XmpMeta(),
      m_id(_id)
{
}


bool LibMetadata::setMetaData(fwk::PropertyIndex meta, const fwk::PropertyValue & value)
{
    const char * ns = NULL;
    const char * property = NULL;
    bool result = false;

    result = property_index_to_xmp(meta, ns, property);
    if(result) {
        
        if(value.type() == typeid(int)) {
            result = xmp_set_property_int32(xmp(), ns, property, 
                                            boost::get<int>(value), 0);
        }
        else if(value.type() == typeid(std::string)) {
            result = xmp_set_property(xmp(), ns, property, 
                                      boost::get<std::string>(value).c_str(), 0);
        }
    }
    else {
        ERR_OUT("unknown property");
    }
    return result;
}

bool LibMetadata::touch()
{
    bool result = false;
    XmpDateTime date;
    struct tm dt, *dt2;
    time_t currenttime = time(NULL);
    dt2 = gmtime_r(&currenttime, &dt);
    if(dt2 == &dt) {
        memset(&date, 0, sizeof date);
        date.second = dt.tm_sec;
        date.minute = dt.tm_min;
        date.hour = dt.tm_hour;
        date.day = dt.tm_mday;
        date.month = dt.tm_mon;
        date.year = dt.tm_year + 1900;

        result = xmp_set_property_date(xmp(), NS_XAP, "MetadataDate",
                                       &date, 0);
    }
    return result;
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
