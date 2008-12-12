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

//insertstatement_test
int test_main(int, char *[])
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

	BOOST_CHECK(s == "INSERT INTO table( id, name) VALUES (null, 'foo')");
    return 0;
}


