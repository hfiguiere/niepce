/*
 * niepce - utils/testxmp.cpp
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
/** @brief unit test for xmp */

#include <boost/test/minimal.hpp>

#include <stdlib.h>
#include <vector>

#include "exempi.h"

int test_main( int, char *[] )             // note the name!
{
	boost::filesystem::path dir;
	const char * pdir = getenv("srcdir");
	if(pdir == NULL) {
		dir = ".";
	}
	else {
		dir = pdir;
	}
	utils::ExempiManager xmpManager;

	utils::XmpMeta meta(dir / "test.xmp");
	BOOST_CHECK(meta.isOk());
	BOOST_CHECK(meta.orientation() == 1);
	const std::vector< std::string > & keywords(meta.keywords());
	BOOST_CHECK(keywords.size() == 5);
	BOOST_CHECK(keywords[0] == "choir");
	BOOST_CHECK(keywords[1] == "night");
	BOOST_CHECK(keywords[2] == "ontario");
	BOOST_CHECK(keywords[3] == "ottawa");
	BOOST_CHECK(keywords[4] == "parliament of canada");
	
	return 0;
}

