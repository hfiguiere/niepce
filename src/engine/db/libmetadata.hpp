/*
 * niepce - eng/db/libmetadata.hpp
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

#pragma once

#include <memory>
#include <string>
#include <vector>

#include <boost/any.hpp>

#include "engine/db/librarytypes.hpp"
#include "engine/db/metadata.hpp"
#include "fwk/base/propertybag.hpp"
#include "fwk/utils/exempi.hpp"

namespace eng {

class LibMetadata;

void libmetadata_to_properties(const LibMetadata *meta,
                               const fwk::PropertySet &propset,
                               fwk::PropertyBag &props);

struct IndexToXmp {
    const char *ns;
    const char *property;
};
IndexToXmp property_index_to_xmp(fwk::PropertyIndex index);
}

extern "C" {
eng::library_id_t engine_libmetadata_get_id(const eng::LibMetadata *meta);
XmpPtr engine_libmetadata_get_xmp(const eng::LibMetadata *meta);
fwk::XmpMeta *engine_libmetadata_get_xmpmeta(const eng::LibMetadata *meta);
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
