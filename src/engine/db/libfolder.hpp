/*
 * niepce - eng/db/libfolder.hpp
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

#pragma once

#include <list>
#include <memory>
#include <string>

#include "engine/db/librarytypes.hpp"

#include "rust_bindings.hpp"

namespace eng {

#if RUST_BINDGEN
class LibFolder;
#endif

typedef std::shared_ptr<LibFolder> LibFolderPtr;
typedef std::list<LibFolderPtr> LibFolderList;
typedef std::shared_ptr<LibFolderList> LibFolderListPtr;

enum class LibFolderVirtualType {
    NONE = 0,
    TRASH = 1,

    _LAST
};

LibFolderPtr libfolder_new(library_id_t id, const char *name);

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
