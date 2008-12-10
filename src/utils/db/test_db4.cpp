/*
 * niepce - db/test_db.cpp
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

#include <unistd.h>
#include <sstream>
#include <boost/scoped_ptr.hpp>

#include "utils/buffer.h"
#include "utils/debug.h"

#include "sqlstatement.h"
#include "insertstatement.h"
#include "iconnectiondriver.h"
#include "iconnectionmanagerdriver.h"
#include "sqlite/sqlitecnxmgrdrv.h"

#include <boost/test/minimal.hpp>


//connection_test
int test_main(int, char *[])
{
	try {
		db::IConnectionManagerDriver::Ptr mgr(new db::sqlite::SqliteCnxMgrDrv());
		
		db::DBDesc desc("", 0, "test.db");
		db::IConnectionDriver::Ptr drv(mgr->connect_to_db(desc, "", ""));

		db::SQLStatement schemacreate("CREATE TABLE foo "
									  "(bar TEXT NOT NULL)");
		BOOST_CHECK(drv->execute_statement(schemacreate));
		
		const char * sql = "SELECT * FROM foo WHERE bar='1'";
		db::SQLStatement stmt(sql);
		
		BOOST_CHECK(drv->execute_statement(stmt));
		BOOST_CHECK(drv->should_have_data());
		BOOST_CHECK(unlink("test.db") != -1);
	}
	catch(...) 
	{
		BOOST_CHECK(0);
	}
    return 0;
}

