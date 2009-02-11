/*
 * niepce - db/test_library.cpp
 *
 * Copyright (C) 2007-2009 Hubert Figuiere
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  
 * 02110-1301, USA
 */


#include "fwk/utils/db/sqlstatement.h"
#include "fwk/utils/db/iconnectiondriver.h"
#include "library.h"
#include "libfile.h"
#include "libfolder.h"

#include <boost/test/minimal.hpp>


//BOOST_AUTO_TEST_CASE(library_test)
int test_main(int, char *[])
{
	db::Library lib("./", framework::NotificationCenter::Ptr());

	BOOST_CHECK(lib.checkDatabaseVersion() == DB_SCHEMA_VERSION);

	db::IConnectionDriver::Ptr db(lib.dbDriver());
	
    db::LibFolder::Ptr folder_added(lib.addFolder("foo"));
    BOOST_CHECK(folder_added);
    BOOST_CHECK(folder_added->id() > 0);
	db::LibFolder::Ptr f(lib.getFolder("foo"));
	BOOST_CHECK(f);
    BOOST_CHECK(f->id() == folder_added->id());
	lib.addFolder("bar");
	BOOST_CHECK(lib.getFolder("bar"));

	db::LibFolder::ListPtr l( new db::LibFolder::List );
	lib.getAllFolders( l );
	BOOST_CHECK( l->size() == 2 );

    int file_id = lib.addFile(folder_added->id(), "foo/myfile", false);
    BOOST_CHECK(file_id > 0);

	BOOST_CHECK(unlink(lib.dbName().string().c_str()) != -1);
	return 0;
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
