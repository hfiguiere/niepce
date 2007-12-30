/*
 * niepce - utils/testfiles.cpp
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
/** @brief unit test for files */

#include <boost/test/minimal.hpp>
#include <boost/bind.hpp>

#include <stdlib.h>

#include "files.h"

using utils::FileList;

int test_main( int, char *[] )             // note the name!
{
	system( "mkdir -p AAtest/sub" );
	system( "touch AAtest/1" );
	system( "touch AAtest/2" );
	system( "touch AAtest/3" );

	FileList::Ptr files;
	
	files = FileList::getFilesFromDirectory( "foo", boost::bind(utils::filter_none, _1) );
	BOOST_CHECK( !files );

	files = FileList::getFilesFromDirectory( "AAtest", boost::bind(utils::filter_none, _1));
	BOOST_CHECK( files );
	BOOST_CHECK( files->size() == 3 );
	
	system( "rm -fr AAtest" );
	return 0;
}

