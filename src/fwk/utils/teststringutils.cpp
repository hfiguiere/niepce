/*
 * niepce - utils/teststringutils.cpp
 *
 * Copyright (C) 2008 Hubert Figuiere
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



#include <boost/test/minimal.hpp>

#include <iostream>
#include <vector>
#include <string>


#include "stringutils.hpp"

int test_main( int, char *[] )             // note the name!
{
	std::vector<std::string> v;
	v.push_back("a");
	v.push_back("b");
	v.push_back("c");

	std::string s = fwk::join(v, ", ");

	BOOST_CHECK(s == "a, b, c");

	return 0;
}

