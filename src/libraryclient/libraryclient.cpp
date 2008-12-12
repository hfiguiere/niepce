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

#include <boost/filesystem/path.hpp>

#include "fwk/utils/moniker.h"

#include "libraryclient.h"
#include "clientimpl.h"

using library::tid_t;

namespace bfs = boost::filesystem;

namespace libraryclient {

const char * s_thumbcacheDirname = "thumbcache";

LibraryClient::LibraryClient(const utils::Moniker & moniker, 
                             const framework::NotificationCenter::Ptr & nc)
    : m_pImpl(ClientImpl::makeClientImpl(moniker, nc)),
      m_thumbnailCache(bfs::path(moniker.path()) / s_thumbcacheDirname, nc)
{

}

LibraryClient::~LibraryClient()
{
    delete m_pImpl;
}

tid_t LibraryClient::newTid()
{
    static tid_t id = 0;
    id++;
    return id;
}


tid_t LibraryClient::getAllKeywords()
{
    return m_pImpl->getAllKeywords();
}


tid_t LibraryClient::getAllFolders()
{
    return m_pImpl->getAllFolders();
}

tid_t LibraryClient::queryFolderContent(int id)
{
    return m_pImpl->queryFolderContent(id);
}

tid_t LibraryClient::queryKeywordContent(int id)
{
    return m_pImpl->queryKeywordContent(id);
}

library::tid_t LibraryClient::countFolder(int id)
{
    return m_pImpl->countFolder(id);
}

library::tid_t LibraryClient::requestMetadata(int id)
{
    return m_pImpl->requestMetadata(id);
}

/** set the metadata */
library::tid_t LibraryClient::setMetadata(int id, int meta, int value)
{
    return m_pImpl->setMetadata(id, meta, value);
}

library::tid_t LibraryClient::processXmpUpdateQueue()
{
    return m_pImpl->processXmpUpdateQueue();
}

void LibraryClient::importFromDirectory(const std::string & dir, bool manage)
{
    m_pImpl->importFromDirectory(dir, manage);
}

bool LibraryClient::fetchKeywordsForFile(int /*file*/, 
                                         db::Keyword::IdList & /*keywords*/)
{
    // TODO
    return false;
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
