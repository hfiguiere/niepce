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

namespace eng {
class QueriedContent;
}

using eng::QueriedContent;

#include "target/bindings.h"

namespace fwk {

typedef ffi::PropertyValue PropertyValue;
typedef ffi::PropertyBag PropertyBag;
typedef ffi::PropertySet PropertySet;
typedef ffi::Date Date;

}

namespace eng {

typedef ffi::Keyword Keyword;
typedef ffi::LibFolder LibFolder;
typedef ffi::LibMetadata LibMetadata;
typedef ffi::Label Label;
typedef ffi::Notification LibNotification;

}

#endif
