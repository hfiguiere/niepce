/*
 * niepce - utils/boost.h
 *
 * Copyright (C) 2007-2009 Hubert Figuiere
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

#include <tr1/memory>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <glibmm/refptr.h>

namespace Glib {

	/** Dereference Glib::RefPtr<> for use in boost::bind */
	template< class T_CppObject > inline
	T_CppObject *get_pointer(const Glib::RefPtr< T_CppObject >& p)
	{
		return p.operator->();
	}

}


namespace std {
namespace tr1 {

template< class T_CppObject > inline
T_CppObject *get_pointer(const shared_ptr< T_CppObject > &p)
{
    return p.get();
}


}
}

namespace utils {

/** function to make a shared_ptr<> form a weak_ptr<> 
 * idea from http://lists.boost.org/boost-users/2007/01/24384.php
 */
template<class T> 
std::tr1::shared_ptr<T> shared_ptr_from( std::tr1::weak_ptr<T> const & wpt )
{
    return std::tr1::shared_ptr<T>( wpt ); // throws on wpt.expired()
} 

}

/** need to convert a weak_ptr<> to a shared_ptr<> in the bind() 
 * this allow breaking some circular references.
 */
#define BIND_SHARED_PTR(_type, _spt) \
    boost::bind(&utils::shared_ptr_from<_type>, std::tr1::weak_ptr<_type>(_spt))


#endif
/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:80
  End:
*/
