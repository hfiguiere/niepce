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

void LibraryClient::getAllKeywords()
{
    m_pImpl->getAllKeywords();
}


void LibraryClient::getAllFolders()
{
    m_pImpl->getAllFolders();
}

void LibraryClient::queryFolderContent(eng::library_id_t id)
{
    m_pImpl->queryFolderContent(id);
}

void LibraryClient::queryKeywordContent(eng::library_id_t id)
{
    m_pImpl->queryKeywordContent(id);
}

void LibraryClient::countFolder(library_id_t id)
{
    m_pImpl->countFolder(id);
}

void LibraryClient::requestMetadata(eng::library_id_t id)
{
    m_pImpl->requestMetadata(id);
}

/** set the metadata */
void LibraryClient::setMetadata(library_id_t id, fwk::PropertyIndex meta,
                                      const fwk::PropertyValuePtr & value)
{
    m_pImpl->setMetadata(id, static_cast<eng::Np>(meta), value);
}

void LibraryClient::write_metadata(eng::library_id_t id)
{
    m_pImpl->write_metadata(id);
}


void LibraryClient::moveFileToFolder(eng::library_id_t file_id, eng::library_id_t from_folder,
                                           eng::library_id_t to_folder)
{
    m_pImpl->moveFileToFolder(file_id, from_folder, to_folder);
}

void LibraryClient::getAllLabels()
{
    m_pImpl->getAllLabels();
}


void LibraryClient::createLabel(const std::string & s, const std::string & color)
{
    m_pImpl->createLabel(s, color);
}


void LibraryClient::deleteLabel(int id)
{
    m_pImpl->deleteLabel(id);
}


void LibraryClient::updateLabel(library_id_t id, const std::string & new_name,
                                          const std::string & new_color)
{
    m_pImpl->updateLabel(id, new_name, new_color);
}

void LibraryClient::processXmpUpdateQueue(bool write_xmp)
{
    m_pImpl->processXmpUpdateQueue(write_xmp);
}

void LibraryClient::importFile(const std::string & path, eng::Managed manage)
{
    m_pImpl->importFile(path, manage);
}

void LibraryClient::importFromDirectory(const std::string & dir, eng::Managed manage)
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
