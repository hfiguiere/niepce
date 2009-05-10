/*
 * niepce - fwk/base/fractions.cpp
 *
 * Copyright (C) 2008-2009 Hubert Figuiere
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


#include <boost/rational.hpp>
#include <boost/lexical_cast.hpp>

#include "debug.hpp"
#include "fractions.hpp"

namespace fwk {


double fraction_to_decimal(const std::string & value)
{
    double v = 0.0;

    try {
        boost::rational<int> r = boost::lexical_cast<boost::rational<int> >(value);
        v = boost::rational_cast<double>(r);
    }
    catch(const std::exception & e) {
        ERR_OUT("unable to cast fraction '%s': %s", value.c_str(), e.what());
    }
    return v;
}


}
/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:80
  End:
*/
