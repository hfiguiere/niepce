/*
 * niepce - eng/db/xmpproperties.hpp
 *
 * Copyright (C) 2011 Hubert Figuiere
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

#include <exempi/xmpconsts.h>

#include "fwk/base/debug.hpp"
#include "fwk/base/propertybag.hpp"
#include "fwk/utils/stringutils.hpp"
#include "fwk/utils/exempi.hpp"

#include "properties.hpp"
#include "xmpproperties.hpp"


namespace eng {

const PropsToXmpMap & props_to_xmp_map()
{
    static PropsToXmpMap s_props_map;
    if(s_props_map.empty()) {

#define DEFINE_PROPERTY(a,b,c,d) \
        s_props_map.insert(std::make_pair(b, std::make_pair(c,d)));
#include "engine/db/properties-def.hpp"
#undef DEFINE_PROPERTY

    }
    return s_props_map;
}

bool get_prop_from_xmp(const fwk::XmpMeta * meta, fwk::PropertyIndex p, 
                       fwk::PropertyValue & value)
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
    if(xmp_get_property(meta->xmp(), iter->second.first,
                        iter->second.second, xmp_value, NULL)) {
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

void convert_xmp_to_properties(const fwk::XmpMeta * meta, const fwk::PropertySet & propset,
                               fwk::PropertyBag & properties)
{
    if(meta == NULL) {
        ERR_OUT("invalid passing NULL meta");
        return;
    }
    fwk::PropertySet::const_iterator iter;
    xmp::ScopedPtr<XmpStringPtr> value(xmp_string_new());
    fwk::PropertyValue propval;
    for(iter = propset.begin(); iter != propset.end(); ++iter) {
        switch(*iter) {
        case NpXmpRatingProp:
            properties.set_value_for_property(NpXmpRatingProp,
                                              fwk::PropertyValue(meta->rating()));
            break;
        case NpXmpLabelProp:
            properties.set_value_for_property(NpXmpLabelProp,
                                              fwk::PropertyValue(meta->label()));
            break;
        case NpTiffOrientationProp:
            properties.set_value_for_property(NpTiffOrientationProp, 
                                              fwk::PropertyValue(meta->orientation()));
            break;
        case NpIptcKeywordsProp:
        {
            xmp::ScopedPtr<XmpIteratorPtr> 
                iter(xmp_iterator_new(meta->xmp(), NS_DC,
                                      "subject", XMP_ITER_JUSTLEAFNODES));
            std::vector<std::string> vec;
            while(xmp_iterator_next(iter, NULL, NULL, value, NULL)) {
                vec.push_back(xmp_string_cstr(value));
            }
            std::string v = fwk::join(vec, ", ");
            properties.set_value_for_property(NpIptcKeywordsProp, 
                                              fwk::PropertyValue(v));
            break;
        }
        default:
            if(get_prop_from_xmp(meta, *iter, propval)) {
                properties.set_value_for_property(*iter, propval);                
            }
            else {
                DBG_OUT("unknown prop %u", *iter);
            }
            break;
        }
    }
}

#if 0

if(current->type == META_DT_STRING_ARRAY) {
    xmp::ScopedPtr<XmpIteratorPtr> 
        iter(xmp_iterator_new(xmp->xmp(), current->ns,
                              current->property, XMP_ITER_JUSTLEAFNODES));
    std::vector<std::string> vec;
    while(xmp_iterator_next(iter, NULL, NULL, value, NULL)) {
        vec.push_back(xmp_string_cstr(value));
    }
    std::string v = fwk::join(vec, ", ");
    add_data(current, v.c_str());
}

#endif

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
