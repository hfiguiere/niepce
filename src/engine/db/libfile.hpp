/*
 * niepce - eng/db/libfile.hpp
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

#pragma once

#include <list>
#include <memory>
#include <string>

#if !RUST_BINDGEN
#include "fwk/toolkit/mimetype.hpp"
#endif

#include "engine/db/keyword.hpp"
#include "engine/db/librarytypes.hpp"
#include "fwk/base/propertybag.hpp"

namespace eng {

#if !RUST_BINDGEN
ffi::FileType mimetype_to_filetype(fwk::MimeType mime);
#endif

#if RUST_BINDGEN
class LibFile;
#endif
typedef std::shared_ptr<LibFile> LibFilePtr;
typedef std::weak_ptr<LibFile> LibFileWeakPtr;
typedef std::list<LibFilePtr> LibFileList;
typedef std::shared_ptr<LibFileList> LibFileListPtr;

LibFilePtr libfile_new(library_id_t, library_id_t, library_id_t, const char *,
                       const char *);
LibFilePtr libfile_wrap(LibFile *);
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
