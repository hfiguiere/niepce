/*
 * niepce - eng/db/fsfile.hpp
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

#ifndef __NIEPCE_DB_FSFILE_H__
#define __NIEPCE_DB_FSFILE_H__

#include <string>
#include <tr1/memory>


namespace eng {

/** @brief describe a file on file system */
class FsFile
{
public:
    typedef std::tr1::shared_ptr< FsFile > Ptr;

    FsFile(int id, const std::string & path);

    int id()
        { return m_id; }
    const std::string & path() const
        { return m_path; }
private:
    int m_id;
    std::string m_path; /**< absolute path */
};

}

#endif
/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
