/*
 * niepce - fwk/utils/pathutils.hpp
 *
 * Copyright (C) 2009 Hubert Figuiere
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



#ifndef __FWK_PATHUTILS_HPP_
#define __FWK_PATHUTILS_HPP_

#include <string>

namespace fwk {

/** return the basename of a path. Example:
    /foo/bar/baz.txt returns baz.txt
 */
std::string path_basename(const std::string & path);

/** return the stem of a path, ie basename minus extension
    /foo/bar/baz.txt returns baz
 */
std::string path_stem(const std::string & path);

/** return the dirname of a path. Assume the last component is a file.
    /foo/bar/baz.txt returns /foo/bar/
 */
std::string path_dirname(const std::string & path);

/** return the extension of a path
    /foo/bar/baz.txt returns .txt
    It always return the "." if any. No extension = "".
 */
std::string path_extension(const std::string & path);

std::string path_replace_extension(const std::string & path, const char * ext);


bool path_exists(const std::string & path);
bool ensure_path_for_file(const std::string & path);
void path_remove_recursive(const std::string & path);

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
#endif
