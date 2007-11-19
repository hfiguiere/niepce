/*
 * niepce - library/commands.cpp
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


#include <boost/any.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem/path.hpp>


#include "utils/debug.h"
#include "db/library.h"
#include "db/libfolder.h"
#include "commands.h"

namespace bfs = boost::filesystem;

using boost::any_cast;

using db::Library;
using db::LibFolder;
using utils::FileList;

namespace library {

	bool Commands::dispatch(const Library::Ptr & lib, const Op::Ptr & _op)
	{
		try {
			const Op::Args & args(_op->args());
			switch( _op->type() )
			{
			case OP_IMPORT_FILES:
				cmdImportFiles( lib, any_cast<bfs::path>(args[0]), 
								any_cast<FileList::Ptr>(args[1]), 
								any_cast<bool>(args[2]) );
				break;
			case OP_LIST_ALL_FOLDERS:
				cmdListAllFolders(lib);
				break;
			default:
				DBG_OUT("unkown op");
				break;
			}
		}
		catch( const boost::bad_any_cast & e)
		{
			DBG_OUT("parameter cast error %s", e.what());
			return false;
		}
		return true;
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
		DBG_ASSERT(manage == true, "managing file is currently unsupported");
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
					   bind(&Library::addFile,
							boost::ref(lib), pf->id(),
							_1, manage) );
		lib->notify(Library::NOTIFY_ADDED_FILES, boost::any());
	}



	Op::Ptr Commands::opListAllFolders(tid_t id)
	{
		return Op::Ptr(new Op( OP_LIST_ALL_FOLDERS, id ));
	}

	Op::Ptr Commands::opImportFiles(tid_t id, const bfs::path & folder, 
									const FileList::Ptr & files, bool manage)
	{
		Op::Ptr op(new Op( OP_IMPORT_FILES, id ));

		Op::Args & args(op->args());
		args.push_back( boost::any( folder ));
		args.push_back( boost::any( files ));
		args.push_back( boost::any( manage ));

		return op;
	}


}

