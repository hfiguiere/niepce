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

namespace fwk {
	class Moniker;
	class NotificationCenter;
}

namespace libraryclient {

	class ClientImpl;

	class LibraryClient
		: public eng::Storage
	{
	public:
		typedef std::tr1::shared_ptr< LibraryClient > Ptr;

		LibraryClient(const fwk::Moniker & moniker, const fwk::NotificationCenter::Ptr & nc);
		virtual ~LibraryClient();

		static eng::tid_t newTid();
		/** get all the keywords 
		 * @return transaction ID
		 */
		eng::tid_t getAllKeywords();
		/** get all the folder
		 * @return transaction ID
		 */
		eng::tid_t getAllFolders();

		eng::tid_t queryFolderContent(int id);
		eng::tid_t queryKeywordContent(int id);
		eng::tid_t countFolder(int id);
		eng::tid_t requestMetadata(int id);

    /** set the metadata */
    eng::tid_t setMetadata(int id, int meta, int value);

    /** get all the labels */
    eng::tid_t getAllLabels();
    eng::tid_t createLabel(const std::string & s, const std::string & color);
    eng::tid_t deleteLabel(int id);
    /** update a label */
    eng::tid_t updateLabel(int id, const std::string & new_name, 
                               const std::string & new_color);

    /** tell to process the Xmp update Queue */
    eng::tid_t processXmpUpdateQueue();

		/** Import files from a directory
		 * @param dir the directory
		 * @param manage true if imports have to be managed
		 */
		void importFromDirectory(const std::string & dir, bool manage);
		
		eng::ThumbnailCache & thumbnailCache()
			{ return m_thumbnailCache; }

		/* sync call */
		virtual bool fetchKeywordsForFile(int file, eng::Keyword::IdList &keywords);

	private:
		ClientImpl* m_pImpl;

		eng::ThumbnailCache                    m_thumbnailCache;

		LibraryClient(const LibraryClient &);
		LibraryClient & operator=(const LibraryClient &);
	};

}

#endif
