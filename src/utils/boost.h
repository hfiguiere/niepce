/*
 * niepce - utils/boost.h
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



#ifndef _UTILS_BOOST_H_
#define _UTILS_BOOST_H_

namespace Glib {

	/** Dereference Glib::RefPtr<> for use in boost::bind */
	template< class T_CppObject >
	T_CppObject *get_pointer(const Glib::RefPtr< T_CppObject >& p)
	{
		return p.operator->();
	}

}


#endif
