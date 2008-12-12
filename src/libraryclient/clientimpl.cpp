/*
 * niepce - libraryclient/libraryclient.cpp
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


#include "fwk/utils/debug.h"
#include "fwk/utils/files.h"
#include "library/op.h"
#include "library/commands.h"
#include "libraryclient.h"
#include "clientimpl.h"
#include "locallibraryserver.h"

using utils::FileList;
using library::Op;
using library::Commands;
using library::tid_t;

namespace libraryclient {
	
ClientImpl *ClientImpl::makeClientImpl(const utils::Moniker & moniker, 
                                       const framework::NotificationCenter::Ptr & nc)
{
    return new ClientImpl(moniker, nc);
}

ClientImpl::ClientImpl(const utils::Moniker & moniker, const framework::NotificationCenter::Ptr & nc)
    : m_moniker(moniker),
      m_localLibrary(NULL)
{
    DBG_OUT("creating implementation with moniker %s", 
            moniker.c_str());
    m_localLibrary = new LocalLibraryServer(moniker.path(), nc);
}

ClientImpl::~ClientImpl()
{
    delete m_localLibrary;
}

tid_t ClientImpl::getAllKeywords()
{
    tid_t id = LibraryClient::newTid();
    Op::Ptr op(new Op(id, boost::bind(&Commands::cmdListAllKeywords, _1)));
    m_localLibrary->schedule(op);
    return id;
}


tid_t ClientImpl::getAllFolders()
{
    tid_t id = LibraryClient::newTid();
    Op::Ptr op(new Op(id, boost::bind(&Commands::cmdListAllFolders, _1)));
    m_localLibrary->schedule(op);
    return id;
}

tid_t ClientImpl::queryFolderContent(int folder_id)
{
    tid_t id = LibraryClient::newTid();
    Op::Ptr op(new Op(id, boost::bind(&Commands::cmdQueryFolderContent,
                                      _1, folder_id)));
    m_localLibrary->schedule(op);
    return id;
}


tid_t ClientImpl::countFolder(int folder_id)
{
    tid_t id = LibraryClient::newTid();
    Op::Ptr op(new Op(id, boost::bind(&Commands::cmdCountFolder, 
                                      _1, folder_id)));
    m_localLibrary->schedule(op);
    return id;
}


tid_t ClientImpl::queryKeywordContent(int keyword_id)
{
    tid_t id = LibraryClient::newTid();
    Op::Ptr op(new Op(id, boost::bind(&Commands::cmdQueryKeywordContent,
                                      _1, keyword_id)));
    m_localLibrary->schedule(op);
    return id;
}


tid_t ClientImpl::requestMetadata(int file_id)
{
    tid_t id = LibraryClient::newTid();
    Op::Ptr op(new Op(id, boost::bind(&Commands::cmdRequestMetadata,
                                      _1, file_id)));
    m_localLibrary->schedule(op);
    return id;
}


tid_t ClientImpl::setMetadata(int file_id, int meta, int value)
{
    tid_t id = LibraryClient::newTid();
    Op::Ptr op(new Op(id, boost::bind(&Commands::cmdSetMetadata, _1,
                                      file_id, meta, value)));
    m_localLibrary->schedule(op);
    return id;
}


tid_t ClientImpl::processXmpUpdateQueue()
{
    tid_t id = LibraryClient::newTid();
    Op::Ptr op(new Op(id, boost::bind(&Commands::cmdProcessXmpUpdateQueue,
                                      _1)));
    m_localLibrary->schedule(op);
    return id;
}


tid_t ClientImpl::importFromDirectory(const std::string & dir, bool manage)
{
    FileList::Ptr files;
	
    files = FileList::getFilesFromDirectory(dir, 
                                            boost::bind(&utils::filter_xmp_out, _1));
    
    tid_t id = LibraryClient::newTid();
    Op::Ptr op(new Op(id, boost::bind(&Commands::cmdImportFiles,
                                      _1, dir, files, manage)));
    m_localLibrary->schedule(op);
    return id;
}

}



/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
