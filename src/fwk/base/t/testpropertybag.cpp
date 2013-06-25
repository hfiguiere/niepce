/*
 * niepce - base/t/testpropertybag.cpp
 *
 * Copyright (C) 2013 Hubert Figuiere
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
/** @brief unit test for propertybag */

#include <boost/test/minimal.hpp>

#include <stdlib.h>
#include "fwk/base/propertybag.hpp"

int test_main( int, char *[] )             // note the name!
{
	fwk::PropertyValue v;

	BOOST_CHECK(v.type() == typeid(fwk::EmptyValue));

	BOOST_CHECK(is_empty(v));
	BOOST_CHECK(get_integer(v) == 0);

	v = 42;

	BOOST_CHECK(!is_empty(v));
	BOOST_CHECK(is_integer(v));
	BOOST_CHECK(get_integer(v) == 42);

        return 0;
}

