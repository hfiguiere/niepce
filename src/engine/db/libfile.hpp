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

#include <string>
#include <list>
#include <memory>

#include "fwk/toolkit/mimetype.hpp"
#include "fwk/base/propertybag.hpp"
#include "engine/db/librarytypes.hpp"
#include "engine/db/keyword.hpp"

namespace eng {

enum class LibFileType {
    UNKNOWN = 0,
    RAW = 1,
    RAW_JPEG = 2,
    IMAGE = 3,
    VIDEO = 4
};

LibFileType mimetype_to_filetype(fwk::MimeType mime);

class LibFile;
typedef std::shared_ptr<LibFile> LibFilePtr;
typedef std::weak_ptr< LibFile> LibFileWeakPtr;
typedef std::list<LibFilePtr> LibFileList;
typedef std::shared_ptr<LibFileList> LibFileListPtr;

LibFilePtr libfile_new(library_id_t, library_id_t, library_id_t, const char*, const char*);
}

extern "C" const char* engine_db_libfile_path(const eng::LibFile*);
extern "C" eng::library_id_t engine_db_libfile_id(const eng::LibFile*);
extern "C" eng::library_id_t engine_db_libfile_folderid(const eng::LibFile*);
extern "C" int32_t engine_db_libfile_orientation(const eng::LibFile*);
extern "C" int32_t engine_db_libfile_rating(const eng::LibFile*);
extern "C" int32_t engine_db_libfile_label(const eng::LibFile*);
extern "C" int32_t engine_db_libfile_flag(const eng::LibFile*);
extern "C" int32_t engine_db_libfile_property(const eng::LibFile*, fwk::PropertyIndex);
extern "C" eng::LibFileType engine_db_libfile_file_type(const eng::LibFile*);
extern "C" void engine_db_libfile_set_orientation(eng::LibFile*, int32_t);
extern "C" void engine_db_libfile_set_rating(eng::LibFile*, int32_t);
extern "C" void engine_db_libfile_set_label(eng::LibFile*, int32_t);
extern "C" void engine_db_libfile_set_flag(eng::LibFile*, int32_t);
extern "C" void engine_db_libfile_set_property(const eng::LibFile*, fwk::PropertyIndex, int32_t);
extern "C" void engine_db_libfile_set_file_type(eng::LibFile*, eng::LibFileType);

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
