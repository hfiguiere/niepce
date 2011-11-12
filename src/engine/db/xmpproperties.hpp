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




#ifndef __ENG_XMPPROPERTIES_HPP__
#define __ENG_XMPPROPERTIES_HPP__

#include <map>
#include "fwk/base/propertybag.hpp"

namespace fwk {
class XmpMeta;
}

namespace eng {

typedef std::map<fwk::PropertyIndex, std::pair<const char*, const char *> > PropsToXmpMap;

/** get the mapping of properties to XMP */
const PropsToXmpMap & props_to_xmp_map();

bool property_index_to_xmp(fwk::PropertyIndex index, const char * & ns, const char * & property);

/** convert XMP to a set of properties 
 * @param meta the XmpMeta source
 * @param propset the property set requested
 * @param props the output properties
 */
void convert_xmp_to_properties(const fwk::XmpMeta * meta, 
			       const fwk::PropertySet & propset, fwk::PropertyBag & props);

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
#endif
