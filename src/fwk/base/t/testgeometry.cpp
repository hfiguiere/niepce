/*
 * niepce - utils/testgeometry.cpp
 *
 * Copyright (C) 2007-2013 Hubert Figuiere
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
#include "fwk/base/geometry.hpp"

using fwk::Rect;

int test_main( int, char *[] )             // note the name!
{
	Rect r1(0,1,2,3);
	BOOST_CHECK(std::to_string(r1) == "0 1 2 3");

	std::string s("100 100 250 250");
	Rect r2(s);
	BOOST_CHECK(r2.x() == 100);
	BOOST_CHECK(r2.y() == 100);
	BOOST_CHECK(r2.w() == 250);
	BOOST_CHECK(r2.h() == 250);
	std::vector<std::string> vtest;
	vtest.push_back("a b c d");
	vtest.push_back("100 100 150");
	std::for_each(vtest.begin(), vtest.end(),
		      [] (const std::string & value) {
			      bool raised = false;
			      try {
				      Rect r3(value);
			      }
			      catch(const std::bad_cast&) {
				      raised = true;
			      }
			      BOOST_CHECK(raised);
		      }
		);


    Rect dest1(0, 0, 640, 480);
    Rect dest2(0, 0, 480, 640);

    Rect source1(0, 0, 2000, 1000);
    Rect source2(0, 0, 1000, 2000);

    Rect result;

    // FIT
    result = source1.fit_into(dest1);
    std::cout << std::to_string(result) << std::endl;
    BOOST_CHECK(result == Rect(0, 0, 640, 320));
    result = source1.fit_into(dest2);
    std::cout << std::to_string(result) << std::endl;
    BOOST_CHECK(result.w() == 480);

    result = source2.fit_into(dest1);
    std::cout << std::to_string(result) << std::endl;
    BOOST_CHECK(result.h() == 480);
    result = source2.fit_into(dest2);
    std::cout << std::to_string(result) << std::endl;
    BOOST_CHECK(result == Rect(0, 0, 320, 640));

    // FILL
    result = source1.fill_into(dest1);
    std::cout << std::to_string(result) << std::endl;
    BOOST_CHECK(result.h() == 480);
    result = source1.fill_into(dest2);
    std::cout << std::to_string(result) << std::endl;
    BOOST_CHECK(result == Rect(0, 0, 1280, 640));

    result = source2.fill_into(dest1);
    std::cout << std::to_string(result) << std::endl;
    BOOST_CHECK(result == Rect(0, 0, 640, 1280));
    result = source2.fill_into(dest2);
    std::cout << std::to_string(result) << std::endl;
    BOOST_CHECK(result.w() == 480);

    return 0;
}

