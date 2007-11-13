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
#include <boost/bind/apply.hpp>

#include "utils/debug.h"
#include "commands.h"
#include "db/library.h"

using boost::any_cast;

using db::Library;
using utils::FileList;

namespace library {

	bool Commands::dispatch(const Library::Ptr & lib, const Op::Ptr & _op)
	{
		switch( _op->type() )
		{
		case OP_IMPORT_FILES:
		{
			const Op::Args & args(_op->args());
			try {
				cmdImportFiles( lib, any_cast<std::string>(args[0]), 
								any_cast<FileList::Ptr>(args[1]), 
								any_cast<bool>(args[2]) );
			}
			catch( const boost::bad_any_cast & e)
			{
				DBG_OUT("parameter cast error %s", e.what());
				return false;
			}
			break;
		}
		default:
			break;
		}
		return false;
	}

	
	void Commands::cmdImportFiles(const Library::Ptr & lib, const std::string & folder, 
								  const FileList::Ptr & files, bool manage)
	{
		DBG_ASSERT(manage == true, "managing file is currently unsupported");
		int folder_id = lib->getFolder(folder);
		if(folder_id == -1)
		{
			folder_id = lib->addFolder(folder);
		}
		std::for_each( files->begin(), files->end(), 
					   bind(&Library::addFile,
							boost::ref(lib), folder_id,
							_1, manage) );
	}



	Op::Ptr Commands::opImportFiles(const std::string & folder, 
									const FileList::Ptr & files, bool manage)
	{
		Op::Ptr op(new Op( OP_IMPORT_FILES ));

		Op::Args & args(op->args());
		args.push_back( boost::any( folder ));
		args.push_back( boost::any( files ));
		args.push_back( boost::any( manage ));

		return op;
	}


}

