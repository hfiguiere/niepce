/*
 * niepce - engine/db/filebundle.h
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

#ifndef __DB_FILEBUNDLE_H_
#define __DB_FILEBUNDLE_H_

#include <list>

#include <boost/shared_ptr.hpp>
#include <boost/filesystem/path.hpp>

#include "fwk/utils/files.h"
#include "engine/db/libfile.h"

namespace db {


class FileBundle
{
public:
    typedef boost::shared_ptr<FileBundle> Ptr;
    typedef std::list<Ptr> List;
    typedef boost::shared_ptr<List> ListPtr;

    /** add a file to a bundle. Will determine what type it is. */
    void add(const boost::filesystem::path & path);
    const boost::filesystem::path & main_file() const
        { return m_main; }
    const boost::filesystem::path & jpeg() const
        { return m_jpeg; }
    const boost::filesystem::path & sidecar() const
        { return m_xmp_sidecar; }
    
    static ListPtr filter_bundles(const utils::FileList::Ptr & files);
private:
    boost::filesystem::path m_main;
    boost::filesystem::path m_xmp_sidecar;
    boost::filesystem::path m_jpeg;
    boost::filesystem::path m_thumbnail;
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

