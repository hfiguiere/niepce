/*
 * niepce - library/commands.cpp
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


#include <tr1/array>

#include <boost/any.hpp>
#include <boost/bind.hpp>


#include "fwk/utils/boost.hpp"
#include "fwk/base/debug.hpp"
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
                              const std::string & folder, 
                              const FileList::Ptr & files, bool manage)
{
    DBG_ASSERT(!manage, "managing file is currently unsupported");

    FileBundle::ListPtr bundles = FileBundle::filter_bundles(files);

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
    std::for_each( bundles->begin(), bundles->end(),
                   bind(&Library::addBundle, boost::ref(lib),
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

void Commands::cmdCountFolder(const Library::Ptr & lib, 
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

void Commands::cmdRequestMetadata(const Library::Ptr & lib,
                                  int file_id)
{
    LibMetadata::Ptr lm(new LibMetadata(file_id));
    lib->getMetaData(file_id, lm);
    lib->notify(Library::NOTIFY_METADATA_QUERIED, boost::any(lm));
}

void Commands::cmdSetMetadata(const Library::Ptr & lib,
                              int file_id, int meta, int value)
{
    std::tr1::array<int, 3> m;
    m[0] = file_id;
    m[1] = meta;
    m[2] = value;
    lib->setMetaData(file_id, meta, value);
    lib->notify(Library::NOTIFY_METADATA_CHANGED, boost::any(m));
}

void Commands::cmdListAllLabels(const Library::Ptr & lib)
{
    eng::Label::ListPtr l(new eng::Label::List);
    lib->getAllLabels(l);
    lib->notify(Library::NOTIFY_ADDED_LABELS, boost::any(l));
}

void Commands::cmdCreateLabel(const Library::Ptr & lib,
                              const std::string & s, const std::string & color)
{
    int id = lib->addLabel(s, color);
    if(id != -1) {
        eng::Label::ListPtr l(new eng::Label::List);
        l->push_back(eng::Label::Ptr(new eng::Label(id, s, color)));
        lib->notify(Library::NOTIFY_ADDED_LABELS, boost::any(l));
    }
}


void Commands::cmdDeleteLabel(const Library::Ptr & lib,
                              int label_id)
{
    lib->deleteLabel(label_id);
    lib->notify(Library::NOTIFY_LABEL_DELETED, boost::any(label_id));
}


void Commands::cmdUpdateLabel(const Library::Ptr & lib,
                              int label_id, const std::string & name,
                              const std::string & color)
{
    lib->updateLabel(label_id, name, color);
    eng::Label::Ptr label(new eng::Label(label_id, name, color));
    lib->notify(Library::NOTIFY_LABEL_CHANGED, boost::any(label));
}


void Commands::cmdProcessXmpUpdateQueue(const Library::Ptr & lib)
{
    lib->processXmpUpdateQueue();
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

