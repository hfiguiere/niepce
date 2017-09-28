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
#include "engine/library/op.hpp"
#include "libraryclient.hpp"
#include "clientimpl.hpp"
#include "locallibraryserver.hpp"

#include "rust_bindings.hpp"

using fwk::FileListPtr;
using eng::Op;
using eng::Managed;
using eng::tid_t;

namespace libraryclient {

std::unique_ptr<ClientImpl> ClientImpl::makeClientImpl(const fwk::Moniker & moniker,
                                                       uint64_t notif_id)
{
    return std::unique_ptr<ClientImpl>(new ClientImpl(moniker, notif_id));
}

ClientImpl::ClientImpl(const fwk::Moniker & moniker, uint64_t notif_id)
    : m_moniker(moniker),
      m_localLibrary(new LocalLibraryServer(moniker.path(), notif_id))
{
    DBG_OUT("creating implementation with moniker %s",
            moniker.c_str());
}

ClientImpl::~ClientImpl()
{
}

bool ClientImpl::ok() const
{
    return m_localLibrary && m_localLibrary->ok();
}

/**
   Schedule an operation.
   @param func the function to schedule
   @return the tid
 */
tid_t ClientImpl::schedule_op(const Op::Function & func)
{
    tid_t id = LibraryClient::newTid();
    Op::Ptr op(new Op(id, func));
    m_localLibrary->schedule(op);
    return id;
}

tid_t ClientImpl::getAllKeywords()
{
    return schedule_op(&ffi::cmd_list_all_keywords);
}


tid_t ClientImpl::getAllFolders()
{
    return schedule_op(&ffi::cmd_list_all_folders);
}

tid_t ClientImpl::queryFolderContent(eng::library_id_t folder_id)
{
    return schedule_op([folder_id](const auto& lib) {
            return ffi::cmd_query_folder_content(lib, folder_id);
        });
}


tid_t ClientImpl::countFolder(eng::library_id_t folder_id)
{
    return schedule_op([folder_id](const auto& lib) {
            return ffi::cmd_count_folder(lib, folder_id);
        });
}


tid_t ClientImpl::queryKeywordContent(eng::library_id_t keyword_id)
{
    return schedule_op([keyword_id](const auto& lib) {
            return ffi::cmd_query_keyword_content(lib, keyword_id);
        });
}


tid_t ClientImpl::requestMetadata(eng::library_id_t file_id)
{
    return schedule_op([file_id](const auto& lib) {
            return ffi::cmd_request_metadata(lib, file_id);
        });
}


tid_t ClientImpl::setMetadata(eng::library_id_t file_id, eng::Np meta,
                              const fwk::PropertyValuePtr & value)
{
    return schedule_op([file_id, meta, value](const auto& lib) {
            return ffi::cmd_set_metadata(lib, file_id, meta, value.get());
        });
}

tid_t ClientImpl::write_metadata(eng::library_id_t file_id)
{
    return schedule_op([file_id](const auto& lib) {
            return ffi::cmd_write_metadata(lib, file_id);
        });
}

tid_t ClientImpl::moveFileToFolder(eng::library_id_t file_id,
                                   eng::library_id_t from_folder_id,
                                   eng::library_id_t to_folder_id)
{
    return schedule_op([file_id, from_folder_id, to_folder_id](const auto& lib) {
            return ffi::cmd_move_file_to_folder(lib, file_id, from_folder_id, to_folder_id);
        });
}


tid_t ClientImpl::getAllLabels()
{
    return schedule_op(&ffi::cmd_list_all_labels);
}


tid_t ClientImpl::createLabel(const std::string & s, const std::string & colour)
{
    return schedule_op([s, colour](const auto& lib) {
            return ffi::cmd_create_label(lib, s.c_str(), colour.c_str());
        });
}

tid_t ClientImpl::deleteLabel(int label_id)
{
    return schedule_op([label_id](const auto& lib) {
            return ffi::cmd_delete_label(lib, label_id);
        });
}

tid_t ClientImpl::updateLabel(eng::library_id_t label_id,
                              const std::string & new_name,
                              const std::string & new_colour)
{
    return schedule_op([label_id, new_name, new_colour](const auto& lib) {
            return ffi::cmd_update_label(lib, label_id, new_name.c_str(), new_colour.c_str());
        });
}


tid_t ClientImpl::processXmpUpdateQueue(bool write_xmp)
{
    return schedule_op([write_xmp](const auto& lib) {
            return ffi::cmd_process_xmp_update_queue(lib, write_xmp);
        });
}

tid_t ClientImpl::importFile(const std::string & path, Managed manage)
{
    return schedule_op([path, manage](const auto& lib) {
            return ffi::cmd_import_file(lib, path.c_str(), manage);
        });
}

tid_t ClientImpl::importFromDirectory(const std::string & dir, Managed manage)
{
    FileListPtr files;

    files = fwk::FileList::getFilesFromDirectory(dir, &fwk::filter_none);

    return schedule_op([dir, files, manage](const auto& lib) {
            return ffi::cmd_import_files(lib, dir.c_str(), files.get(), manage);
        });
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
