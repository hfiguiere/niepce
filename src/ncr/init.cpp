/*
 * niepce - ncr/init.cpp
 *
 * Copyright (C) 2011-2017 Hubert Figuière
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

#include <stdlib.h>
#include <stdio.h>

#include <gegl.h>

#include "init.hpp"

namespace ncr {

void init()
{
  if (getenv("DISABLE_OPENCL")) {
    // Disable OpenCL for when it causes hang on your system.
    // XXX evaluate a better way to enable-disable as needed
    // XXX my old system == old laptop requiring nouveau gfx driver.
    GeglConfig *config = gegl_config();
    GValue value = G_VALUE_INIT;
    g_value_init(&value, G_TYPE_BOOLEAN);
    g_value_set_boolean(&value, FALSE);
    fprintf(stderr, "Disabling opencl\n");
    g_object_set_property(G_OBJECT(config), "use-opencl", &value);
  }
  gegl_init(0, nullptr);
}

}
