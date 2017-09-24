/*
 * niepce - eng/db/properties.hpp
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


#pragma once

#include <map>
#include <typeinfo>

#include "fwk/base/propertybag.hpp"
#include "engine/db/metadata.hpp"

#include "engine/db/properties-enum.hpp"

namespace eng {


struct property_desc_t {
    fwk::PropertyIndex prop;
    const char * name;
    const std::type_info & type;
};

typedef std::map<fwk::PropertyIndex, const property_desc_t*> PropDescMap;

/** get the properties description */
const PropDescMap & property_desc_map();
/** return true of the property is of the type ti */
bool check_property_type(fwk::PropertyIndex idx, const std::type_info & ti);


/** internal property name */
const char * _propertyName(fwk::PropertyIndex idx);

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
