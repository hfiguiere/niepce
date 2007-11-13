/*
 * niepce - library/op.h
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


#ifndef __NIEPCE_LIBRARY_OP_H__
#define __NIEPCE_LIBRARY_OP_H__

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>


namespace library {

	typedef enum {
		OP_NONE = 0,
		OP_QUERY_FILES,
		OP_UPDATE_FILES,
		OP_IMPORT_FILES
	} OpType;


	/** a library operation */
	class Op
	{
	public:
		typedef boost::shared_ptr< Op > Ptr;
		typedef unsigned int id_t; 
		typedef std::vector< boost::any > Args;

		Op(OpType t);
		~Op();

		OpType type()
			{ return m_type; }
		id_t id() const 
			{ return m_id; }

		Args & args()
			{ return m_args; }
	protected:
		Args m_args; /*< the arguments, free form */
	private:
		/** generate a new ID */
		static id_t newId();

		OpType m_type;
		id_t   m_id;
	};

}


#endif
