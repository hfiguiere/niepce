/*
 * niepce - engine/db/filebundle.hpp
 *
 * Copyright (C) 2009-2013 Hubert Figuiere
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

#ifndef __DB_FILEBUNDLE_H_
#define __DB_FILEBUNDLE_H_

#include <list>
#include <string>
#include <memory>

#include "fwk/utils/files.hpp"
#include "engine/db/libfile.hpp"

namespace eng {


class FileBundle
{
public:
    typedef std::shared_ptr<FileBundle> Ptr;
    typedef std::list<Ptr> List;
    typedef std::shared_ptr<List> ListPtr;

    FileBundle()
        : m_type(LibFile::FILE_TYPE_UNKNOWN)
        { }
    LibFile::FileType type() const
        { return m_type; }

    /** add a file to a bundle. Will determine what type it is. 
     * @return false if it does not know about the file
     */
    bool add(const std::string & path);
    const std::string & main_file() const
        { return m_main; }
    const std::string & jpeg() const
        { return m_jpeg; }
    const std::string & sidecar() const
        { return m_xmp_sidecar; }
    
    static ListPtr filter_bundles(const fwk::FileList::Ptr & files);
private:
    LibFile::FileType m_type;
    std::string m_main;
    std::string m_xmp_sidecar;
    std::string m_jpeg;
    std::string m_thumbnail;
};


/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/

}

#endif

