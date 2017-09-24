/*
 * niepce - fwk/base/propertybag.cpp
 *
 * Copyright (C) 2011-2013 Hubert Figuiere
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

#include <stdint.h>
#include <string>
#include <vector>
#include <set>
#include <memory>

#include "fwk/base/date.hpp"
#include "fwk/base/option.hpp"

#include "rust_bindings.hpp"

namespace fwk {

typedef uint32_t PropertyIndex;

typedef std::set<PropertyIndex> PropertySet;

#if !RUST_BINDGEN
typedef std::shared_ptr<PropertyValue> PropertyValuePtr;

PropertyValuePtr property_value_new(const std::string&);
PropertyValuePtr property_value_new(int);
PropertyValuePtr property_value_new(const std::vector<std::string>&);
PropertyValuePtr property_value_new(const DatePtr&);

std::string property_value_get_string(const PropertyValue &value);
std::vector<std::string> property_value_get_string_array(const PropertyValue &value);
#endif

#if RUST_BINDGEN
class PropertyBag;
#endif

/** a property bag
 * It is important that the values for PropertyIndex be properly name spaced
 * by the caller.
 */
typedef std::shared_ptr<PropertyBag> PropertyBagPtr;

#if !RUST_BINDGEN
PropertyBagPtr property_bag_new();

PropertyValuePtr property_bag_value(const PropertyBagPtr& bag, PropertyIndex key);

std::string property_value_get_string(const PropertyValue& v);

/** return true if a property was removed prior to insertion */
bool set_value_for_property(PropertyBag&, PropertyIndex idx, const PropertyValue & value);
/** return property or an empty option */
fwk::Option<PropertyValuePtr> get_value_for_property(const PropertyBag&, PropertyIndex idx);
/** return true if property exist */
bool has_value_for_property(const PropertyBag&, PropertyIndex idx);
/** return true if the property was removed */
bool remove_value_for_property(PropertyBag&, PropertyIndex idx);
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
