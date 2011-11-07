/*
 * niepce - libraryclient/clientimpl.cpp
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

#include <boost/bind.hpp>

#include "fwk/base/debug.hpp"
#include "fwk/utils/files.hpp"
#include "engine/library/op.hpp"
#include "engine/library/commands.hpp"
#include "libraryclient.hpp"
#include "clientimpl.hpp"
#include "locallibraryserver.hpp"

using fwk::FileList;
using eng::Op;
using eng::Commands;
using eng::tid_t;

namespace libraryclient {
	
ClientImpl *ClientImpl::makeClientImpl(const fwk::Moniker & moniker, 
                                       const fwk::NotificationCenter::Ptr & nc)
{
    return new ClientImpl(moniker, nc);
}

ClientImpl::ClientImpl(const fwk::Moniker & moniker, const fwk::NotificationCenter::Ptr & nc)
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

tid_t ClientImpl::queryFolderContent(eng::library_id_t folder_id)
{
    tid_t id = LibraryClient::newTid();
    Op::Ptr op(new Op(id, boost::bind(&Commands::cmdQueryFolderContent,
                                      _1, folder_id)));
    m_localLibrary->schedule(op);
    return id;
}


tid_t ClientImpl::countFolder(eng::library_id_t folder_id)
{
    tid_t id = LibraryClient::newTid();
    Op::Ptr op(new Op(id, boost::bind(&Commands::cmdCountFolder, 
                                      _1, folder_id)));
    m_localLibrary->schedule(op);
    return id;
}


tid_t ClientImpl::queryKeywordContent(eng::library_id_t keyword_id)
{
    tid_t id = LibraryClient::newTid();
    Op::Ptr op(new Op(id, boost::bind(&Commands::cmdQueryKeywordContent,
                                      _1, keyword_id)));
    m_localLibrary->schedule(op);
    return id;
}


tid_t ClientImpl::requestMetadata(eng::library_id_t file_id)
{
    tid_t id = LibraryClient::newTid();
    Op::Ptr op(new Op(id, boost::bind(&Commands::cmdRequestMetadata,
                                      _1, file_id)));
    m_localLibrary->schedule(op);
    return id;
}


tid_t ClientImpl::setMetadata(eng::library_id_t file_id, int meta, int value)
{
    tid_t id = LibraryClient::newTid();
    Op::Ptr op(new Op(id, boost::bind(&Commands::cmdSetMetadata, _1,
                                      file_id, meta, value)));
    m_localLibrary->schedule(op);
    return id;
}


tid_t ClientImpl::getAllLabels()
{
    tid_t id = LibraryClient::newTid();
    Op::Ptr op(new Op(id, boost::bind(&Commands::cmdListAllLabels,
                                      _1)));
    m_localLibrary->schedule(op);
    return id;    
}


tid_t ClientImpl::createLabel(const std::string & s, const std::string & color)
{
    tid_t id = LibraryClient::newTid();
    Op::Ptr op(new Op(id, boost::bind(&Commands::cmdCreateLabel,
                                      _1, s, color)));
    m_localLibrary->schedule(op);
    return id;    
}

tid_t ClientImpl::deleteLabel(int label_id)
{
    tid_t id = LibraryClient::newTid();
    Op::Ptr op(new Op(id, boost::bind(&Commands::cmdDeleteLabel,
                                      _1, label_id)));
    m_localLibrary->schedule(op);
    return id;    
}

tid_t ClientImpl::updateLabel(eng::library_id_t label_id, const std::string & new_name,
                              const std::string & new_color)
{
    tid_t id = LibraryClient::newTid();
    Op::Ptr op(new Op(id, boost::bind(&Commands::cmdUpdateLabel,
                                      _1, label_id, new_name, new_color)));
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
                                            boost::bind(&fwk::filter_none, _1));
    
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
