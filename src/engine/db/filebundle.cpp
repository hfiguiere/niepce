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

FileBundlePtr filebundle_new() {
    return FileBundlePtr(ffi::engine_db_filebundle_new(), &ffi::engine_db_filebundle_delete);
}

FileBundleListPtr
filebundle_filter_bundles(const fwk::FileList::Ptr & files)
{
    FileBundleListPtr bundles(new FileBundleList);
    FileBundlePtr current_bundle;
    std::string current_base;

    files->sort();

    std::for_each(
        files->cbegin(), files->cend(),
        [&current_base, &bundles, &current_bundle] (const std::string & f) {
            std::string basename = fwk::path_stem(f);
            DBG_OUT("basename %s current_base %s", basename.c_str(), current_base.c_str());
            if(basename != current_base) {
                FileBundlePtr new_bundle(filebundle_new());
                auto r = engine_db_filebundle_add(new_bundle.get(), f.c_str());
                DBG_OUT("r = %d", r);
                if (r) {
                    bundles->push_back(new_bundle);
                    current_bundle = new_bundle;
                    current_base = basename;
                }
            } else {
                engine_db_filebundle_add(current_bundle.get(), f.c_str());
            }
        });

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

