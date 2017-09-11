/*
 * niepce - utils/exempi.cpp
 *
 * Copyright (C) 2007-2013 Hubert Figuiere
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

#include <string.h>
#include <time.h>

#include <boost/lexical_cast.hpp>

#include <giomm/file.h>
#include <glib.h>

#include <exempi/xmp.h>
#include <exempi/xmpconsts.h>

#include "exempi.hpp"
#include "fwk/base/date.hpp"
#include "fwk/base/debug.hpp"
#include "pathutils.hpp"

namespace xmp {

const char* NIEPCE_XMP_NAMESPACE = "http://xmlns.figuiere.net/ns/niepce/1.0";
const char* NIEPCE_XMP_NS_PREFIX = "niepce";

const char* UFRAW_INTEROP_NAMESPACE =
    "http://xmlns.figuiere.net/ns/ufraw_interop/1.0";
const char* UFRAW_INTEROP_NS_PREFIX = "ufrint";
}

extern "C" {

fwk::ExempiManager* fwk_exempi_manager_new();
void fwk_exempi_manager_delete(fwk::ExempiManager*);
}

namespace fwk {

ExempiManagerPtr exempi_manager_new()
{
    return ExempiManagerPtr(fwk_exempi_manager_new(),
                            &fwk_exempi_manager_delete);
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
