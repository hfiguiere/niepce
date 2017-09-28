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

#pragma once

#include <list>
#include <memory>

#include "fwk/utils/files.hpp"

#include "rust_bindings.hpp"

namespace eng {

#if RUST_BINDGEN
class FileBundle;
#endif

typedef std::shared_ptr<FileBundle> FileBundlePtr;
typedef std::list<FileBundlePtr> FileBundleList;
typedef std::shared_ptr<FileBundleList> FileBundleListPtr;

FileBundleListPtr filebundle_filter_bundles(const fwk::FileList::Ptr & files);

FileBundlePtr filebundle_new();
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
