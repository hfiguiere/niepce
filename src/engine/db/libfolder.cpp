/*
 * niepce - eng/db/libfolder.cpp
 *
 * Copyright (C) 2007, 2011 Hubert Figuiere
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

#include "libfolder.hpp"

extern "C" eng::LibFolder *engine_db_libfolder_new(eng::library_id_t id,
                                                   const char *name);
extern "C" void engine_db_libfolder_delete(eng::LibFolder *);

namespace eng {

LibFolderPtr libfolder_new(eng::library_id_t id, const char *name)
{
    return LibFolderPtr(engine_db_libfolder_new(id, name),
                        &engine_db_libfolder_delete);
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
