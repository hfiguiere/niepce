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

#include "library/clienttypes.h"

namespace library {

	typedef enum {
		OP_NONE = 0,
		OP_QUERY_FILES,
		OP_UPDATE_FILES,
		OP_LIST_ALL_FOLDERS,
		OP_LIST_ALL_KEYWORDS,
		OP_IMPORT_FILES,
		OP_QUERY_FOLDER_CONTENT,
		OP_QUERY_KEYWORD_CONTENT
	} OpType;


	/** a library operation */
	class Op
	{
	public:
		typedef boost::shared_ptr< Op > Ptr;
		typedef std::vector< boost::any > Args;

		Op(OpType t, tid_t id);
		~Op();

		OpType type()
			{ return m_type; }
		tid_t id() const 
			{ return m_id; }

		Args & args()
			{ return m_args; }
	protected:
		Args m_args; /*< the arguments, free form */
	private:
		OpType m_type;
		tid_t   m_id;
	};

}


#endif
