/*
 * niepce - libraryclient/clientimpl.h
 *
 * Copyright (C) 2007-2008 Hubert Figuiere
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


#ifndef _LIBRARYCLIENT_CLIENTIMPL_H_
#define _LIBRARYCLIENT_CLIENTIMPL_H_

#include <string>

#include "utils/moniker.h"
#include "library/clienttypes.h"


namespace libraryclient {

	class LocalLibraryServer;

	class ClientImpl
	{
	public:
		static ClientImpl *makeClientImpl(const utils::Moniker & moniker, 
										  const framework::NotificationCenter::Ptr & nc);
		
		ClientImpl(const utils::Moniker & moniker, const framework::NotificationCenter::Ptr & nc);
		virtual ~ClientImpl();

		library::tid_t getAllKeywords();
		library::tid_t queryKeywordContent(int id);
		library::tid_t getAllFolders();
		library::tid_t queryFolderContent(int id);
		library::tid_t countFolder(int id);
		library::tid_t requestMetadata(int id);
        library::tid_t setMetadata(int id, int meta, int value);

		library::tid_t importFromDirectory(const std::string & dir, bool manage);

	protected:
		const utils::Moniker m_moniker;
		LocalLibraryServer *m_localLibrary;
	};

}


#endif
