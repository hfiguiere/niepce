/*
 * niepce - utils/testgeometry.cpp
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

#include <stdlib.h>
#include <vector>

#include "geometry.h"


int test_main( int, char *[] )             // note the name!
{
	utils::Rect r1(0,1,2,3);
	BOOST_CHECK(r1.to_string() == "0 1 2 3");
	
	std::string s("100 100 250 250");
	utils::Rect r2(s);
	BOOST_CHECK(r2.x() == 100);
	BOOST_CHECK(r2.y() == 100);
	BOOST_CHECK(r2.w() == 250);
	BOOST_CHECK(r2.h() == 250);
	bool raised = false;
	std::vector<std::string> vtest;
	vtest.push_back("a b c d");
	vtest.push_back("100 100 150");
	for(std::vector<std::string>::iterator iter = vtest.begin();
		iter != vtest.end(); ++iter) {
		try {
			utils::Rect r3(*iter);
		}
		catch(std::bad_cast) {
			raised = true;
		}
		BOOST_CHECK(raised);
	}
	return 0;
}

