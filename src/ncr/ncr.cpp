/*
 * niepce - ncr/ncr.cpp
 *
 * Copyright (C) 2008 Hubert Figuiere
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, see 
 * <http://www.gnu.org/licenses/>.
 */

extern "C" {
#include <babl/babl.h>
}

#include <geglmm/init.h>

#include "ncr.h"

namespace ncr {

void init()
{
    Gegl::init(0, NULL);
}

Glib::RefPtr<Gegl::Buffer> load_rawdata(ORRawDataRef rawdata)
{
    uint32_t x, y;
    void *data;
    /* TODO take the dest_x and dest_y into account */
    GeglRectangle rect = {0, 0, 0, 0};
    or_rawdata_dimensions(rawdata, &x, &y);
    rect.width = x;
    rect.height = y;

    Glib::RefPtr<Gegl::Buffer> buffer 
        = Gegl::Buffer::create(Gegl::Rectangle(rect), babl_format ("Y u16"));

    data = or_rawdata_data(rawdata);
    buffer->set(Gegl::Rectangle(rect), babl_format ("Y u16"),
                data, GEGL_AUTO_ROWSTRIDE);

    return buffer;
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
