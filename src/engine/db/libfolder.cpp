/*
 * niepce - eng/db/libfolder.cpp
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

#include "libfolder.hpp"

namespace eng {

const char* LibFolder::read_db_columns()
{
  return "id,name,virtual,locked";
}

LibFolder::Ptr LibFolder::read_from(const db::IConnectionDriver::Ptr & db)
{
  library_id_t id;
  std::string name;
  int32_t virt_type, locked;
  db->get_column_content(0, id);
  db->get_column_content(1, name);
  db->get_column_content(2, virt_type);
  db->get_column_content(3, locked);
  LibFolder::Ptr f(new LibFolder(id, name));
  f->set_virtual_type((VirtualType)virt_type);
  f->set_is_locked(locked);
  return f;
}

}
