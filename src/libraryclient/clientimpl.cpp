/*
 * niepce - libraryclient/libraryclient.cpp
 *
 * Copyright (C) 2007 Hubert Figuiere
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  
 * 02110-1301, USA
 */



#include "clientimpl.h"
#include "locallibraryserver.h"


namespace libraryclient {
	
	ClientImpl *ClientImpl::makeClientImpl(const std::string & moniker)
	{
		return new ClientImpl(moniker);
	}
	
	ClientImpl::ClientImpl(const std::string & moniker)
		: m_moniker(moniker),
			m_localLibrary(NULL)
	{
		m_localLibrary = new LocalLibraryServer();
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

}


