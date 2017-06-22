/*
 * niepce - library/commands.cpp
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


#include <array>

#include <boost/any.hpp>

#include "fwk/base/debug.hpp"
#include "fwk/utils/pathutils.hpp"
#include "engine/db/library.hpp"
#include "engine/db/libfolder.hpp"
#include "engine/db/libfile.hpp"
#include "engine/db/libmetadata.hpp"
#include "engine/db/filebundle.hpp"
#include "engine/db/keyword.hpp"
#include "engine/db/label.hpp"
#include "commands.hpp"

using fwk::FileList;

namespace eng {

void Commands::cmdListAllKeywords(const Library::Ptr & lib)
{
    KeywordListPtr l(new KeywordList);
    lib->getAllKeywords(l);
    /////
    // notify folder added l
    lib->notify(LibNotification::make<LibNotification::Type::ADDED_KEYWORDS>({l}));
}

void Commands::cmdListAllFolders(const Library::Ptr & lib)
{
    LibFolderListPtr l(new LibFolderList);
    lib->getAllFolders(l);
    /////
    // notify folder added l
    lib->notify(LibNotification::make<LibNotification::Type::ADDED_FOLDERS>({l}));
}

void Commands::cmdImportFile(const Library::Ptr & lib,
                             const std::string & file_path,
                             LibraryManaged manage)
{
    DBG_ASSERT(manage == LibraryManaged::NO,
               "managing file is currently unsupported");

    FileBundlePtr bundle(filebundle_new());
    engine_db_filebundle_add(bundle.get(), file_path.c_str());

    std::string folder = fwk::path_dirname(file_path);

    LibFolderPtr pf = lib->getFolder(folder);
    if(!pf) {
        pf = lib->addFolder(folder);
        LibFolderListPtr l(new LibFolderList);
        l->push_back(pf);
        lib->notify(LibNotification::make<LibNotification::Type::ADDED_FOLDERS>({l}));
    }
    lib->addBundle(engine_db_libfolder_id(pf.get()), bundle, manage);
    lib->notify(LibNotification::make<LibNotification::Type::ADDED_FILES>({}));
}

void Commands::cmdImportFiles(const Library::Ptr & lib,
                              const std::string & folder,
                              const FileList::Ptr & files, LibraryManaged manage)
{
    DBG_ASSERT(manage == LibraryManaged::NO,
               "managing file is currently unsupported");

    FileBundleListPtr bundles = filebundle_filter_bundles(files);

    LibFolderPtr pf = lib->getFolder(folder);
    if(!pf) {
        pf = lib->addFolder(folder);
        LibFolderListPtr l(new LibFolderList);
        l->push_back(pf);
        lib->notify(LibNotification::make<LibNotification::Type::ADDED_FOLDERS>({l}));
    }
    std::for_each(bundles->begin(), bundles->end(),
                  [&lib, &pf, manage] (const auto& fb) {
                      lib->addBundle(engine_db_libfolder_id(pf.get()), fb, manage);
                  });
    lib->notify(LibNotification::make<LibNotification::Type::ADDED_FILES>({}));
}


void Commands::cmdQueryFolderContent(const Library::Ptr & lib,
                                     library_id_t folder_id)
{
    LibFileListPtr fl(new LibFileList);
    lib->getFolderContent(folder_id, fl);
    lib->notify(LibNotification::make<LibNotification::Type::FOLDER_CONTENT_QUERIED>(
                    {folder_id, fl}));
}

void Commands::cmdCountFolder(const Library::Ptr & lib,
                              eng::library_id_t folder_id)
{
    int count = lib->countFolder(folder_id);
    lib->notify(LibNotification::make<LibNotification::Type::FOLDER_COUNTED>(
                    {folder_id, count}));
}

void Commands::cmdQueryKeywordContent(const Library::Ptr & lib,
                                      library_id_t keyword_id)
{
    LibFileListPtr fl(new LibFileList);
    lib->getKeywordContent(keyword_id, fl);
    lib->notify(LibNotification::make<LibNotification::Type::KEYWORD_CONTENT_QUERIED>(
                    {keyword_id, fl}));
}

void Commands::cmdRequestMetadata(const Library::Ptr & lib,
                                  library_id_t file_id)
{
    LibMetadata::Ptr lm(new LibMetadata(file_id));
    lib->getMetaData(file_id, lm);
    lib->notify(LibNotification::make<LibNotification::Type::METADATA_QUERIED>(
                    {file_id, lm}));
}

void Commands::cmdSetMetadata(const Library::Ptr & lib,
                              library_id_t file_id, fwk::PropertyIndex meta,
                              const fwk::PropertyValue & value)
{
    metadata_desc_t m;
    m.id = file_id;
    m.meta = meta;
    m.value = value;
    lib->setMetaData(file_id, meta, value);
    lib->notify(LibNotification::make<LibNotification::Type::METADATA_CHANGED>(
                    std::move(m)));
}

void Commands::cmdWriteMetadata(const Library::Ptr & lib,
                                 library_id_t file_id)
{
    lib->writeMetaData(file_id);
}

void Commands::cmdMoveFileToFolder(const Library::Ptr & lib,
                                   library_id_t file_id, library_id_t from_folder_id,
                                   library_id_t to_folder_id)
{
    if(lib->moveFileToFolder(file_id, to_folder_id)) {
        lib->notify(LibNotification::make<LibNotification::Type::FILE_MOVED>(
                        {file_id, from_folder_id, to_folder_id}));
        lib->notify(LibNotification::make<LibNotification::Type::FOLDER_COUNT_CHANGE>(
                        {from_folder_id, -1}));
        lib->notify(LibNotification::make<LibNotification::Type::FOLDER_COUNT_CHANGE>(
                        {to_folder_id, 1}));
    }
}

void Commands::cmdListAllLabels(const Library::Ptr & lib)
{
    eng::Label::ListPtr l(new eng::Label::List);
    lib->getAllLabels(l);
    lib->notify(LibNotification::make<LibNotification::Type::ADDED_LABELS>({l}));
}

void Commands::cmdCreateLabel(const Library::Ptr & lib,
                              const std::string & s, const std::string & color)
{
    int64_t id = lib->addLabel(s, color);
    if(id != -1) {
        eng::Label::ListPtr l(new eng::Label::List);
        l->push_back(eng::Label::Ptr(new eng::Label(id, s, color)));
        lib->notify(LibNotification::make<LibNotification::Type::ADDED_LABELS>({l}));
    }
}


void Commands::cmdDeleteLabel(const Library::Ptr & lib,
                              int label_id)
{
    lib->deleteLabel(label_id);
    lib->notify(LibNotification::make<LibNotification::Type::LABEL_DELETED>({label_id}));
}


void Commands::cmdUpdateLabel(const Library::Ptr & lib,
                              eng::library_id_t label_id, const std::string & name,
                              const std::string & color)
{
    lib->updateLabel(label_id, name, color);
    eng::Label::Ptr label(new eng::Label(label_id, name, color));
    lib->notify(LibNotification::make<LibNotification::Type::LABEL_CHANGED>({label}));
}


void Commands::cmdProcessXmpUpdateQueue(const Library::Ptr & lib, bool write_xmp)
{
    lib->processXmpUpdateQueue(write_xmp);
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

