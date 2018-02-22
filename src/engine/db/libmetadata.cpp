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
        return {nullptr, nullptr};
    }
    if (iter->second.first == NULL || iter->second.second == NULL) {
        // no XMP equivalent
        return {nullptr, nullptr};
    }
    return {iter->second.first, iter->second.second};
}

fwk::PropertyBagPtr libmetadata_to_properties(const LibMetadata* meta,
                                              const fwk::PropertySet& propset)
{
    return fwk::property_bag_wrap(ffi::engine_libmetadata_to_properties(meta, &propset));
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
