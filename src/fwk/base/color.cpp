/*
 * niepce - fwk/base/color.hpp
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


#include <vector>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

#include "fwk/base/color.hpp"

namespace fwk {


  RgbColor::RgbColor(value_type r, value_type g, value_type b)
  {
    at(0) = r;
    at(1) = g;
    at(2) = b;
  }


  RgbColor::RgbColor(const std::string & s)
  {
    std::vector<std::string> components;
    boost::split(components, s, boost::is_any_of(" "));
    if(components.size() >= 3) {
      try {
        for(int i = 0; i < 3; ++i) {
          at(i) = boost::lexical_cast<value_type>(components[i]);
        }
        return;
      }
      catch(...) {

      }
    }
    // fallback in case of failure
    at(0) = 0;
    at(1) = 0;
    at(2) = 0;      
  }


  std::string RgbColor::to_string() const
  {
    return str(boost::format("%1% %2% %3%") % at(0) % at(1) % at(2));
  }


}
