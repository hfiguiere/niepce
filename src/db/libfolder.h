/*
 * niepce - db/libfolder.h
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



#ifndef __NIEPCE_DB_LIBFOLDER_H__
#define __NIEPCE_DB_LIBFOLDER_H__

#include <string>
#include <list>
#include <boost/shared_ptr.hpp>

namespace db {

	class LibFolder
	{
	public:
		typedef boost::shared_ptr< LibFolder > Ptr;
		typedef std::list< Ptr > List;
		typedef boost::shared_ptr< List > ListPtr;

		LibFolder(int _id, std::string _name)
			: m_id(_id), m_name(_name)
			{
			}
		int id() const
			{ return m_id; }
		const std::string & name() const
			{ return m_name; }
	private:
		int         m_id;
		std::string m_name;
	};
}

#endif
