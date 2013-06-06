/*
 * niepce - eng/db/keyword.hpp
 *
 * Copyright (C) 2007-2013 Hubert Figuiere
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


#ifndef __NIEPCE_DB_KEYWORD_H__
#define __NIEPCE_DB_KEYWORD_H__

#include <string>
#include <vector>
#include <memory>

#include "engine/db/librarytypes.hpp"

namespace eng {

	class Keyword
	{
	public:
		typedef std::shared_ptr<Keyword> Ptr;
		typedef std::vector<Ptr> List;
		typedef std::shared_ptr<List> ListPtr;
		typedef std::vector<int> IdList;

		Keyword(library_id_t id, const std::string & keyword);

		library_id_t id() const
			{ return m_id; }
		const std::string & keyword()
			{ return m_keyword; }
	private:
		library_id_t m_id;
		std::string m_keyword;
	};

}

#endif
