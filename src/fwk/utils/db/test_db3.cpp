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

#include "fwk/utils/buffer.h"
#include "fwk/utils/debug.h"

#include "sqlstatement.h"
#include "insertstatement.h"
#include "iconnectiondriver.h"
#include "iconnectionmanagerdriver.h"
#include "sqlite/sqlitecnxmgrdrv.h"

#include <boost/test/minimal.hpp>



//insertblob_test
int test_main(int, char *[])
{
	try {
		db::IConnectionManagerDriver::Ptr mgr(new db::sqlite::SqliteCnxMgrDrv());
		
		db::DBDesc desc("", 0, "test.db");
		db::IConnectionDriver::Ptr drv(mgr->connect_to_db(desc, "", ""));

		db::SQLStatement schemacreate("CREATE TABLE foo "
									  "(bar TEXT NOT NULL, "
									  "baz BLOB)");
		BOOST_CHECK(drv->execute_statement(schemacreate));

		
		const char * sql = "INSERT INTO foo (bar, baz) VALUES(?1, ?2);";
		db::SQLStatement stmt(sql);
		std::string s("zaphod");
		utils::Buffer b("beeblebrox'", 11);
		BOOST_CHECK(stmt.bind(1, s));
		BOOST_CHECK(stmt.bind(2, b));
		
		BOOST_CHECK(drv->execute_statement(stmt));
		BOOST_CHECK(!drv->should_have_data());

		sql = "SELECT bar, baz FROM foo";
		db::SQLStatement stmt2( sql );
		BOOST_CHECK(drv->execute_statement(stmt2));
		BOOST_CHECK(drv->should_have_data());
		BOOST_CHECK(drv->read_next_row());
		std::string s2;
		utils::Buffer b2;
		BOOST_CHECK(drv->get_column_content(0, s2));
		BOOST_CHECK(s == s2);
		BOOST_CHECK(drv->get_column_content(1, b2));
		BOOST_CHECK(b.get_len() == b2.get_len());
		BOOST_CHECK(std::string(b2.get_data()) == "beeblebrox'");

		BOOST_CHECK(unlink("test.db") != -1);
	}
	catch(...) 
	{
		BOOST_CHECK(0);
	}
    return 0;
}

