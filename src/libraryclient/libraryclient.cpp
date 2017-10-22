/*
 * niepce - libraryclient/libraryclient.cpp
 *
 * Copyright (C) 2007-2017 Hubert Figuière
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
#include "fwk/utils/files.hpp"

#include "libraryclient.hpp"
#include "uidataprovider.hpp"

#include "rust_bindings.hpp"

using eng::library_id_t;

namespace libraryclient {

const char * s_thumbcacheDirname = "thumbcache";

LibraryClient::LibraryClient(const fwk::Moniker & moniker,
                             uint64_t notif_id)
    : m_impl(ffi::libraryclient_clientimpl_new(moniker.path().c_str(), notif_id))
    , m_thumbnailCache(moniker.path() + "/" + s_thumbcacheDirname, notif_id)
    , m_uidataprovider(new UIDataProvider())
    , m_trash_id(0)
{
}

LibraryClient::~LibraryClient()
{
    ffi::libraryclient_clientimpl_delete(m_impl);
}

bool LibraryClient::ok() const
{
    return m_impl;
}

void LibraryClient::getAllKeywords()
{
    ffi::libraryclient_clientimpl_get_all_keywords(m_impl);
}


void LibraryClient::getAllFolders()
{
    ffi::libraryclient_clientimpl_get_all_folders(m_impl);
}

void LibraryClient::queryFolderContent(eng::library_id_t folder_id)
{
    ffi::libraryclient_clientimpl_query_folder_content(m_impl, folder_id);
}

void LibraryClient::queryKeywordContent(eng::library_id_t keyword_id)
{
    ffi::libraryclient_clientimpl_query_keyword_content(m_impl, keyword_id);
}

void LibraryClient::countFolder(library_id_t folder_id)
{
    ffi::libraryclient_clientimpl_count_folder(m_impl, folder_id);
}

void LibraryClient::requestMetadata(eng::library_id_t file_id)
{
    ffi::libraryclient_clientimpl_request_metadata(m_impl, file_id);
}

/** set the metadata */
void LibraryClient::setMetadata(library_id_t file_id, fwk::PropertyIndex meta,
                                      const fwk::PropertyValuePtr & value)
{
    ffi::libraryclient_clientimpl_set_metadata(m_impl, file_id, static_cast<eng::Np>(meta),
                                               value.get());
}

void LibraryClient::write_metadata(eng::library_id_t file_id)
{
    ffi::libraryclient_clientimpl_write_metadata(m_impl, file_id);
}


void LibraryClient::moveFileToFolder(eng::library_id_t file_id, eng::library_id_t from_folder_id,
                                           eng::library_id_t to_folder_id)
{
    ffi::libraryclient_clientimpl_move_file_to_folder(m_impl, file_id, from_folder_id,
                                                      to_folder_id);
}

void LibraryClient::getAllLabels()
{
    ffi::libraryclient_clientimpl_get_all_labels(m_impl);
}


void LibraryClient::createLabel(const std::string& s, const std::string& colour)
{
    ffi::libraryclient_clientimpl_create_label(m_impl, s.c_str(), colour.c_str());
}


void LibraryClient::deleteLabel(int label_id)
{
    ffi::libraryclient_clientimpl_delete_label(m_impl, label_id);
}


void LibraryClient::updateLabel(library_id_t label_id, const std::string& new_name,
                                          const std::string& new_colour)
{
    ffi::libraryclient_clientimpl_update_label(m_impl, label_id,
                                               new_name.c_str(), new_colour.c_str());
}

void LibraryClient::processXmpUpdateQueue(bool write_xmp)
{
    ffi::libraryclient_clientimpl_process_xmp_update_queue(m_impl, write_xmp);
}

void LibraryClient::importFile(const std::string& path, eng::Managed manage)
{
    ffi::libraryclient_clientimpl_import_file(m_impl, path.c_str(), manage);
}

void LibraryClient::importFromDirectory(const std::string& dir, eng::Managed manage)
{
    fwk::FileListPtr files;
    files = fwk::FileList::getFilesFromDirectory(dir, &fwk::filter_none);

    ffi::libraryclient_clientimpl_import_files(m_impl, dir.c_str(), files.get(), manage);
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
