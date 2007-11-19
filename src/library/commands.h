/*
 * niepce - library/commands.h
 *
 * Copyright (C) 2007 Hubert Figuiere
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

namespace boost {
	namespace filesystem {
		class path;
	}
}

namespace library {

	/** Marshalling and demarshalling of commands ops */ 
	class Commands 
	{
	public:

		static bool dispatch(const db::Library::Ptr & lib, const Op::Ptr & _op);

		// commands: execute an op
		static void cmdQueryFiles(const db::Library::Ptr & lib);
		static void cmdUpdateFiles(const db::Library::Ptr & lib);

		static void cmdListAllFolders(const db::Library::Ptr & lib);
		static void cmdImportFiles(const db::Library::Ptr & lib, 
								   const boost::filesystem::path & folder, 
								   const utils::FileList::Ptr & files, bool manage);
		

		// op: create an op
		static Op::Ptr opListAllFolders(tid_t id);
		static Op::Ptr opImportFiles(tid_t id, const boost::filesystem::path & folder, 
									 const utils::FileList::Ptr & files, bool manage);
	};

}


#endif
