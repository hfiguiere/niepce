/*
 * niepce - db/libmetadata.cpp
 *
 * Copyright (C) 2008-2013 Hubert Figuiere
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
#include <exempi/xmperrors.h>

#include "fwk/base/debug.hpp"
#include "fwk/utils/stringutils.hpp"
#include "libmetadata.hpp"
#include "properties.hpp"

namespace eng {

typedef std::map<fwk::PropertyIndex, std::pair<const char*, const char *> > PropsToXmpMap;

/** get the mapping of properties to XMP */
const PropsToXmpMap & props_to_xmp_map();


const PropsToXmpMap & props_to_xmp_map()
{
    static PropsToXmpMap s_props_map;
    if(s_props_map.empty()) {

#define DEFINE_PROPERTY(a,b,c,d,e)                                       \
        s_props_map.insert(std::make_pair(b, std::make_pair(c,d)));
#include "engine/db/properties-def.hpp"
#undef DEFINE_PROPERTY

    }
    return s_props_map;
}

bool
LibMetadata::property_index_to_xmp(fwk::PropertyIndex index, 
                                   const char * & ns, const char * & property)
{
    const PropsToXmpMap & propmap = props_to_xmp_map();
    PropsToXmpMap::const_iterator iter = propmap.find(index);
    if(iter == propmap.end()) {
        // not found
        return false;
    }
    if(iter->second.first == NULL || iter->second.second == NULL) {
        // no XMP equivalent
        return false;
    }
    ns = iter->second.first;
    property = iter->second.second;
    return true;
}



LibMetadata::LibMetadata(library_id_t _id)
	: XmpMeta(),
      m_id(_id)
{
}


bool LibMetadata::setMetaData(fwk::PropertyIndex meta, 
                              const fwk::PropertyValue & value)
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
            std::string val = boost::get<std::string>(value);
            if (val.empty()) {
                result = xmp_delete_property(xmp(), ns, property);
            }
            else {
                result = xmp_set_property(xmp(), ns, property, val.c_str(), 0);
                // FIXME we should know in advance it is localized.
                if(!result && (xmp_get_error() == XMPErr_BadXPath)) {
                    result = xmp_set_localized_text(xmp(), ns, property,
                                                    "", "x-default",
                                                    val.c_str(), 0);
                }
            }
        }
        else if(value.type() == typeid(fwk::StringArray)) {
            fwk::StringArray v = boost::get<fwk::StringArray>(value);
            // TODO see if we can get that without deleting the whole property
            result = xmp_delete_property(xmp(), ns, property);
            std::for_each(v.begin(), v.end(),
                          [this, ns, property] (const std::string & val) {
                              /*result =*/
                              xmp_append_array_item(this->xmp(),
                                                    ns, property,
                                                    XMP_PROP_VALUE_IS_ARRAY,
                                                    val.c_str(), 0);
                          });
        }
        else if(value.type() == typeid(fwk::Date)) {
            fwk::Date d = boost::get<fwk::Date>(value);

            result = xmp_set_property_date(xmp(), ns, property,
                                           &d.xmp_date(), 0);

        }
        if(!result) {
            ERR_OUT("error setting property %s:%s %d", ns, property, 
                    xmp_get_error());
        }
    }
    else {
        ERR_OUT("unknown property");
    }
    return result;
}


bool LibMetadata::getMetaData(fwk::PropertyIndex p, 
                              fwk::PropertyValue & value) const
{
    const PropsToXmpMap & propmap = props_to_xmp_map();
    PropsToXmpMap::const_iterator iter = propmap.find(p);
    if(iter == propmap.end()) {
        // not found
        return false;
    }
    if(iter->second.first == NULL || iter->second.second == NULL) {
        // no XMP equivalent
        return false;
    }
    xmp::ScopedPtr<XmpStringPtr> xmp_value(xmp_string_new());
    uint32_t prop_bits = 0;
    const char * ns = iter->second.first;
    const char * xmp_prop = iter->second.second;
    bool found = xmp_get_property(xmp(), ns, xmp_prop, xmp_value, &prop_bits);
    if(found && XMP_IS_ARRAY_ALTTEXT(prop_bits)) {
        found = xmp_get_localized_text(xmp(), ns, xmp_prop, "", "x-default", 
                                       NULL, xmp_value, NULL);
    }
    if(found) {
        const char * v = NULL;
        v = xmp_string_cstr(xmp_value);
        if(v) {
            value = fwk::PropertyValue(v);
            return true;
        }
    }
    // not found in XMP
    return false;
}

void LibMetadata::to_properties(const fwk::PropertySet & propset,
                                fwk::PropertyBag & props)
{
    std::for_each(propset.begin(), propset.end(),
                  [&props, this] (fwk::PropertySet::key_type prop_id) {
                      switch(prop_id) {
                      case NpXmpRatingProp:
                          props.set_value_for_property(prop_id,
                                                       fwk::PropertyValue(rating()));
                          break;
                      case NpXmpLabelProp:
                          props.set_value_for_property(prop_id,
                                                       fwk::PropertyValue(label()));
                          break;
                      case NpTiffOrientationProp:
                          props.set_value_for_property(prop_id,
                                                       fwk::PropertyValue(orientation()));
                          break;
                      case NpExifDateTimeOriginalProp:
                          props.set_value_for_property(prop_id,
                                                       fwk::PropertyValue(creation_date()));
                          break;
                      case NpIptcKeywordsProp:
                      {
                          xmp::ScopedPtr<XmpIteratorPtr>
                              iter(xmp_iterator_new(xmp(), NS_DC,
                                                    "subject", XMP_ITER_JUSTLEAFNODES));
                          fwk::StringArray vec;
                          xmp::ScopedPtr<XmpStringPtr> value(xmp_string_new());
                          while(xmp_iterator_next(iter, NULL, NULL, value, NULL)) {
                              vec.push_back(xmp_string_cstr(value));
                          }
                          props.set_value_for_property(prop_id,
                                                       fwk::PropertyValue(vec));
                          break;
                      }
                      default:
                      {
                          fwk::PropertyValue propval;
                          if(getMetaData(prop_id, propval)) {
                              props.set_value_for_property(prop_id, propval);
                          }
                          else {
                              DBG_OUT("unknown prop %u", prop_id);
                          }
                          break;
                      }
                      }
                  }
        );
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
