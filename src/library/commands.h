/*
 * niepce - library/commands.h
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



#ifndef __LIBRARY_COMMANDS_H__
#define __LIBRARY_COMMANDS_H__

#include "op.h"
#include "utils/files.h"
#include "db/library.h"

namespace library {

	/** Marshalling and demarshalling of commands ops */ 
	class Commands 
	{
	public:

		// commands: execute an op
//		static void cmdQueryFiles(const db::Library::Ptr & lib);
//		static void cmdUpdateFiles(const db::Library::Ptr & lib);

		static void cmdListAllFolders(const db::Library::Ptr & lib);
		static void cmdListAllKeywords(const db::Library::Ptr & lib);
		static void cmdImportFiles(const db::Library::Ptr & lib, 
								   const boost::filesystem::path & folder, 
								   const utils::FileList::Ptr & files, 
								   bool manage);
		static void cmdQueryFolderContent(const db::Library::Ptr & lib, 
										  int folder_id);
		static void cmdCountFolder(const db::Library::Ptr & lib, 
								   int folder_id);
		static void cmdQueryKeywordContent(const db::Library::Ptr & lib, 
										   int keyword_id);
		static void cmdRequestMetadata(const db::Library::Ptr & lib,
									   int file_id);
        static void cmdSetMetadata(const db::Library::Ptr & lib,
                                   int file_id, int meta, int value);
		static void cmdProcessXmpUpdateQueue(const db::Library::Ptr & lib);

		// op: create an op
		static Op::Ptr opListAllFolders(tid_t id);
		static Op::Ptr opListAllKeywords(tid_t id);
		static Op::Ptr opImportFiles(tid_t id, 
									 const boost::filesystem::path & folder, 
									 const utils::FileList::Ptr & files, 
									 bool manage);
 		static Op::Ptr opQueryFolderContent(tid_t id, int folder_id);
		static Op::Ptr opCountFolder(tid_t id, int folder_id);
 		static Op::Ptr opQueryKeywordContent(tid_t id, int keyword_id);
		static Op::Ptr opRequestMetadata(tid_t id, int file_id);
        static Op::Ptr opSetMetadata(tid_t id, int file_id, int meta, int value);
        static Op::Ptr opProcessXmpUpdateQueue(tid_t id);
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
