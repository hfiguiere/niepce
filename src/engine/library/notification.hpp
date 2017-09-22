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

namespace eng {
class Label;
class LibMetadata;
class LibFolder;
class Keyword;

class LibNotification;
typedef std::shared_ptr<LibNotification> LibNotificationPtr;

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

struct LnFileMove {
    library_id_t file;
    library_id_t from;
    library_id_t to;
};

struct LnFolderCount {
    library_id_t folder;
    int64_t count;
};

struct QueriedContent {
    library_id_t container;
    LibFileListPtr files;

    QueriedContent(library_id_t container);
    void push(LibFile*);
};
}

extern "C" {

eng::LibNotificationType
engine_library_notification_type(const eng::LibNotification* n);

// if METADATA_CHANGE return the inner id. otherwise directly attached id.
eng::library_id_t
engine_library_notification_get_id(const eng::LibNotification* n);

const eng::Label*
engine_library_notification_get_label(const eng::LibNotification* n);

const eng::LnFileMove*
engine_library_notification_get_filemoved(const eng::LibNotification* n);

const eng::LibMetadata*
engine_library_notification_get_libmetadata(const eng::LibNotification* n);

const eng::LnFolderCount*
engine_library_notification_get_folder_count(const eng::LibNotification* n);

const eng::metadata_desc_t*
engine_library_notification_get_metadatachange(const eng::LibNotification* n);

const eng::LibFolder*
engine_library_notification_get_libfolder(const eng::LibNotification* n);

const eng::Keyword*
engine_library_notification_get_keyword(const eng::LibNotification* n);

const eng::QueriedContent*
engine_library_notification_get_content(const eng::LibNotification* n);

void engine_library_notification_delete(eng::LibNotification* n);

void engine_library_notify(uint64_t notify_id, eng::LibNotification* n);
}
