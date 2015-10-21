/*
 * niepce - libraryclient/clientimpl.cpp
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

#include <functional>

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
using std::placeholders::_1;

namespace libraryclient {

ClientImpl *ClientImpl::makeClientImpl(const fwk::Moniker & moniker,
                                       const fwk::NotificationCenter::Ptr & nc)
{
    return new ClientImpl(moniker, nc);
}

ClientImpl::ClientImpl(const fwk::Moniker & moniker, const fwk::NotificationCenter::Ptr & nc)
    : m_moniker(moniker),
      m_localLibrary(nullptr)
{
    DBG_OUT("creating implementation with moniker %s",
            moniker.c_str());
    m_localLibrary = new LocalLibraryServer(moniker.path(), nc);
}

ClientImpl::~ClientImpl()
{
    delete m_localLibrary;
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
    return schedule_op(std::bind(&Commands::cmdQueryFolderContent,
                                 _1, folder_id));
}


tid_t ClientImpl::countFolder(eng::library_id_t folder_id)
{
    return schedule_op(std::bind(&Commands::cmdCountFolder,
                                 _1, folder_id));
}


tid_t ClientImpl::queryKeywordContent(eng::library_id_t keyword_id)
{
    return schedule_op(std::bind(&Commands::cmdQueryKeywordContent,
                                 _1, keyword_id));
}


tid_t ClientImpl::requestMetadata(eng::library_id_t file_id)
{
    return schedule_op(std::bind(&Commands::cmdRequestMetadata,
                                 _1, file_id));
}


tid_t ClientImpl::setMetadata(eng::library_id_t file_id, int meta,
                              const fwk::PropertyValue & value)
{
    return schedule_op(std::bind(&Commands::cmdSetMetadata, _1,
                                 file_id, meta, value));
}

tid_t ClientImpl::write_metadata(eng::library_id_t file_id)
{
    return schedule_op(std::bind(&Commands::cmdWriteMetadata, _1,
                                 file_id));
}

tid_t ClientImpl::moveFileToFolder(eng::library_id_t file_id,
                                   eng::library_id_t from_folder_id,
                                   eng::library_id_t to_folder_id)
{
    return schedule_op(std::bind(&Commands::cmdMoveFileToFolder, _1,
                                 file_id, from_folder_id, to_folder_id));
}


tid_t ClientImpl::getAllLabels()
{
    return schedule_op(&Commands::cmdListAllLabels);
}


tid_t ClientImpl::createLabel(const std::string & s, const std::string & colour)
{
    return schedule_op(std::bind(&Commands::cmdCreateLabel,
                                 _1, s, colour));
}

tid_t ClientImpl::deleteLabel(int label_id)
{
    return schedule_op(std::bind(&Commands::cmdDeleteLabel,
                                 _1, label_id));
}

tid_t ClientImpl::updateLabel(eng::library_id_t label_id,
                              const std::string & new_name,
                              const std::string & new_colour)
{
    return schedule_op(std::bind(&Commands::cmdUpdateLabel,
                                 _1, label_id, new_name, new_colour));
}


tid_t ClientImpl::processXmpUpdateQueue(bool write_xmp)
{
    return schedule_op(std::bind(&Commands::cmdProcessXmpUpdateQueue,
                                 _1, write_xmp));
}

tid_t ClientImpl::importFile(const std::string & path, bool manage)
{
    return schedule_op(std::bind(&Commands::cmdImportFile,
                                 _1, path, manage));
}

tid_t ClientImpl::importFromDirectory(const std::string & dir, bool manage)
{
    FileList::Ptr files;

    files = FileList::getFilesFromDirectory(dir, &fwk::filter_none);

    return schedule_op(std::bind(&Commands::cmdImportFiles,
                                 _1, dir, files, manage));
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
