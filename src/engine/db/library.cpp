/*
 * niepce - engine/db/library.cpp
 *
 * Copyright (C) 2007-2017 Hubert Figuière
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

#include "library.hpp"

extern "C" eng::Library *engine_db_library_new(const char *dir,
                                               uint64_t notif_id);
extern "C" void engine_db_library_delete(eng::Library *);

namespace eng {

LibraryPtr library_new(const char *dir, uint64_t notif_id)
{
    return LibraryPtr(engine_db_library_new(dir, notif_id),
                      &engine_db_library_delete);
}
}
/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
