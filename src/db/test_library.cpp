/*
 * niepce - db/test_library.cpp
 *
 * Copyright (C) 2007 Hubert Figuiere
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


#include "sqlstatement.h"
#include "iconnectiondriver.h"
#include "library.h"

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>


BOOST_AUTO_UNIT_TEST(library_test)
{
	db::Library lib("./");

	BOOST_CHECK_EQUAL(lib.checkDatabaseVersion(), 1);

	db::IConnectionDriver::Ptr db(lib.dbDriver());
	
//	db::SQLStatement check("SELECT ");
//	db->


	lib.addFolder("foo");
	BOOST_CHECK(lib.getFolder("foo") != -1);

	BOOST_CHECK(unlink(lib.dbName().c_str()) != -1);
}
