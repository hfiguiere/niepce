/*
 * niepce - ncr/ncr.h
 *
 * Copyright (C) 2008,2011 Hubert Figuiere
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


#ifndef _NIEPCE_NCR_H
#define _NIEPCE_NCR_H

#include <glibmm/refptr.h>
#include <gegl.h>
#include <libopenraw/libopenraw.h>

namespace ncr {

/** load RAW data into a buffer. */
GeglBuffer* load_rawdata(ORRawDataRef rawdata);

}

#endif
/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:80
  End:
*/
