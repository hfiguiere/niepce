/*
 * niepce - rust_bindings.hpp
 *
 * Copyright (C) 2017 Hubert Figuiere
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

#if !RUST_BINDGEN

#include "engine/db/properties-enum.hpp"

namespace fwk {
class FileList;
}
namespace eng {
class QueriedContent;
}


namespace ffi {
typedef eng::QueriedContent QueriedContent;
typedef eng::NiepceProperties Np;
typedef fwk::FileList FileList;
}

#include "target/bindings.h"

namespace fwk {

typedef ffi::ExempiManager ExempiManager;
typedef ffi::PropertyValue PropertyValue;
typedef ffi::PropertyBag PropertyBag;
typedef ffi::PropertySet PropertySet;
typedef ffi::Date Date;
typedef ffi::RgbColour RgbColour;

}

namespace eng {
typedef ffi::LibraryId library_id_t; // XXX change this to LibraryId
typedef ffi::FileType FileType;
typedef ffi::Keyword Keyword;
typedef ffi::LibFile LibFile;
typedef ffi::LibFolder LibFolder;
typedef ffi::LibMetadata LibMetadata;
typedef ffi::Label Label;
typedef ffi::Managed Managed;
typedef ffi::Notification LibNotification;
typedef ffi::NotificationType NotificationType;
typedef ffi::Np Np;
typedef ffi::FolderVirtualType FolderVirtualType;
}

#endif
