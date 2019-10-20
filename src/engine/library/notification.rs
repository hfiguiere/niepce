/*
 * niepce - engine/library/notification.rs
 *
 * Copyright (C) 2017-2019 Hubert Figuière
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
use engine::db::libfile::{
    FileStatus,
};
use libraryclient::LcChannel;

#[cfg(not(test))]
use root::eng::QueriedContent;
#[cfg(not(test))]
pub type Content = QueriedContent;

// Content need to be stubbed for the test as it is a FFI struct
// and is missing the proper C++ code.
#[cfg(test)]
#[derive(Clone,Copy)]
pub struct Content {
}

#[repr(i32)]
#[allow(non_camel_case_types)]
pub enum NotificationType {
    NONE,
    NEW_LIBRARY_CREATED,
    ADDED_FOLDER,
    ADDED_FILE,
    ADDED_FILES,
    ADDED_KEYWORD,
    ADDED_LABEL,
    FOLDER_CONTENT_QUERIED,
    FOLDER_DELETED,
    FOLDER_COUNTED,
    FOLDER_COUNT_CHANGE,
    KEYWORD_CONTENT_QUERIED,
    KEYWORD_COUNTED,
    KEYWORD_COUNT_CHANGE,
    METADATA_QUERIED,
    METADATA_CHANGED,
    LABEL_CHANGED,
    LABEL_DELETED,
    XMP_NEEDS_UPDATE,
    FILE_MOVED,
    FILE_STATUS_CHANGED,
}

#[repr(C)]
#[derive(Clone)]
pub struct FileMove {
    pub file: LibraryId,
    pub from: LibraryId,
    pub to: LibraryId,
}

#[repr(C)]
#[derive(Clone)]
pub struct FileStatusChange {
    pub id: LibraryId,
    pub status: FileStatus,
}

#[repr(C)]
#[derive(Clone)]
pub struct Count {
    pub id: LibraryId,
    pub count: i64,
}

#[derive(Clone)]
pub struct MetadataChange {
    id: LibraryId,
    meta: PropertyIndex,
    value: PropertyValue,
}

impl MetadataChange {
    pub fn new(id: LibraryId, meta: PropertyIndex, value: PropertyValue) -> Self {
        MetadataChange {id, meta, value}
    }
}

#[no_mangle]
pub unsafe extern "C" fn metadatachange_get_id(meta: *const MetadataChange) -> LibraryId {
    (*meta).id
}

#[no_mangle]
pub unsafe extern "C" fn metadatachange_get_meta(meta: *const MetadataChange) -> PropertyIndex {
    (*meta).meta
}

#[no_mangle]
pub unsafe extern "C" fn metadatachange_get_value(meta: *const MetadataChange) -> *const PropertyValue {
    &(*meta).value
}

#[derive(Clone)]
pub enum LibNotification {
    AddedFile,
    AddedFiles,
    AddedFolder(LibFolder),
    AddedKeyword(Keyword),
    AddedLabel(Label),
    FileMoved(FileMove),
    FileStatusChanged(FileStatusChange),
    FolderContentQueried(Content),
    FolderCounted(Count),
    FolderCountChanged(Count),
    FolderDeleted(LibraryId),
    KeywordContentQueried(Content),
    KeywordCounted(Count),
    KeywordCountChanged(Count),
    LabelChanged(Label),
    LabelDeleted(LibraryId),
    LibCreated,
    MetadataChanged(MetadataChange),
    MetadataQueried(LibMetadata),
    XmpNeedsUpdate,
}

impl Drop for LibNotification {
    fn drop(&mut self) {
        match *self {
            LibNotification::FolderContentQueried(mut c) |
            LibNotification::KeywordContentQueried(mut c) => {
                unsafe { c.destruct(); }
            },
            _ => (),
        }
    }
}

/// Send a notification for the file status change.
/// Return `false` if sending failed.
#[no_mangle]
pub unsafe extern "C" fn engine_library_notify_filestatus_changed(channel: *const LcChannel,
                                                           id: LibraryId,
                                                           status: FileStatus) -> bool {
    if let Err(err) = (*channel).0.clone().send(LibNotification::FileStatusChanged(
        FileStatusChange{id, status})) {
        err_out!("Error sending notification: {}", err);
        return false;
    }
    true
}

/// Delete the Notification object.
#[no_mangle]
pub unsafe extern "C" fn engine_library_notification_delete(n: *mut LibNotification) {
    Box::from_raw(n);
}

#[no_mangle]
pub unsafe extern "C" fn engine_library_notification_type(n: *const LibNotification) -> NotificationType {
    match n.as_ref() {
        Some(&LibNotification::AddedFile) => NotificationType::ADDED_FILE,
        Some(&LibNotification::AddedFiles) => NotificationType::ADDED_FILES,
        Some(&LibNotification::AddedFolder(_)) => NotificationType::ADDED_FOLDER,
        Some(&LibNotification::AddedKeyword(_)) => NotificationType::ADDED_KEYWORD,
        Some(&LibNotification::AddedLabel(_)) => NotificationType::ADDED_LABEL,
        Some(&LibNotification::FileMoved(_)) => NotificationType::FILE_MOVED,
        Some(&LibNotification::FileStatusChanged(_)) => NotificationType::FILE_STATUS_CHANGED,
        Some(&LibNotification::FolderContentQueried(_)) => NotificationType::FOLDER_CONTENT_QUERIED,
        Some(&LibNotification::FolderCounted(_)) => NotificationType::FOLDER_COUNTED,
        Some(&LibNotification::FolderCountChanged(_)) => NotificationType::FOLDER_COUNT_CHANGE,
        Some(&LibNotification::FolderDeleted(_)) => NotificationType::FOLDER_DELETED,
        Some(&LibNotification::KeywordContentQueried(_)) =>
            NotificationType::KEYWORD_CONTENT_QUERIED,
        Some(&LibNotification::KeywordCounted(_)) => NotificationType::KEYWORD_COUNTED,
        Some(&LibNotification::KeywordCountChanged(_)) => NotificationType::KEYWORD_COUNT_CHANGE,
        Some(&LibNotification::LabelChanged(_)) => NotificationType::LABEL_CHANGED,
        Some(&LibNotification::LabelDeleted(_)) => NotificationType::LABEL_DELETED,
        Some(&LibNotification::LibCreated) => NotificationType::NEW_LIBRARY_CREATED,
        Some(&LibNotification::MetadataChanged(_)) => NotificationType::METADATA_CHANGED,
        Some(&LibNotification::MetadataQueried(_)) => NotificationType::METADATA_QUERIED,
        Some(&LibNotification::XmpNeedsUpdate) => NotificationType::XMP_NEEDS_UPDATE,
        None => unreachable!(),
    }
}


#[no_mangle]
pub unsafe extern "C" fn engine_library_notification_get_id(n: *const LibNotification) -> LibraryId {
    match n.as_ref() {
        Some(&LibNotification::MetadataChanged(ref changed)) => changed.id,
        Some(&LibNotification::FolderDeleted(id)) => id,
        Some(&LibNotification::LabelDeleted(id)) => id,
        Some(&LibNotification::FileStatusChanged(ref changed)) => changed.id,
        _ => unreachable!(),
    }
}

#[no_mangle]
pub unsafe extern "C" fn engine_library_notification_get_label(n: *const LibNotification) -> *const Label {
    match n.as_ref() {
        Some(&LibNotification::AddedLabel(ref l)) |
        Some(&LibNotification::LabelChanged(ref l)) => l,
        _ => unreachable!(),
    }
}

#[no_mangle]
pub unsafe extern "C" fn engine_library_notification_get_filemoved(n: *const LibNotification) -> *const FileMove {
    match n.as_ref() {
        Some(&LibNotification::FileMoved(ref m)) => m,
        _ => unreachable!()
    }
}

#[no_mangle]
pub unsafe extern "C" fn engine_library_notification_get_filestatus(n: *const LibNotification) -> FileStatus {
    match n.as_ref() {
        Some(&LibNotification::FileStatusChanged(ref s)) => s.status,
        _ => unreachable!()
    }
}

#[no_mangle]
pub unsafe extern "C" fn engine_library_notification_get_libmetadata(n: *const LibNotification) -> *const LibMetadata {
    match n.as_ref() {
        Some(&LibNotification::MetadataQueried(ref m)) => m,
        _ => unreachable!()
    }
}

#[no_mangle]
pub unsafe extern "C" fn engine_library_notification_get_count(n: *const LibNotification) -> *const Count {
    match n.as_ref() {
        Some(&LibNotification::FolderCountChanged(ref c)) |
        Some(&LibNotification::FolderCounted(ref c)) |
        Some(&LibNotification::KeywordCountChanged(ref c)) |
        Some(&LibNotification::KeywordCounted(ref c)) =>
            c,
        _ => unreachable!()
    }
}

#[no_mangle]
pub unsafe extern "C" fn engine_library_notification_get_metadatachange(n: *const LibNotification) -> *const MetadataChange {
    match n.as_ref() {
        Some(&LibNotification::MetadataChanged(ref c)) => c,
        _ => unreachable!()
    }
}

#[no_mangle]
pub unsafe extern "C" fn engine_library_notification_get_libfolder(n: *const LibNotification) -> *const LibFolder {
    match n.as_ref() {
        Some(&LibNotification::AddedFolder(ref f)) => f,
        _ => unreachable!()
    }
}

#[no_mangle]
pub unsafe extern "C" fn engine_library_notification_get_keyword(n: *const LibNotification) -> *const Keyword {
    match n.as_ref() {
        Some(&LibNotification::AddedKeyword(ref f)) => f,
        _ => unreachable!()
    }
}

#[no_mangle]
pub unsafe extern "C" fn engine_library_notification_get_content(n: *const LibNotification) -> *const Content {
    match n.as_ref() {
        Some(&LibNotification::FolderContentQueried(ref c)) |
        Some(&LibNotification::KeywordContentQueried(ref c)) => {
            c
        },
        _ => {
            unreachable!()
        }
    }
}

#[cfg(test)]
use libc::c_void;

#[cfg(test)]
impl Content {
    pub unsafe fn new(_: LibraryId) -> Self {
        Content{}
    }

    pub unsafe fn push(&mut self, _: *mut c_void) {
    }

    pub unsafe fn destruct(&self) {
    }
}
