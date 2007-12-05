/*
 * niepce - utils/geometry.h
 *
 * Copyright (C) 2007 Hubert Figuiere
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


#ifndef __UTILS_GEOMETRY_H__
#define __UTILS_GEOMETRY_H__

#include <boost/array.hpp>
#include <string>

namespace utils {

	class Rect
	{
	public:
		Rect();
		Rect(int x, int y, int w, int h);
		/** build a Rect from a string 
		 * @param s string whose format is "x y w h" as decimal ints.
		 * @throw a std::bad_cast exception if there is not 4 element
		 * or if one of them is not an int.
		 */
		Rect(const std::string & s) 
			throw(std::bad_cast);
		
		int x()
			{ return _r[X]; }
		int y() 
			{ return _r[Y]; }
		int w()
			{ return _r[W]; }
		int h()
			{ return _r[H]; }
		/** convert to a string in the same format as
		 * accepted by the %Rect(const std::string & s) constructor.
		 */
		std::string to_string() const;
	private:
		/** the indices */
		enum {
			X = 0,
			Y,
			W,
			H			
		};
		boost::array<int, 4> _r;
	};

}

#endif
