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

namespace db {
	
	LibFile::LibFile(int id, int folderId, const std::string &name,
					const std::string & relPath)
		: m_id(id), m_folderId(folderId),
			m_name(name), m_relativePath(relPath),
			m_hasKeywordList(false)
	{

	}

	LibFile::~LibFile()
	{
	}


	const Keyword::IdList & LibFile::keywords() const
	{
		if(!m_hasKeywordList) {
			storage()->fetchKeywordsForFile(m_id, m_keywordList);
		}
		return m_keywordList;
	}

}
