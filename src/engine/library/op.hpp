/*
 * niepce - engine/library/op.h
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


#ifndef __NIEPCE_LIBRARY_OP_H__
#define __NIEPCE_LIBRARY_OP_H__

#include <tr1/memory>
#include <boost/function.hpp>

#include "engine/library/clienttypes.hpp"
#include "engine/db/library.hpp"

namespace library {

	/** a library operation */
	class Op
	{
	public:
		typedef std::tr1::shared_ptr< Op > Ptr;
		typedef boost::function<void (const db::Library::Ptr &)> function_t;

		Op(tid_t id, const function_t & func);

		tid_t id() const 
			{ return m_id; }

        void operator() (const db::Library::Ptr &);
		const function_t & fn() const
			{ return m_function; }
	protected:
	private:
		tid_t   m_id;
		function_t m_function;
	};

}


#endif
/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
