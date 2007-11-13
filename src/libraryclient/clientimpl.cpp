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

#include "utils/debug.h"
#include "utils/files.h"
#include "library/op.h"
#include "library/commands.h"
#include "clientimpl.h"
#include "locallibraryserver.h"

using utils::FileList;
using library::Op;
using library::Commands;

namespace libraryclient {
	
	ClientImpl *ClientImpl::makeClientImpl(const utils::Moniker & moniker)
	{
		return new ClientImpl(moniker);
	}
	
	ClientImpl::ClientImpl(const utils::Moniker & moniker)
		: m_moniker(moniker),
		  m_localLibrary(NULL)
	{
		DBG_OUT("creating implementation with moniker %s", 
						moniker.c_str());
		m_localLibrary = new LocalLibraryServer(moniker.path());
	}

	ClientImpl::~ClientImpl()
	{
		delete m_localLibrary;
	}

	tid ClientImpl::getAllKeywords()
	{
		return 0;
	}


	tid ClientImpl::getAllFolders()
	{
		return 0;
	}

	void ClientImpl::importFromDirectory(const std::string & dir, bool manage)
	{
		FileList::Ptr files;
		
		files = FileList::getFilesFromDirectory(dir);

		Op::Ptr op(Commands::opImportFiles(dir, files, manage));
		m_localLibrary->schedule(op);
	}

}


