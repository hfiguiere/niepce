/*
 * niepce - utils/testmoniker.cpp
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
/** @brief Unit test for the Moniker class */


#include <boost/test/minimal.hpp>

#include "moniker.hpp"


int test_main( int, char *[] )             // note the name!
{
	utils::Moniker moniker("foo:/bar/test");

	BOOST_CHECK(moniker.scheme() == "foo");
	BOOST_CHECK(moniker.path() == "/bar/test");

	BOOST_CHECK(strcmp(moniker.c_str(), "foo:/bar/test") == 0);
	return 0;
}

