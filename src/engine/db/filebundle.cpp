/*
 * niepce - engine/db/filebundle.cpp
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


#include <boost/version.hpp>
#include <boost/filesystem/convenience.hpp>

#include "filebundle.hpp"
#include "fwk/utils/debug.hpp"
#include "fwk/toolkit/mimetype.hpp"


namespace db {

void 
FileBundle::add(const boost::filesystem::path & path)
{
    // TODO make it more reliable with more tests.
    fwk::MimeType type(path);
    
    if(type.isImage()) {
        if(type.isDigicamRaw()) {
            if(!m_main.empty() && m_jpeg.empty()) {
                m_jpeg = m_main;
            }
            m_main = path;
        }
        else {
            if(!m_main.empty()) {
                m_jpeg = path;
            }
            else {
                m_main = path;
            }
        }
    }
    else if(type.isXmp()) {
        m_xmp_sidecar = path;
    }
    else {
        DBG_OUT("Unkown file %s\n", path.string().c_str());
    }
}


FileBundle::ListPtr 
FileBundle::filter_bundles(const utils::FileList::Ptr & files)
{
    FileBundle::ListPtr bundles(new FileBundle::List());
    FileBundle::Ptr current_bundle;
    std::string current_base;

    files->sort();

    for(utils::FileList::const_iterator iter = files->begin();
        iter != files->end(); ++iter)
    {
        std::string basename;
#if BOOST_VERSION >= 103600
        basename = iter->stem();
#else
        basename = boost::filesystem::basename(*iter);
#endif
        if(basename != current_base) {
            current_base = basename;
            current_bundle = FileBundle::Ptr(new FileBundle());
            bundles->push_back(current_bundle);
        }
        current_bundle->add(*iter);
    }

    return bundles;
}


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

