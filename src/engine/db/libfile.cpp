/*
 * niepce - db/libfile.cpp
 *
 * Copyright (C) 2007-2020 Hubert Figuière
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

#include "libfile.hpp"

namespace eng {

LibFilePtr libfile_wrap(eng::LibFile *lf)
{
    return LibFilePtr(lf, &ffi::engine_db_libfile_delete);
}

/**
 * Converts a mimetype, which is expensive to calculate, into a FileType.
 * @param mime The mimetype we want to know as a filetype
 * @return the filetype
 * @todo: add the JPEG+RAW file types.
 */
FileType mimetype_to_filetype(fwk::MimeType mime)
{
    if (mime.isDigicamRaw()) {
        return FileType::RAW;
    } else if (mime.isImage()) {
        return FileType::IMAGE;
    } else if (mime.isMovie()) {
        return FileType::VIDEO;
    } else {
        return FileType::UNKNOWN;
    }
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
