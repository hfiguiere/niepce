/*
 * niepce - fwk/base/colour.hpp
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

#include "fwk/base/colour.hpp"

#include "rust_bindings.hpp"

namespace fwk {

RgbColourPtr rgbcolour_new(uint16_t r, uint16_t g, uint16_t b)
{
  return rgbcolour_wrap(ffi::fwk_rgbcolour_new(r, g, b));
}

RgbColourPtr rgbcolour_clone(const RgbColour* c)
{
  return rgbcolour_wrap(ffi::fwk_rgbcolour_clone(c));
}

RgbColourPtr rgbcolour_wrap(RgbColour* c)
{
  return RgbColourPtr(c, &ffi::fwk_rgbcolour_delete);
}

std::string rgbcolour_to_string(uint16_t r, uint16_t g, uint16_t b)
{
  RgbColour* colour = ffi::fwk_rgbcolour_new(r, g, b);
  std::string s = rgbcolour_to_string(colour);
  ffi::fwk_rgbcolour_delete(colour);
  return s;
}

std::string rgbcolour_to_string(const RgbColour* c)
{
  char* p = ffi::fwk_rgbcolour_to_string(c);
  std::string s(p);
  ffi::rust_cstring_delete(p);
  return s;
}

}
