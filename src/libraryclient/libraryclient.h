/*
 * niepce - libraryclient/libraryclient.h
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

#ifndef _LIBRARYCLIENT_H_
#define _LIBRARYCLIENT_H_

#include <string>
#include <boost/shared_ptr.hpp>

#include "clienttypes.h"
#include "library/storage.h"

namespace libraryclient {

	class ClientImpl;

	class LibraryClient
		: public library::Storage
	{
	public:
		LibraryClient(const std::string & moniker);
		virtual ~LibraryClient();

		/** get all the keywords 
		 * @return transaction ID
		 */
		tid getAllKeywords();
		/** get all the folder
		 * @return transaction ID
		 */
	  tid getAllFolders();

		/* sync call */
		virtual bool fetchKeywordsForFile(int file, library::Keyword::IdList &keywords);

	private:
		ClientImpl* m_pImpl;

		LibraryClient(const LibraryClient &);
		LibraryClient & operator=(const LibraryClient &);
	};

}

#endif
