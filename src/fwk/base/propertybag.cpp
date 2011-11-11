/*
 * niepce - fwk/base/propertybag.cpp
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



#include "propertybag.hpp"

namespace fwk {

bool PropertyBag::set_value_for_property(PropertyIndex idx, const PropertyValue & value)
{
    bool removed = (m_bag.erase(idx) == 1);
    m_bag.insert(std::make_pair(idx, value));
    return removed;
}

/** return true if a property is found */
bool PropertyBag::get_value_for_property(PropertyIndex idx, PropertyValue & value) const
{
    _Map::const_iterator iter = m_bag.find(idx);
    if(iter == m_bag.end()) {
        return false;
    }
    value = iter->second;
    return true;
}


bool PropertyBag::has_value_for_property(PropertyIndex idx) const
{
    return m_bag.find(idx) != m_bag.end();
}


bool PropertyBag::remove_value_for_property(PropertyIndex idx)
{
    _Map::size_type sz = m_bag.erase(idx);
    return sz == 1;
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
