/*
 * niepce - utils/testpathutils.cpp
 *
 * Copyright (C) 2009 Hubert Figuiere
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

#include <string>


#include "pathutils.hpp"

int test_main( int, char *[] )             // note the name!
{
  BOOST_CHECK(fwk::path_basename("/foo/bar/baz.txt") == "baz.txt");

  BOOST_CHECK(fwk::path_stem("/foo/bar/baz.txt") == "/foo/bar/baz");
  
  BOOST_CHECK(fwk::path_extension("/foo/bar/baz.txt") == ".txt");
  BOOST_CHECK(fwk::path_extension("img_2114.xmp") == ".xmp");

  BOOST_CHECK(fwk::path_replace_extension("/foo/bar/baz.txt", ".xmp") == "/foo/bar/baz.xmp");

	return 0;
}

