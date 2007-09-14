/*
 * niepce - library/libfile.h
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





#ifndef __NIEPCE_LIBRARY_LIBFILE_H__
#define __NIEPCE_LIBRARY_LIBFILE_H__

#include <string>
#include <list>
#include <boost/shared_ptr.hpp>

#include "library/keyword.h"
#include "library/storage.h"

namespace library {

	class LibFile
	{
	public:
		typedef boost::shared_ptr< LibFile > Ptr;
		typedef std::list< Ptr > List;

		LibFile(int id, int folderId, const std::string &name,
						const std::string & relPath);
		virtual ~LibFile();

		int id() const
			{ return m_id; }
		int folderId() const
			{ return m_folderId; }
		const std::string & name() const
			{ return m_name; }
		const std::string & relativePath() const
			{ return m_relativePath; }

		Storage::Ptr storage() const;

		/** retrieve the keywords id list 
		 * @return the list
		 */
		const Keyword::IdList & keywords() const;
		
		const std::string & uri() const
			{ return m_uri; }
		/** check is the library file is at uri
		 * @return true of the uri match
		 * @todo
		 */
		bool isUri(const char * _uri) const
			{ return uri() == _uri; }
	private:
		int         m_id;           /**< file ID */
		int         m_folderId;     /**< parent folder */
		std::string m_name;         /**< name */
		std::string m_relativePath; /**< path name relative to the folder */
		std::string m_uri;          /**< the URI */
		std::string m_type;
		mutable bool m_hasKeywordList;
		mutable Keyword::IdList m_keywordList;
	};

}


#endif

