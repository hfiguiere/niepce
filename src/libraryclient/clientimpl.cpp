/*
 * niepce - libraryclient/clientimpl.cpp
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

#include "fwk/base/debug.hpp"
#include "fwk/utils/files.hpp"
#include "libraryclient.hpp"
#include "clientimpl.hpp"

#include "rust_bindings.hpp"

using fwk::FileListPtr;
using eng::Managed;

namespace libraryclient {

std::unique_ptr<ClientImpl> ClientImpl::makeClientImpl(const fwk::Moniker & moniker,
                                                       uint64_t notif_id)
{
    return std::unique_ptr<ClientImpl>(new ClientImpl(moniker, notif_id));
}

ClientImpl::ClientImpl(const fwk::Moniker & moniker, uint64_t notif_id)
    : m_moniker(moniker)
    , m_impl(ffi::libraryclient_clientimpl_new(moniker.path().c_str(), notif_id))
{
    DBG_OUT("creating implementation with moniker %s",
            moniker.c_str());
}

ClientImpl::~ClientImpl()
{
    ffi::libraryclient_clientimpl_delete(m_impl);
}

bool ClientImpl::ok() const
{
    return m_impl;
}

void ClientImpl::getAllKeywords()
{
    ffi::libraryclient_clientimpl_get_all_keywords(m_impl);
}

void ClientImpl::getAllFolders()
{
    ffi::libraryclient_clientimpl_get_all_folders(m_impl);
}

void ClientImpl::queryFolderContent(eng::library_id_t folder_id)
{
    ffi::libraryclient_clientimpl_query_folder_content(m_impl, folder_id);
}

void ClientImpl::countFolder(eng::library_id_t folder_id)
{
    ffi::libraryclient_clientimpl_count_folder(m_impl, folder_id);
}


void ClientImpl::queryKeywordContent(eng::library_id_t keyword_id)
{
    ffi::libraryclient_clientimpl_query_keyword_content(m_impl, keyword_id);
}


void ClientImpl::requestMetadata(eng::library_id_t file_id)
{
    ffi::libraryclient_clientimpl_request_metadata(m_impl, file_id);
}


void ClientImpl::setMetadata(eng::library_id_t file_id, eng::Np meta,
                              const fwk::PropertyValuePtr & value)
{
    ffi::libraryclient_clientimpl_set_metadata(m_impl, file_id, meta, value.get());
}

void ClientImpl::write_metadata(eng::library_id_t file_id)
{
    ffi::libraryclient_clientimpl_write_metadata(m_impl, file_id);
}

void ClientImpl::moveFileToFolder(eng::library_id_t file_id,
                                   eng::library_id_t from_folder_id,
                                   eng::library_id_t to_folder_id)
{
    ffi::libraryclient_clientimpl_move_file_to_folder(m_impl, file_id, from_folder_id,
                                                      to_folder_id);
}

void ClientImpl::getAllLabels()
{
    ffi::libraryclient_clientimpl_get_all_labels(m_impl);
}

void ClientImpl::createLabel(const std::string & s, const std::string & colour)
{
    ffi::libraryclient_clientimpl_create_label(m_impl, s.c_str(), colour.c_str());
}

void ClientImpl::deleteLabel(int label_id)
{
    ffi::libraryclient_clientimpl_delete_label(m_impl, label_id);
}

void ClientImpl::updateLabel(eng::library_id_t label_id,
                              const std::string & new_name,
                              const std::string & new_colour)
{
    ffi::libraryclient_clientimpl_update_label(m_impl, label_id,
                                               new_name.c_str(), new_colour.c_str());
}


void ClientImpl::processXmpUpdateQueue(bool write_xmp)
{
    ffi::libraryclient_clientimpl_process_xmp_update_queue(m_impl, write_xmp);
}

void ClientImpl::importFile(const std::string & path, Managed manage)
{
    ffi::libraryclient_clientimpl_import_file(m_impl, path.c_str(), manage);
}

void ClientImpl::importFromDirectory(const std::string & dir, Managed manage)
{
    FileListPtr files;

    files = fwk::FileList::getFilesFromDirectory(dir, &fwk::filter_none);

    ffi::libraryclient_clientimpl_import_files(m_impl, dir.c_str(), files.get(), manage);
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
