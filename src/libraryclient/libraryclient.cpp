/*
 * niepce - libraryclient/libraryclient.cpp
 *
 * Copyright (C) 2007-2015 Hubert Figuière
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
                             uint64_t notif_id)
    : m_pImpl(ClientImpl::makeClientImpl(moniker, notif_id))
    , m_thumbnailCache(moniker.path() + "/" + s_thumbcacheDirname, notif_id)
    , m_uidataprovider(new UIDataProvider())
    , m_trash_id(0)
{
}

LibraryClient::~LibraryClient()
{
}

bool LibraryClient::ok() const
{
    return m_pImpl && m_pImpl->ok();
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
eng::tid_t LibraryClient::setMetadata(library_id_t id, fwk::PropertyIndex meta,
                                      const fwk::PropertyValuePtr & value)
{
    return m_pImpl->setMetadata(id, meta, value);
}

eng::tid_t LibraryClient::write_metadata(eng::library_id_t id)
{
    return m_pImpl->write_metadata(id);
}


eng::tid_t LibraryClient::moveFileToFolder(eng::library_id_t file_id, eng::library_id_t from_folder,
                                           eng::library_id_t to_folder)
{
    return m_pImpl->moveFileToFolder(file_id, from_folder, to_folder);
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

eng::tid_t LibraryClient::processXmpUpdateQueue(bool write_xmp)
{
    return m_pImpl->processXmpUpdateQueue(write_xmp);
}

void LibraryClient::importFile(const std::string & path, eng::LibraryManaged manage)
{
    m_pImpl->importFile(path, manage);
}

void LibraryClient::importFromDirectory(const std::string & dir, eng::LibraryManaged manage)
{
    m_pImpl->importFromDirectory(dir, manage);
}

#if 0
bool LibraryClient::fetchKeywordsForFile(int /*file*/, 
                                         eng::Keyword::IdList & /*keywords*/)
{
    // TODO
    return false;
}
#endif

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
