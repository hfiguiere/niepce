/*
 * niepce - libraryclient/libraryclient.cpp
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


#include "fwk/base/moniker.hpp"

#include "libraryclient.hpp"
#include "clientimpl.hpp"
#include "uidataprovider.hpp"

using eng::tid_t;
using eng::library_id_t;

namespace libraryclient {

const char * s_thumbcacheDirname = "thumbcache";

LibraryClient::LibraryClient(const fwk::Moniker & moniker, 
                             const fwk::NotificationCenter::Ptr & nc)
    : m_pImpl(ClientImpl::makeClientImpl(moniker, nc))
    , m_thumbnailCache(moniker.path() + "/" + s_thumbcacheDirname, nc)
    , m_uidataprovider(new UIDataProvider())
{
}

LibraryClient::~LibraryClient()
{
    delete m_pImpl;
    delete m_uidataprovider;
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

tid_t LibraryClient::queryFolderContent(eng::library_id_t id)
{
    return m_pImpl->queryFolderContent(id);
}

tid_t LibraryClient::queryKeywordContent(eng::library_id_t id)
{
    return m_pImpl->queryKeywordContent(id);
}

eng::tid_t LibraryClient::countFolder(library_id_t id)
{
    return m_pImpl->countFolder(id);
}

eng::tid_t LibraryClient::requestMetadata(eng::library_id_t id)
{
    return m_pImpl->requestMetadata(id);
}

/** set the metadata */
eng::tid_t LibraryClient::setMetadata(library_id_t id, fwk::PropertyIndex meta, const fwk::PropertyValue & value)
{
    // FIXME we should pass the value directly
    return m_pImpl->setMetadata(id, meta, boost::get<int>(value));
}

eng::tid_t LibraryClient::getAllLabels()
{
    return m_pImpl->getAllLabels();
}


eng::tid_t LibraryClient::createLabel(const std::string & s, const std::string & color)
{
    return m_pImpl->createLabel(s, color);
}


eng::tid_t LibraryClient::deleteLabel(int id)
{
    return m_pImpl->deleteLabel(id);
}


eng::tid_t LibraryClient::updateLabel(library_id_t id, const std::string & new_name,
                                          const std::string & new_color)
{
    return m_pImpl->updateLabel(id, new_name, new_color);
}

eng::tid_t LibraryClient::processXmpUpdateQueue()
{
    return m_pImpl->processXmpUpdateQueue();
}

void LibraryClient::importFromDirectory(const std::string & dir, bool manage)
{
    m_pImpl->importFromDirectory(dir, manage);
}

bool LibraryClient::fetchKeywordsForFile(int /*file*/, 
                                         eng::Keyword::IdList & /*keywords*/)
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
