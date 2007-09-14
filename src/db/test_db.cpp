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

#include "sqlstatement.h"
#include "insertstatement.h"
#include "iconnectiondriver.h"
#include "iconnectionmanagerdriver.h"
#include "sqlite/sqlitecnxmgrdrv.h"

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>


BOOST_AUTO_UNIT_TEST(sqlstatement_test)
{
	BOOST_CHECK(1);

	const char * sql = "SELECT * FROM foo WHERE bar='1'";
	db::SQLStatement stmt(sql);

	BOOST_CHECK_EQUAL(stmt.to_string(), sql);

	db::SQLStatement stmt2 = stmt;

	BOOST_CHECK_EQUAL(stmt2.to_string(), sql);
	
	BOOST_CHECK_EQUAL(db::SQLStatement::escape_string("d'oh"), "d''oh");

	std::ostringstream ss;
	ss << stmt;
	BOOST_CHECK_EQUAL(ss.str(), sql);
}

BOOST_AUTO_UNIT_TEST(insertstatement_test)
{
	db::ColumnList columns(2);

	db::Column &c0(columns[0]);
	c0.set_name("id");
	c0.set_value(boost::lexical_cast<std::string>(1));
	c0.set_auto_increment(true);

	db::Column &c1(columns[1]);
	c1.set_name("name");
	c1.set_value("foo");

	db::InsertStatement stmt("table", columns);
	
	std::string s = stmt.to_string();
	BOOST_CHECK(s.size() != 0); 

	BOOST_CHECK_EQUAL(s, "INSERT INTO table( id, name) VALUES (null, 'foo')");
}



BOOST_AUTO_UNIT_TEST(connection_test)
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

}

