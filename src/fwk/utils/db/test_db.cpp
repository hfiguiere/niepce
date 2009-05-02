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

#include "fwk/utils/buffer.hpp"
#include "fwk/utils/debug.hpp"

#include "sqlstatement.hpp"
#include "insertstatement.hpp"
#include "iconnectiondriver.hpp"
#include "iconnectionmanagerdriver.hpp"
#include "sqlite/sqlitecnxmgrdrv.hpp"

#include <boost/test/minimal.hpp>


//sqlstatement_test
int test_main(int, char *[])
{
	BOOST_CHECK(1);

	const char * sql = "SELECT * FROM foo WHERE bar='1'";
	db::SQLStatement stmt(sql);

	BOOST_CHECK(stmt.to_string() == sql);

	db::SQLStatement stmt2 = stmt;

	BOOST_CHECK(stmt2.to_string() == sql);
	
	BOOST_CHECK(db::SQLStatement::escape_string("d'oh") == "d''oh");

	std::ostringstream ss;
	ss << stmt;
	BOOST_CHECK(ss.str() == sql);
    return 0;
}
