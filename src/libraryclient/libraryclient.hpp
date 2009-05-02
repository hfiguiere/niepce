/*
 * niepce - libraryclient/libraryclient.hpp
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

#ifndef _LIBRARYCLIENT_H_
#define _LIBRARYCLIENT_H_

#include <string>
#include <tr1/memory>

#include "engine/library/clienttypes.hpp"
#include "engine/library/thumbnailcache.hpp"
#include "engine/db/storage.hpp"

namespace utils {
	class Moniker;
}

namespace fwk {
	class NotificationCenter;
}

namespace libraryclient {

	class ClientImpl;

	class LibraryClient
		: public db::Storage
	{
	public:
		typedef std::tr1::shared_ptr< LibraryClient > Ptr;

		LibraryClient(const utils::Moniker & moniker, const fwk::NotificationCenter::Ptr & nc);
		virtual ~LibraryClient();

		static library::tid_t newTid();
		/** get all the keywords 
		 * @return transaction ID
		 */
		library::tid_t getAllKeywords();
		/** get all the folder
		 * @return transaction ID
		 */
		library::tid_t getAllFolders();

		library::tid_t queryFolderContent(int id);
		library::tid_t queryKeywordContent(int id);
		library::tid_t countFolder(int id);
		library::tid_t requestMetadata(int id);

    /** set the metadata */
    library::tid_t setMetadata(int id, int meta, int value);

    /** get all the labels */
    library::tid_t getAllLabels();
    library::tid_t createLabel(const std::string & s, const std::string & color);
    library::tid_t deleteLabel(int id);
    /** update a label */
    library::tid_t updateLabel(int id, const std::string & new_name, 
                               const std::string & new_color);

    /** tell to process the Xmp update Queue */
    library::tid_t processXmpUpdateQueue();

		/** Import files from a directory
		 * @param dir the directory
		 * @param manage true if imports have to be managed
		 */
		void importFromDirectory(const std::string & dir, bool manage);
		
		library::ThumbnailCache & thumbnailCache()
			{ return m_thumbnailCache; }

		/* sync call */
		virtual bool fetchKeywordsForFile(int file, db::Keyword::IdList &keywords);

	private:
		ClientImpl* m_pImpl;

		library::ThumbnailCache                    m_thumbnailCache;

		LibraryClient(const LibraryClient &);
		LibraryClient & operator=(const LibraryClient &);
	};

}

#endif
