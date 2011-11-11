/*
 * niepce - eng/db/properties.hpp
 *
 * Copyright (C) 2011 Hubert Figuiere
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


#ifndef __ENG_PROPERTIES_HPP__
#define __ENG_PROPERTIES_HPP__

#include "fwk/base/propertybag.hpp"
#include "engine/db/metadata.hpp"

namespace eng {

  // prefix Np is for Niepce Property

#define DEFINE_PROPERTY(a,b,c,d)                 \
    a = b,

enum {

    #include "engine/db/properties-def.hpp"

    _NpLastProp
};

#undef DEFINE_PROPERTY

struct property_desc_t {
    fwk::PropertyIndex prop;
    const char * name;
};

extern const property_desc_t properties_names[];

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
#endif
