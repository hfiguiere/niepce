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
#include "fwk/base/rust.hpp"

// Rust glue.

extern "C" {

fwk::RgbColour* fwk_rgbcolour_new(uint16_t r, uint16_t g, uint16_t b);
fwk::RgbColour* fwk_rgbcolour_clone(const fwk::RgbColour*);
void fwk_rgbcolour_delete(fwk::RgbColour*);
}

namespace fwk {

RgbColourPtr rgbcolour_new(uint16_t r, uint16_t g, uint16_t b)
{
    return rgbcolour_wrap(fwk_rgbcolour_new(r, g, b));
}

RgbColourPtr rgbcolour_clone(const RgbColour* c)
{
    return rgbcolour_wrap(fwk_rgbcolour_clone(c));
}

RgbColourPtr rgbcolour_wrap(RgbColour* c)
{
    return RgbColourPtr(c, &fwk_rgbcolour_delete);
}

std::string rgbcolour_to_string(uint16_t r, uint16_t g, uint16_t b)
{
    RgbColour* colour = fwk_rgbcolour_new(r, g, b);
    std::string s = rgbcolour_to_string(colour);
    fwk_rgbcolour_delete(colour);
    return s;
}

std::string rgbcolour_to_string(const RgbColour* c)
{
    char* p = fwk_rgbcolour_to_string(c);
    std::string s(p);
    rust_cstring_delete(p);
    return s;
}
}
