/*
 * niepce - engine/library/commands.hpp
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



#ifndef __LIBRARY_COMMANDS_H__
#define __LIBRARY_COMMANDS_H__

#include "op.hpp"
#include "fwk/utils/files.hpp"
#include "engine/db/library.hpp"

namespace eng {

/** Marshalling and demarshalling of commands ops */ 
class Commands 
{
public:

		// commands: execute an op
//		static void cmdQueryFiles(const Library::Ptr & lib);
//		static void cmdUpdateFiles(const Library::Ptr & lib);

		static void cmdListAllFolders(const Library::Ptr & lib);
		static void cmdListAllKeywords(const Library::Ptr & lib);
    static void cmdImportFile(const Library::Ptr & lib,
                              const std::string & path,
                              LibraryManaged manage);
    static void cmdImportFiles(const Library::Ptr & lib,
                               const std::string & folder,
                               const fwk::FileList::Ptr & files,
                               LibraryManaged manage);
		static void cmdQueryFolderContent(const Library::Ptr & lib, 
                                      library_id_t folder_id);
		static void cmdCountFolder(const Library::Ptr & lib, 
								   library_id_t folder_id);
		static void cmdQueryKeywordContent(const Library::Ptr & lib, 
                                       library_id_t keyword_id);
		static void cmdRequestMetadata(const Library::Ptr & lib,
                                   library_id_t file_id);
    static void cmdSetMetadata(const Library::Ptr & lib,
                               library_id_t file_id, fwk::PropertyIndex meta, 
                               const fwk::PropertyValue & value);
    static void cmdWriteMetadata(const Library::Ptr & lib,
                                 library_id_t file_id);
    static void cmdMoveFileToFolder(const Library::Ptr & lib, 
                                    library_id_t file_id, library_id_t from_folder_id,
                                    library_id_t to_folder_id);
    static void cmdListAllLabels(const Library::Ptr & lib);
    static void cmdCreateLabel(const Library::Ptr & lib, const std::string & s, 
                               const std::string & color);
    static void cmdDeleteLabel(const Library::Ptr & lib,
                               int label_id);
    static void cmdUpdateLabel(const Library::Ptr & lib,
                               eng::library_id_t label_id, const std::string & name,
                               const std::string & color);
    static void cmdProcessXmpUpdateQueue(const Library::Ptr & lib, bool write_xmp);
};

}


#endif
/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
