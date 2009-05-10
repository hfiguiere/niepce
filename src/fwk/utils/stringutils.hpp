/*
 * niepce - fwk/utils/stringutils.hpp
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


#ifndef __UTILS_STRINGUTILS_H__
#define __UTILS_STRINGUTILS_H__

#include <boost/range/value_type.hpp>

namespace fwk {

/** join elements with a separator 
 * boost has such an algorithm in later versions. This is not it, but does a
 * similar thing.
 */
template<typename S, typename C>
typename boost::range_value< S >::type join(const S & input, const C & separator)
{
	typename boost::range_value< S >::type joint;
	typename S::const_iterator iter(input.begin());
	bool first = true;
	for( ; iter != input.end(); iter++) {
		if(!first) {
			joint += separator;
		}
		joint += *iter;
		first = false;
	}
	return joint;
}


}

#endif
