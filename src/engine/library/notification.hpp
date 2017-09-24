/* -*- mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode:nil; -*- */
/*
 * niepce - engine/library/notification.hpp
 *
 * Copyright (C) 2009-2017 Hubert Figuiere
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

#include <memory>

#include "engine/db/libfile.hpp"
#include "engine/db/librarytypes.hpp"
#include "engine/library/clienttypes.hpp"

#include "rust_bindings.hpp"

namespace eng {

#if RUST_BINDGEN
class Keyword;
class Label;
class LibFolder;
class LibMetadata;
class LibNotification;
#endif

typedef std::shared_ptr<eng::LibNotification> LibNotificationPtr;
class MetadataChange;
class FolderCount;
class FileMove;

enum class LibNotificationType {
    NONE = 0,
    NEW_LIBRARY_CREATED,
    ADDED_FOLDER,
    ADDED_FILE,
    ADDED_FILES,
    ADDED_KEYWORD,
    ADDED_LABEL,
    FOLDER_CONTENT_QUERIED,
    KEYWORD_CONTENT_QUERIED,
    METADATA_QUERIED,
    METADATA_CHANGED,
    LABEL_CHANGED,
    LABEL_DELETED,
    XMP_NEEDS_UPDATE,
    FOLDER_COUNTED,
    FOLDER_COUNT_CHANGE,
    FILE_MOVED
};

struct QueriedContent {
    library_id_t container;
    LibFileListPtr files;

    QueriedContent(library_id_t container);
    void push(LibFile*);
};

}
