/*
 * niepce - utils/testxmp.cpp
 *
 * Copyright (C) 2007-2008 Hubert Figuiere
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
/** @brief unit test for xmp */

#include <boost/test/minimal.hpp>

#include <stdlib.h>
#include <math.h>
#include <vector>

#include <glibmm/init.h>

#include "exempi.hpp"

int test_main( int, char *[] )             // note the name!
{
  Glib::init();

  std::string dir;
	const char * pdir = getenv("srcdir");
	if(pdir == NULL) {
		dir = ".";
	}
	else {
		dir = pdir;
	}
	fwk::ExempiManager xmpManager;

	fwk::XmpMeta meta(dir + "/test.xmp", true);
	BOOST_CHECK(meta.isOk());
	BOOST_CHECK(meta.orientation() == 1);
	const std::vector< std::string > & keywords(meta.keywords());
	BOOST_CHECK(keywords.size() == 5);
	BOOST_CHECK(keywords[0] == "choir");
	BOOST_CHECK(keywords[1] == "night");
	BOOST_CHECK(keywords[2] == "ontario");
	BOOST_CHECK(keywords[3] == "ottawa");
	BOOST_CHECK(keywords[4] == "parliament of canada");

        double output = fwk::XmpMeta::gpsCoordFromXmp("foobar");
        BOOST_CHECK(isnan(output));

        // malformed 1
        output = fwk::XmpMeta::gpsCoordFromXmp("45,29.6681666667");
        BOOST_CHECK(isnan(output));

        // malformed 2
        output = fwk::XmpMeta::gpsCoordFromXmp("45,W");
        BOOST_CHECK(isnan(output));

        // malformed 3
        output = fwk::XmpMeta::gpsCoordFromXmp("45,29,N");
        BOOST_CHECK(isnan(output));

        // out of bounds
        output = fwk::XmpMeta::gpsCoordFromXmp("200,29.6681666667N");
        BOOST_CHECK(isnan(output));

        // well-formed 1
        std::string gps1 = "45,29.6681666667N";
        output = fwk::XmpMeta::gpsCoordFromXmp(gps1);
        BOOST_CHECK(output == 45.494469444445002181964810006320476531982421875);

        // well-formed 2
        std::string gps2 = "73,38.2871666667W";
        output = fwk::XmpMeta::gpsCoordFromXmp(gps2);
        BOOST_CHECK(output == -73.6381194444449960201382054947316646575927734375);

        // well-formed 3
        std::string gps3 = "45,29,30.45N";
        output = fwk::XmpMeta::gpsCoordFromXmp(gps3);
        BOOST_CHECK(output == 45.49179166666666418450404307805001735687255859375);

	return 0;
}

