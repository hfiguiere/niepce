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
#include "engine/library/commands.hpp"
#include "libraryclient.hpp"
#include "clientimpl.hpp"
#include "locallibraryserver.hpp"

using fwk::FileList;
using eng::Op;
using eng::Commands;
using eng::Library;
using eng::tid_t;

namespace libraryclient {

std::unique_ptr<ClientImpl> ClientImpl::makeClientImpl(const fwk::Moniker & moniker,
                                                       const fwk::NotificationCenter::Ptr & nc)
{
    return std::unique_ptr<ClientImpl>(new ClientImpl(moniker, nc));
}

ClientImpl::ClientImpl(const fwk::Moniker & moniker, const fwk::NotificationCenter::Ptr & nc)
    : m_moniker(moniker),
      m_localLibrary(new LocalLibraryServer(moniker.path(), nc))
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
tid_t ClientImpl::schedule_op(const Op::function_t & func)
{
    tid_t id = LibraryClient::newTid();
    Op::Ptr op(new Op(id, func));
    m_localLibrary->schedule(op);
    return id;
}

tid_t ClientImpl::getAllKeywords()
{
    return schedule_op(&Commands::cmdListAllKeywords);
}


tid_t ClientImpl::getAllFolders()
{
    return schedule_op(&Commands::cmdListAllFolders);
}

tid_t ClientImpl::queryFolderContent(eng::library_id_t folder_id)
{
    return schedule_op([folder_id](const auto& lib) {
        Commands::cmdQueryFolderContent(lib, folder_id);
    });
}


tid_t ClientImpl::countFolder(eng::library_id_t folder_id)
{
    return schedule_op([folder_id](const auto& lib) {
        Commands::cmdCountFolder(lib, folder_id);
    });
}


tid_t ClientImpl::queryKeywordContent(eng::library_id_t keyword_id)
{
    return schedule_op([keyword_id](const auto& lib) {
        Commands::cmdQueryKeywordContent(lib, keyword_id);
    });
}


tid_t ClientImpl::requestMetadata(eng::library_id_t file_id)
{
    return schedule_op([file_id](const auto& lib) {
        Commands::cmdRequestMetadata(lib, file_id);
    });
}


tid_t ClientImpl::setMetadata(eng::library_id_t file_id, int meta,
                              const fwk::PropertyValue & value)
{
    return schedule_op([file_id, meta, value](const auto& lib) {
        Commands::cmdSetMetadata(lib, file_id, meta, value);
    });
}

tid_t ClientImpl::write_metadata(eng::library_id_t file_id)
{
    return schedule_op([file_id](const auto& lib) {
        Commands::cmdWriteMetadata(lib, file_id);
    });
}

tid_t ClientImpl::moveFileToFolder(eng::library_id_t file_id,
                                   eng::library_id_t from_folder_id,
                                   eng::library_id_t to_folder_id)
{
    return schedule_op([file_id, from_folder_id, to_folder_id](const auto& lib) {
        Commands::cmdMoveFileToFolder(lib, file_id, from_folder_id, to_folder_id);
    });
}


tid_t ClientImpl::getAllLabels()
{
    return schedule_op(&Commands::cmdListAllLabels);
}


tid_t ClientImpl::createLabel(const std::string & s, const std::string & colour)
{
    return schedule_op([s, colour](const auto& lib) {
        Commands::cmdCreateLabel(lib, s, colour);
    });
}

tid_t ClientImpl::deleteLabel(int label_id)
{
    return schedule_op([label_id](const auto& lib) {
        Commands::cmdDeleteLabel(lib, label_id);
    });
}

tid_t ClientImpl::updateLabel(eng::library_id_t label_id,
                              const std::string & new_name,
                              const std::string & new_colour)
{
    return schedule_op([label_id, new_name, new_colour](const auto& lib) {
        Commands::cmdUpdateLabel(lib, label_id, new_name, new_colour);
    });
}


tid_t ClientImpl::processXmpUpdateQueue(bool write_xmp)
{
    return schedule_op([write_xmp](const auto& lib) {
        Commands::cmdProcessXmpUpdateQueue(lib, write_xmp);
    });
}

tid_t ClientImpl::importFile(const std::string & path, Library::Managed manage)
{
    return schedule_op([path, manage](const auto& lib) {
        Commands::cmdImportFile(lib, path, manage);
    });
}

tid_t ClientImpl::importFromDirectory(const std::string & dir, Library::Managed manage)
{
    FileList::Ptr files;

    files = FileList::getFilesFromDirectory(dir, &fwk::filter_none);

    return schedule_op([dir, files, manage](const auto& lib) {
        Commands::cmdImportFiles(lib, dir, files, manage);
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
