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



#ifndef __FWK_BASE_COLOR_HPP_
#define __FWK_BASE_COLOR_HPP_

#include <string>
#include <tr1/array>

namespace fwk {

  /** A RgbColor tuple (3 components, 8bpp)
   *  To be used only for UI.
   */
  class RgbColor
    : public std::tr1::array<uint8_t, 3>
  {
  public:
    RgbColor(value_type r = 0, value_type g = 0, value_type b = 0);
    explicit RgbColor(const std::string & );
    
    std::string to_string() const;
  };

}


#endif
