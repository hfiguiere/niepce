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

use fwk::base::PropertyIndex;
use fwk::PropertyValue;
use engine::db::{
    LibraryId,
    Label, LibFolder, LibMetadata, Keyword
};

use root::eng::QueriedContent;

pub type Content = QueriedContent;

#[repr(i32)]
#[allow(non_camel_case_types)]
pub enum NotificationType {
    NONE = 0,
    NEW_LIBRARY_CREATED = 1,
    ADDED_FOLDER = 2,
    ADDED_FILE = 3,
    ADDED_FILES = 4,
    ADDED_KEYWORD = 5,
    ADDED_LABEL = 6,
    FOLDER_CONTENT_QUERIED = 7,
    KEYWORD_CONTENT_QUERIED = 8,
    METADATA_QUERIED = 9,
    METADATA_CHANGED = 10,
    LABEL_CHANGED = 11,
    LABEL_DELETED = 12,
    XMP_NEEDS_UPDATE = 13,
    FOLDER_COUNTED = 14,
    FOLDER_COUNT_CHANGE = 15,
    FILE_MOVED = 16,
}

#[repr(C)]
pub struct FileMove {
    pub file: LibraryId,
    pub from: LibraryId,
    pub to: LibraryId,
}

#[repr(C)]
pub struct FolderCount {
    pub folder: LibraryId,
    pub count: i64,
}

#[repr(C)]
pub struct MetadataChange {
    id: LibraryId,
    meta: PropertyIndex,
    value: *mut PropertyValue,
}

impl MetadataChange {
    pub fn new(id: LibraryId, meta: PropertyIndex, value: Box<PropertyValue>) -> Self {
        MetadataChange {id: id, meta: meta, value: Box::into_raw(value)}
    }
}

impl Drop for MetadataChange {
    fn drop(&mut self) {
        unsafe { Box::from_raw(self.value); }
    }
}

#[repr(C)]
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

impl Drop for Notification {
    fn drop(&mut self) {
        match *self {
            Notification::FolderContentQueried(mut c) |
            Notification::KeywordContentQueried(mut c) => {
                unsafe { c.destruct(); }
            },
            _ => (),
        }
    }
}

#[cfg(not(test))]
#[allow(improper_ctypes)]
extern "C" {
    // actually a *mut Notification
    pub fn engine_library_notify(notif_id: u64, n: *mut Notification);
}

#[cfg(test)]
#[no_mangle]
pub unsafe fn engine_library_notify(_: u64, _: *mut Notification) {
    // stub for tests
    // unsafe since it non test function is extern
}

/// Delete the Notification object.
#[no_mangle]
pub extern "C" fn engine_library_notification_delete(n: *mut Notification) {
    unsafe { Box::from_raw(n); }
}

#[no_mangle]
pub extern "C" fn engine_library_notification_type(n: *const Notification) -> NotificationType {
    let t = match unsafe { n.as_ref() } {
        Some(&Notification::AddedFile) => NotificationType::ADDED_FILE,
        Some(&Notification::AddedFiles) => NotificationType::ADDED_FILES,
        Some(&Notification::AddedFolder(_)) => NotificationType::ADDED_FOLDER,
        Some(&Notification::AddedKeyword(_)) => NotificationType::ADDED_KEYWORD,
        Some(&Notification::AddedLabel(_)) => NotificationType::ADDED_LABEL,
        Some(&Notification::FileMoved(_)) => NotificationType::FILE_MOVED,
        Some(&Notification::FolderContentQueried(_)) => NotificationType::FOLDER_CONTENT_QUERIED,
        Some(&Notification::FolderCounted(_)) => NotificationType::FOLDER_COUNTED,
        Some(&Notification::FolderCountChanged(_)) => NotificationType::FOLDER_COUNT_CHANGE,
        Some(&Notification::KeywordContentQueried(_)) =>
            NotificationType::KEYWORD_CONTENT_QUERIED,
        Some(&Notification::LabelChanged(_)) => NotificationType::LABEL_CHANGED,
        Some(&Notification::LabelDeleted(_)) => NotificationType::LABEL_DELETED,
        Some(&Notification::LibCreated) => NotificationType::NEW_LIBRARY_CREATED,
        Some(&Notification::MetadataChanged(_)) => NotificationType::METADATA_CHANGED,
        Some(&Notification::MetadataQueried(_)) => NotificationType::METADATA_QUERIED,
        Some(&Notification::XmpNeedsUpdate) => NotificationType::XMP_NEEDS_UPDATE,
        None => unreachable!(),
    };
    t
}


#[no_mangle]
pub extern "C" fn engine_library_notification_get_id(n: *const Notification) -> LibraryId {
    match unsafe { n.as_ref() } {
        Some(&Notification::MetadataChanged(ref changed)) => changed.id,
        Some(&Notification::LabelDeleted(id)) => id,
        _ => unreachable!(),
    }
}

#[no_mangle]
pub extern "C" fn engine_library_notification_get_label(n: *const Notification) -> *const Label {
    match unsafe { n.as_ref() } {
        Some(&Notification::AddedLabel(ref l)) |
        Some(&Notification::LabelChanged(ref l)) => l,
        _ => unreachable!(),
    }
}

#[no_mangle]
pub extern "C" fn engine_library_notification_get_filemoved(n: *const Notification) -> *const FileMove {
    match unsafe { n.as_ref() } {
        Some(&Notification::FileMoved(ref m)) => m,
        _ => unreachable!()
    }
}

#[no_mangle]
pub extern "C" fn engine_library_notification_get_libmetadata(n: *const Notification) -> *const LibMetadata {
    match unsafe { n.as_ref() } {
        Some(&Notification::MetadataQueried(ref m)) => m,
        _ => unreachable!()
    }
}

#[no_mangle]
pub extern "C" fn engine_library_notification_get_folder_count(n: *const Notification) -> *const FolderCount {
    match unsafe { n.as_ref() } {
        Some(&Notification::FolderCountChanged(ref c)) |
        Some(&Notification::FolderCounted(ref c)) =>
            c,
        _ => unreachable!()
    }
}

#[no_mangle]
pub extern "C" fn engine_library_notification_get_metadatachange(n: *const Notification) -> *const MetadataChange {
    match unsafe { n.as_ref() } {
        Some(&Notification::MetadataChanged(ref c)) => c,
        _ => unreachable!()
    }
}

#[no_mangle]
pub extern "C" fn engine_library_notification_get_libfolder(n: *const Notification) -> *const LibFolder {
    match unsafe { n.as_ref() } {
        Some(&Notification::AddedFolder(ref f)) => f,
        _ => unreachable!()
    }
}

#[no_mangle]
pub extern "C" fn engine_library_notification_get_keyword(n: *const Notification) -> *const Keyword {
    match unsafe { n.as_ref() } {
        Some(&Notification::AddedKeyword(ref f)) => f,
        _ => unreachable!()
    }
}

#[no_mangle]
pub extern "C" fn engine_library_notification_get_content(n: *const Notification) -> *const Content {
    match unsafe { n.as_ref() } {
        Some(&Notification::FolderContentQueried(ref c)) |
        Some(&Notification::KeywordContentQueried(ref c)) => {
            c
        },
        _ => {
            unreachable!()
        }
    }
}
