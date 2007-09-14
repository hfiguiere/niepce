/*
 * niepce - libraryclient/clientimpl.h
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


#ifndef _LIBRARYCLIENT_CLIENTIMPL_H_
#define _LIBRARYCLIENT_CLIENTIMPL_H_

#include <string>

#include "clienttypes.h"

namespace libraryclient {

	class LocalLibraryServer;

	class ClientImpl
	{
	public:
		static ClientImpl *makeClientImpl(const std::string & moniker);

		ClientImpl(const std::string & moniker);
		virtual ~ClientImpl();

		tid getAllKeywords();
	  tid getAllFolders();

	protected:
		const std::string m_moniker;
		LocalLibraryServer *m_localLibrary;
	};

}


#endif
