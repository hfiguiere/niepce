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
#include <boost/filesystem/convenience.hpp>
#include <boost/algorithm/string.hpp>

#include "debug.hpp"
#include "files.hpp"

namespace bfs = boost::filesystem;

namespace utils {

	bool filter_none(const boost::filesystem::path & )
	{
		return true;
	}


	bool filter_xmp_out(const bfs::path & file)
	{
		std::string ext = extension(file);
		boost::to_lower(ext);
		if(ext == ".xmp") {
			return false;
		}
		return true;
	}


	FileList::FileList( const _impltype_t & v )
		: _impltype_t( v )
	{
	}

	FileList::Ptr FileList::getFilesFromDirectory(const FileList::value_type & p, boost::function<bool (const value_type &)> filter)
	{
		if(!exists( p ) ) {
			DBG_OUT( "directory %s do not exist", p.string().c_str() );
			return Ptr();
		}
		try
		{
			FileList::Ptr l( new FileList() );
			
			bfs::directory_iterator end_itr; 
			for ( bfs::directory_iterator itr( p );
				  itr != end_itr;
				  ++itr )
			{
				if ( !is_directory(*itr) )
				{
					if( filter(*itr) ) {
						l->push_back(*itr);
						DBG_OUT( "found file %s", itr->string().c_str() );
					}
				}
			}
			l->sort();
			return l;
		}
		catch( std::exception & e )
		{
			ERR_OUT( "Exception: %s", e.what() );
		}

		return Ptr();
	}


}

