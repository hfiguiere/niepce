/*
 * niepce - utils/files.cpp
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


#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include "debug.h"
#include "files.h"

namespace bfs = boost::filesystem;

namespace utils {

	FileList::FileList( const _impltype_t & v )
		: _impltype_t( v )
	{
	}

	FileList::Ptr FileList::getFilesFromDirectory(const FileList::value_type & dir)
	{

		bfs::path p( dir );
		if(!exists( p ) ) {
			DBG_OUT( "directory %s do not exist", dir.c_str() );
			return Ptr();
		}
		
		FileList::Ptr l( new FileList() );
		
		bfs::directory_iterator end_itr; 
		for ( bfs::directory_iterator itr( p );
			  itr != end_itr;
			  ++itr )
		{
			if ( !is_directory(*itr) )
			{
				l->push_back(itr->leaf());
				DBG_OUT("found file %s", itr->leaf().c_str());
			}
		}
		return l;
	}


}

