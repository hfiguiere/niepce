/*
 * niepce - libraryclient/libraryclient.cpp
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

#include "utils/moniker.h"

#include "libraryclient.h"
#include "clientimpl.h"

namespace libraryclient {

	LibraryClient::LibraryClient(const utils::Moniker & moniker)
		: m_pImpl(ClientImpl::makeClientImpl(moniker))
	{

	}

	LibraryClient::~LibraryClient()
	{
		delete m_pImpl;
	}


	tid LibraryClient::getAllKeywords()
	{
		return m_pImpl->getAllKeywords();
	}


	tid LibraryClient::getAllFolders()
	{
		return m_pImpl->getAllFolders();
	}

	void LibraryClient::importFromDirectory(const std::string & dir, bool manage)
	{
		m_pImpl->importFromDirectory(dir, manage);
	}

	bool LibraryClient::fetchKeywordsForFile(int file, 
											 library::Keyword::IdList &keywords)
	{
		// TODO
		return false;
	}

}
