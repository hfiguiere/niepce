/*
 * niepce - engine/db/filebundle.cpp
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

#include <algorithm>

#include "filebundle.hpp"
#include "fwk/base/debug.hpp"
#include "fwk/utils/pathutils.hpp"
#include "fwk/toolkit/mimetype.hpp"


namespace eng {

bool 
FileBundle::add(const std::string & path)
{
    // TODO make it more reliable with more tests.
    fwk::MimeType mime_type(path);
    bool added = true;

    if(mime_type.isImage()) {
        if(mime_type.isDigicamRaw()) {
            if(!m_main.empty() && m_jpeg.empty()) {
                m_jpeg = m_main;
                m_type = LibFile::FileType::RAW_JPEG;
            }
            else {
                m_type = LibFile::FileType::RAW;
            }
            m_main = path;
        }
        else {
            if(!m_main.empty()) {
                m_jpeg = path;
                m_type = LibFile::FileType::RAW_JPEG;
            }
            else {
                m_main = path;
                m_type = LibFile::FileType::IMAGE;
            }
        }
    }
    else if(mime_type.isXmp()) {
        m_xmp_sidecar = path;
    }
    else if(mime_type.isMovie()) {
        m_main = path;
        m_type = LibFile::FileType::VIDEO;
    }
    else {
        DBG_OUT("Unkown file %s of type %s\n", path.c_str(), 
                mime_type.string().c_str());
        added = false;
    }
    return added;
}


FileBundle::ListPtr 
FileBundle::filter_bundles(const fwk::FileList::Ptr & files)
{
    FileBundle::ListPtr bundles(new FileBundle::List());
    FileBundle::Ptr current_bundle;
    std::string current_base;

    files->sort();

    std::for_each(files->begin(), files->end(),
                  [&current_base, &bundles, &current_bundle]
                  (const std::string & f) {
                      std::string basename = fwk::path_stem(f);

                      if(basename != current_base) {
                          FileBundle::Ptr new_bundle(new FileBundle());
                          if(new_bundle->add(f)) {
                              bundles->push_back(new_bundle);
                              current_bundle = new_bundle;
                              current_base = basename;
                          }
                      }
                      else {
                          current_bundle->add(f);
                      }
                  }
        );

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

