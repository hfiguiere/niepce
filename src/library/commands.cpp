/*
 * niepce - library/commands.cpp
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


#include <boost/any.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <boost/filesystem/path.hpp>


#include "utils/debug.h"
#include "db/library.h"
#include "db/libfolder.h"
#include "db/libfile.h"
#include "db/libmetadata.h"
#include "db/keyword.h"
#include "commands.h"

namespace bfs = boost::filesystem;

using boost::any_cast;

using db::Library;
using db::LibFolder;
using db::LibFile;
using db::LibMetadata;
using db::Keyword;
using utils::FileList;

namespace library {

void Commands::cmdListAllKeywords(const Library::Ptr & lib)
{
    Keyword::ListPtr l( new Keyword::List );
    lib->getAllKeywords( l );
    /////
    // notify folder added l
    lib->notify(Library::NOTIFY_ADDED_KEYWORDS, boost::any(l));
}

void Commands::cmdListAllFolders(const Library::Ptr & lib)
{
    LibFolder::ListPtr l( new LibFolder::List );
    lib->getAllFolders( l );
    /////
    // notify folder added l
    lib->notify(Library::NOTIFY_ADDED_FOLDERS, boost::any(l));
}
	
void Commands::cmdImportFiles(const Library::Ptr & lib, 
                              const bfs::path & folder, 
                              const FileList::Ptr & files, bool manage)
{
    DBG_ASSERT(!manage, "managing file is currently unsupported");
    LibFolder::Ptr pf;
    pf = lib->getFolder(folder);
    if(pf == NULL)
    {
        pf = lib->addFolder(folder);
        LibFolder::ListPtr l( new LibFolder::List );
        l->push_back(pf);
        lib->notify(Library::NOTIFY_ADDED_FOLDERS,
                    boost::any(l));
    }
    std::for_each( files->begin(), files->end(),
                   bind(&Library::addFile, boost::ref(lib),
                        pf->id(), _1, manage) );
    lib->notify(Library::NOTIFY_ADDED_FILES,
                boost::any()); 
}


void Commands::cmdQueryFolderContent(const Library::Ptr & lib, 
                                     int folder_id)
{
    LibFile::ListPtr fl(new LibFile::List());
    lib->getFolderContent(folder_id, fl);
    lib->notify(Library::NOTIFY_FOLDER_CONTENT_QUERIED, boost::any(fl));		
}

void Commands::cmdCountFolder(const db::Library::Ptr & lib, 
                              int folder_id)
{
    int count = lib->countFolder(folder_id);
    lib->notify(Library::NOTIFY_FOLDER_COUNTED, boost::any(std::make_pair(folder_id, count)));
}

void Commands::cmdQueryKeywordContent(const Library::Ptr & lib, 
                                      int keyword_id)
{
    LibFile::ListPtr fl(new LibFile::List());
    lib->getKeywordContent(keyword_id, fl);
    lib->notify(Library::NOTIFY_KEYWORD_CONTENT_QUERIED, boost::any(fl));		
}

void Commands::cmdRequestMetadata(const db::Library::Ptr & lib,
                                  int file_id)
{
    LibMetadata::Ptr lm(new LibMetadata(file_id));
    lib->getMetaData(file_id, lm);
    lib->notify(Library::NOTIFY_METADATA_QUERIED, boost::any(lm));
}

void Commands::cmdSetMetadata(const db::Library::Ptr & lib,
                              int file_id, int meta, int value)
{
    boost::array<int, 3> m;
    m[0] = file_id;
    m[1] = meta;
    m[2] = value;
    lib->setMetaData(file_id, meta, value);
    lib->notify(Library::NOTIFY_METADATA_CHANGED, boost::any(m));
}


/////////////////  ops generation
Op::Ptr Commands::opListAllFolders(tid_t id)
{
    Op::Ptr op(new Op(id));
    op->fn() = boost::bind(&Commands::cmdListAllFolders, _1);
    return op;
}

Op::Ptr Commands::opListAllKeywords(tid_t id)
{
    Op::Ptr op(new Op(id));
    op->fn() = boost::bind(&Commands::cmdListAllKeywords, _1);
    return op;
}

Op::Ptr Commands::opImportFiles(tid_t id, const bfs::path & folder, 
                                const FileList::Ptr & files, bool manage)
{
    Op::Ptr op(new Op(id));
    op->fn() = boost::bind(&Commands::cmdImportFiles,
                           _1, folder, files, manage);
    return op;
}

Op::Ptr Commands::opQueryFolderContent(tid_t id, int folder_id)
{
    Op::Ptr op(new Op(id));
    op->fn() = boost::bind(&Commands::cmdQueryFolderContent,
                           _1, folder_id);
    return op;
}

Op::Ptr Commands::opCountFolder(tid_t id, int folder_id)
{
    Op::Ptr op(new Op(id));
    op->fn() = boost::bind(&Commands::cmdCountFolder, _1, folder_id);
    return op;
}

Op::Ptr Commands::opQueryKeywordContent(tid_t id, int keyword_id)
{
		
    Op::Ptr op(new Op(id));
    op->fn() = boost::bind(&Commands::cmdQueryKeywordContent,
                           _1, keyword_id);
    return op;
}

Op::Ptr Commands::opRequestMetadata(tid_t id, int file_id)
{
    Op::Ptr op(new Op(id));
    op->fn() = boost::bind(&Commands::cmdRequestMetadata,
                           _1, file_id);
    return op;
}


Op::Ptr Commands::opSetMetadata(tid_t id, int file_id, int meta, int value)
{
    Op::Ptr op(new Op(id));
    op->fn() = boost::bind(&Commands::cmdSetMetadata, _1,
                           file_id, meta, value);
    return op;
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

