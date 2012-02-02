/*
 * niepce - base/t/testdate.cpp
 *
 * Copyright (C) 2012 Hubert Figuiere
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
/** @brief unit test for date */

#include <boost/test/minimal.hpp>

#include <stdlib.h>
#include <vector>
#include <boost/rational.hpp>

#include "fwk/base/date.hpp"

int test_main( int, char *[] )             // note the name!
{
    fwk::Date d(0);
    
    XmpDateTime xmp_dt = d.xmp_date();
    BOOST_CHECK(xmp_dt.year == 1970);
    BOOST_CHECK(xmp_dt.month == 1);
    BOOST_CHECK(xmp_dt.day == 1);
    
    printf("%s\n", d.to_string().c_str());
    
    return 0;
}
/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
