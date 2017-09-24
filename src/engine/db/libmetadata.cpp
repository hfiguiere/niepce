/*
 * niepce - db/libmetadata.cpp
 *
 * Copyright (C) 2008-2017 Hubert Figuière
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

#include <algorithm>
#include <map>

#include <exempi/xmpconsts.h>
#include <exempi/xmperrors.h>

#include "fwk/base/date.hpp"
#include "fwk/base/debug.hpp"
#include "fwk/base/rust.hpp"
#include "fwk/utils/exempi.hpp"
#include "fwk/utils/stringutils.hpp"
#include "libmetadata.hpp"
#include "properties.hpp"

namespace eng {

typedef std::map<fwk::PropertyIndex, std::pair<const char *, const char *>>
    PropsToXmpMap;

/** get the mapping of properties to XMP */
const PropsToXmpMap &props_to_xmp_map();

const PropsToXmpMap &props_to_xmp_map()
{
    static PropsToXmpMap s_props_map;
    if (s_props_map.empty()) {

#define DEFINE_PROPERTY(a, b, c, d, e)                                         \
    s_props_map.insert(std::make_pair(b, std::make_pair(c, d)));
#include "engine/db/properties-def.hpp"
#undef DEFINE_PROPERTY
    }
    return s_props_map;
}

IndexToXmp property_index_to_xmp(fwk::PropertyIndex index)
{
    const PropsToXmpMap &propmap = props_to_xmp_map();
    PropsToXmpMap::const_iterator iter = propmap.find(index);
    if (iter == propmap.end()) {
        // not found
        return {ns : nullptr, property : nullptr};
    }
    if (iter->second.first == NULL || iter->second.second == NULL) {
        // no XMP equivalent
        return {ns : nullptr, property : nullptr};
    }
    return {ns : iter->second.first, property : iter->second.second};
}

bool get_meta_data(const LibMetadata *meta, fwk::PropertyIndex p,
                   fwk::PropertyValuePtr &value)
{
    const PropsToXmpMap &propmap = props_to_xmp_map();
    PropsToXmpMap::const_iterator iter = propmap.find(p);
    if (iter == propmap.end()) {
        // not found
        return false;
    }
    if (iter->second.first == NULL || iter->second.second == NULL) {
        // no XMP equivalent
        return false;
    }
    xmp::ScopedPtr<XmpStringPtr> xmp_value(xmp_string_new());
    uint32_t prop_bits = 0;
    const char *ns = iter->second.first;
    const char *xmp_prop = iter->second.second;
    bool found = xmp_get_property(engine_libmetadata_get_xmp(meta), ns,
                                  xmp_prop, xmp_value, &prop_bits);
    if (found && XMP_IS_ARRAY_ALTTEXT(prop_bits)) {
        found = xmp_get_localized_text(engine_libmetadata_get_xmp(meta), ns,
                                       xmp_prop, "", "x-default", nullptr,
                                       xmp_value, nullptr);
    }
    if (found) {
        const char *v = NULL;
        v = xmp_string_cstr(xmp_value);
        if (v) {
            value = fwk::property_value_new(v);
            return true;
        }
    }
    // not found in XMP
    return false;
}

void libmetadata_to_properties(const LibMetadata *meta,
                               const fwk::PropertySet &propset,
                               fwk::PropertyBagPtr &props)
{
    if (!props) {
        props = fwk::property_bag_new();
    }
    std::for_each(
        propset.begin(), propset.end(),
        [&props, meta](fwk::PropertySet::key_type prop_id) {
            auto xmpmeta = engine_libmetadata_get_xmpmeta(meta);
            switch (prop_id) {
            case NpXmpRatingProp:
                fwk::set_value_for_property(
                    *props, prop_id,
                    *fwk::property_value_new(fwk_xmp_meta_get_rating(xmpmeta)));
                break;
            case NpXmpLabelProp: {
                char *str = fwk_xmp_meta_get_label(xmpmeta);
                if (str) {
                    fwk::set_value_for_property(*props, prop_id,
                                                *fwk::property_value_new(str));
                    rust_cstring_delete(str);
                } else {
                    fwk::set_value_for_property(*props, prop_id,
                                                *fwk::property_value_new(""));
                }
                break;
            }
            case NpTiffOrientationProp:
                fwk::set_value_for_property(
                    *props, prop_id,
                    *fwk::property_value_new(fwk_xmp_meta_get_orientation(xmpmeta)));
                break;
            case NpExifDateTimeOriginalProp: {
                fwk::DatePtr date =
                    fwk::date_wrap(fwk_xmp_meta_get_creation_date(xmpmeta));
                if (date) {
                    fwk::set_value_for_property(*props, prop_id, *fwk::property_value_new(date));
                }
                break;
            }
            case NpIptcKeywordsProp: {
                xmp::ScopedPtr<XmpIteratorPtr> iter(
                    xmp_iterator_new(engine_libmetadata_get_xmp(meta), NS_DC,
                                     "subject", XMP_ITER_JUSTLEAFNODES));
                std::vector<std::string> vec;
                xmp::ScopedPtr<XmpStringPtr> value(xmp_string_new());
                while (xmp_iterator_next(iter, NULL, NULL, value, NULL)) {
                    vec.push_back(xmp_string_cstr(value));
                }
                fwk::PropertyValuePtr v = fwk::property_value_new(vec);
                // DBG_ASSERT(check_property_type(prop_id, v.type()), "wrong
                // type");
                fwk::set_value_for_property(*props, prop_id, *v);
                break;
            }
            default: {
                fwk::PropertyValuePtr propval;
                if (get_meta_data(meta, prop_id, propval)) {
                    // DBG_ASSERT(check_property_type(prop_id, propval.type()),
                    // "wrong type");
                    fwk::set_value_for_property(*props, prop_id, *propval);
                } else {
                    DBG_OUT("missing prop %u", prop_id);
                }
                break;
            }
            }
        });
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
