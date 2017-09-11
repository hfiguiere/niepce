/*
 * niepce - engine/library/notification.rs
 *
 * Copyright (C) 2017 Hubert Figuière
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

use libc::c_void;
use engine::db::LibraryId;
use engine::db::label::Label;
use engine::db::libfolder::LibFolder;
use engine::db::libmetadata::LibMetadata;
use engine::db::keyword::Keyword;

use root::eng::LibNotificationType as NotificationType;
pub use root::eng::LnFileMove as FileMove;
pub use root::eng::LnFolderCount as FolderCount;
pub use root::eng::QueriedContent as Content;
pub use root::eng::metadata_desc_t as MetadataChange;

pub enum Notification {
    AddedFile,
    AddedFiles,
    AddedFolder(LibFolder),
    AddedKeyword(Keyword),
    AddedLabel(Label),
    FileMoved(FileMove),
    FolderContentQueried(Content),
    FolderCounted(FolderCount),
    FolderCountChanged(FolderCount),
    KeywordContentQueried(Content),
    LabelChanged(Label),
    LabelDeleted(LibraryId),
    LibCreated,
    MetadataChanged(MetadataChange),
    MetadataQueried(LibMetadata),
    XmpNeedsUpdate,
}

#[cfg(not(test))]
extern "C" {
    // actually a *mut Notification
    pub fn engine_library_notify(notif_id: u64, n: *mut c_void);
}

#[cfg(test)]
#[no_mangle]
pub fn engine_library_notify(notif_id: u64, n: *mut c_void) {
    // stub for tests
}

#[no_mangle]
pub fn engine_library_notification_delete(n: *mut Notification) {
    unsafe { Box::from_raw(n); }
}


#[no_mangle]
pub fn engine_library_notification_type(n: &Notification) -> NotificationType {
    match *n {
        Notification::AddedFile => NotificationType::ADDED_FILE,
        Notification::AddedFiles => NotificationType::ADDED_FILES,
        Notification::AddedFolder(_) => NotificationType::ADDED_FOLDER,
        Notification::AddedKeyword(_) => NotificationType::ADDED_KEYWORD,
        Notification::AddedLabel(_) => NotificationType::ADDED_LABEL,
        Notification::FileMoved(_) => NotificationType::FILE_MOVED,
        Notification::FolderContentQueried(_) => NotificationType::FOLDER_CONTENT_QUERIED,
        Notification::FolderCounted(_) => NotificationType::FOLDER_COUNTED,
        Notification::FolderCountChanged(_) => NotificationType::FOLDER_COUNT_CHANGE,
        Notification::KeywordContentQueried(_) => NotificationType::KEYWORD_CONTENT_QUERIED,
        Notification::LabelChanged(_) => NotificationType::LABEL_CHANGED,
        Notification::LabelDeleted(_) => NotificationType::LABEL_DELETED,
        Notification::LibCreated => NotificationType::NEW_LIBRARY_CREATED,
        Notification::MetadataChanged(_) => NotificationType::METADATA_CHANGED,
        Notification::MetadataQueried(_) => NotificationType::METADATA_QUERIED,
        Notification::XmpNeedsUpdate => NotificationType::XMP_NEEDS_UPDATE,
    }
}

#[no_mangle]
pub fn engine_library_notification_get_id(n: &Notification) -> LibraryId {
    match *n {
        Notification::MetadataChanged(ref changed) => {
            changed.id
        },
        Notification::LabelDeleted(id) => {
            id
        },
        _ => {
            unreachable!()
        }
    }
}

#[no_mangle]
pub fn engine_library_notification_get_label(n: &Notification) -> *const Label {
    match *n {
        Notification::AddedLabel(ref l) => {
            l
        },
        _ => {
            unreachable!()
        }
    }
}

#[no_mangle]
pub fn engine_library_notification_get_filemoved(n: &Notification) -> *const FileMove {
    match *n {
        Notification::FileMoved(ref m) => {
            m
        },
        _ => {
            unreachable!()
        }
    }
}

#[no_mangle]
pub fn engine_library_notification_get_libmetadata(n: &Notification) -> *const LibMetadata {
    match *n {
        Notification::MetadataQueried(ref m) => {
            m
        },
        _ => {
            unreachable!()
        }
    }
}

#[no_mangle]
pub fn engine_library_notification_get_folder_count(n: &Notification) -> *const FolderCount {
    match *n {
        Notification::FolderCountChanged(ref c) |
        Notification::FolderCounted(ref c) => {
            c
        },
        _ => {
            unreachable!()
        }
    }
}

#[no_mangle]
pub fn engine_library_notification_get_metadatachange(n: &Notification) -> *const MetadataChange {
    match *n {
        Notification::MetadataChanged(ref c) => {
            c
        },
        _ => {
            unreachable!()
        }
    }
}

#[no_mangle]
pub fn engine_library_notification_get_libfolder(n: &Notification) -> *const LibFolder {
    match *n {
        Notification::AddedFolder(ref f) => {
            f
        },
        _ => {
            unreachable!()
        }
    }
}

#[no_mangle]
pub fn engine_library_notification_get_keyword(n: &Notification) -> *const Keyword {
    match *n {
        Notification::AddedKeyword(ref f) => {
            f
        },
        _ => {
            unreachable!()
        }
    }
}

#[no_mangle]
pub fn engine_library_notification_get_content(n: &Notification) -> *const Content {
    match *n {
        Notification::FolderContentQueried(ref c) |
        Notification::KeywordContentQueried(ref c) => {
            c
        },
        _ => {
            unreachable!()
        }
    }
}
