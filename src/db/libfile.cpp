/*
 * niepce - db/libfile.cpp
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


#include "libfile.h"

namespace bfs = boost::filesystem;

namespace db {
	
	LibFile::LibFile(int _id, int _folderId, const bfs::path & p,
					 const std::string & _name )
	: m_id(_id), m_folderId(_folderId),
	  m_name(_name), m_path(p),
	  m_orientation(0), m_rating(0), m_label(0)
	{

	}

	LibFile::~LibFile()
	{
	}


	const Keyword::IdList & LibFile::keywords() const
	{
		if(!m_hasKeywordList) {
//			storage()->fetchKeywordsForFile(m_id, m_keywordList);
		}
		return m_keywordList;
	}

	void LibFile::setOrientation(int32_t v)
	{
		m_orientation = v;
	}


	void LibFile::setRating(int32_t v)
	{
		m_rating = v;
	}


	void LibFile::setLabel(int32_t v)
	{
		m_label = v;
	}

}
