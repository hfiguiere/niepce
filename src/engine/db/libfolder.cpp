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

extern "C" eng::LibFolder* engine_db_libfolder_new(eng::library_id_t id, const char* name);
extern "C" void engine_db_libfolder_delete(eng::LibFolder*);

namespace eng {

LibFolderPtr libfolder_new(eng::library_id_t id, const char* name) {
  return LibFolderPtr(
    engine_db_libfolder_new(id, name), &engine_db_libfolder_delete);
}

const char* libfolder_read_db_columns()
{
    return "id,name,virtual,locked,expanded";
}

LibFolderPtr libfolder_read_from(const db::IConnectionDriver::Ptr & db)
{
    library_id_t id;
    std::string name;
    int32_t virt_type, locked, expanded;
    db->get_column_content(0, id);
    db->get_column_content(1, name);
    db->get_column_content(2, virt_type);
    db->get_column_content(3, locked);
    db->get_column_content(4, expanded);
    LibFolderPtr f(libfolder_new(id, name.c_str()));
    engine_db_libfolder_set_virtual_type(f.get(), virt_type);
    engine_db_libfolder_set_locked(f.get(), (bool)locked);
    engine_db_libfolder_set_expanded(f.get(), (bool)expanded);
    return f;
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
