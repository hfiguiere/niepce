/*
 * niepce - engine/library/commands.rs
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

use std::os::raw::c_void;
use std::path::Path;

use fwk::PropertyValue;
use engine::db::LibraryId;
use engine::db::library::{
    Library,
    Managed
};
use engine::db::filebundle::FileBundle;
use engine::db::keyword::Keyword;
use engine::db::label::Label;
use engine::db::libfolder::LibFolder;
use super::notification::Notification as LibNotification;
use super::notification::{
    Content,
    Count,
    FileMove,
    MetadataChange,
};
use root::eng::NiepceProperties as Np;

pub fn cmd_list_all_keywords(lib: &Library) -> bool {
    if let Ok(list) = lib.get_all_keywords() {
        // XXX change this to "LoadKeywords"
        for kw in list {
            lib.notify(Box::new(LibNotification::AddedKeyword(kw)));
        }
        return true;
    }
    false
}

pub fn cmd_list_all_folders(lib: &Library) -> bool {
    if let Ok(list) = lib.get_all_folders() {
        // XXX change this to "LoadedFolders"
        for folder in list {
            lib.notify(Box::new(LibNotification::AddedFolder(folder)));
        }
        return true;
    }
    false
}

//
// Get the folder for import. Create it if needed otherwise return the one that exists
//
fn get_folder_for_import(lib: &Library, folder: &str) -> Option<LibFolder> {
    if let Ok(lf) = lib.get_folder(folder) {
        return Some(lf);
    } else if let Some(name) = Library::leaf_name_for_pathname(folder) {
        if let Ok(lf) = lib.add_folder(&name, Some(String::from(folder))) {
            let libfolder = lf.clone();
            lib.notify(Box::new(LibNotification::AddedFolder(lf)));
            return Some(libfolder);
        }
    }
    None
}

pub fn cmd_import_file(lib: &Library, path: &str, manage: Managed) -> bool {
    dbg_assert!(manage == Managed::NO, "managing file is currently unsupported");

    let mut bundle = FileBundle::new();
    bundle.add(path);

    let folder = Path::new(path).parent().unwrap_or(Path::new(""));

    if let Some(libfolder) = get_folder_for_import(lib, &*folder.to_string_lossy()) {
        if let Ok(_) = lib.add_bundle(libfolder.id(), &bundle, manage) {
            lib.notify(Box::new(LibNotification::AddedFile));
            return true;
        }
    }
    false
}

pub fn cmd_import_files(lib: &Library, folder: &str, files: &Vec<String>,
                        manage: Managed) -> bool {
    dbg_assert!(manage == Managed::NO, "managing file is currently unsupported");

    let bundles = FileBundle::filter_bundles(files);
    if let Some(libfolder) = get_folder_for_import(lib, folder) {
        let folder_id = libfolder.id();
        for bundle in bundles {
            if let Err(err) = lib.add_bundle(folder_id, &bundle, manage.clone()) {
                err_out!("Add bundle failed: {:?}", err);
            }
        }
        lib.notify(Box::new(LibNotification::AddedFiles));
        return true;
    }
    false
}

pub fn cmd_create_folder(lib: &Library, name: &String, path: Option<String>) -> LibraryId {
    if let Ok(lf) = lib.add_folder(name, path) {
        let id = lf.id();
        lib.notify(Box::new(LibNotification::AddedFolder(lf)));
        return id;
    }
    -1
}

pub fn cmd_delete_folder(lib: &Library, id: LibraryId) -> bool {
    if lib.delete_folder(id).is_ok() {
        lib.notify(Box::new(LibNotification::FolderDeleted(id)));
        return true;
    }
    false
}

pub fn cmd_request_metadata(lib: &Library, file_id: LibraryId) -> bool {
    if let Ok(lm) = lib.get_metadata(file_id) {
        lib.notify(Box::new(LibNotification::MetadataQueried(lm)));
        return true;
    }
    false
}

pub fn cmd_query_folder_content(lib: &Library, folder_id: LibraryId) -> bool {
    if let Ok(fl) = lib.get_folder_content(folder_id) {
        let mut value = Box::new(
            LibNotification::FolderContentQueried(unsafe { Content::new(folder_id) }));
        if let LibNotification::FolderContentQueried(ref mut content) = *value {
            for f in fl {
                unsafe { content.push(Box::into_raw(Box::new(f)) as *mut c_void) };
            }
        }
        lib.notify(value);
        return true;
    }
    false
}

pub fn cmd_set_metadata(lib: &Library, id: LibraryId, meta: Np,
                        value: &PropertyValue) -> bool {
    let err = lib.set_metadata(id, meta, value);
    if err.is_ok() {
        lib.notify(Box::new(LibNotification::MetadataChanged(
            MetadataChange::new(id, meta as u32, Box::new(value.clone())))));
        true
    } else {
        err_out!("set_matadata failed: {:?}", err);
        false
    }
}

pub fn cmd_count_folder(lib: &Library, folder_id: LibraryId) -> bool {
    let err = lib.count_folder(folder_id);
    if let Ok(count) = err {
        lib.notify(Box::new(LibNotification::FolderCounted(
            Count{id: folder_id, count: count})));
        true
    } else {
        err_out!("count_folder failed: {:?}", err);
        false
    }
}

pub fn cmd_add_keyword(lib: &Library, keyword: &str) -> LibraryId {
    let err = lib.make_keyword(keyword);
    if let Ok(id) = err {
        lib.notify(Box::new(LibNotification::AddedKeyword(Keyword::new(id, keyword))));
        id
    } else {
        err_out!("add_keyword failed: {:?}", err);
        -1
    }
}

pub fn cmd_query_keyword_content(lib: &Library, keyword_id: LibraryId) -> bool {
    if let Ok(fl) = lib.get_keyword_content(keyword_id) {
        let mut content = unsafe { Content::new(keyword_id) };
        for f in fl {
            unsafe { content.push(Box::into_raw(Box::new(f)) as *mut c_void) };
        }
        lib.notify(Box::new(LibNotification::KeywordContentQueried(content)));
        return true;
    }
    false
}

pub fn cmd_count_keyword(lib: &Library, id: LibraryId) -> bool {
    if let Ok(count) = lib.count_keyword(id) {
        lib.notify(Box::new(LibNotification::KeywordCounted(
            Count{id: id, count: count})));
        return true;
    }
    false
}

pub fn cmd_write_metadata(lib: &Library, file_id: LibraryId) -> bool {
    lib.write_metadata(file_id).is_ok()
}

pub fn cmd_move_file_to_folder(lib: &Library, file_id: LibraryId, from: LibraryId,
                               to: LibraryId) -> bool {

    if lib.move_file_to_folder(file_id, to).is_ok() {
        lib.notify(Box::new(LibNotification::FileMoved(
            FileMove{file: file_id, from: from, to: to})));
        lib.notify(Box::new(LibNotification::FolderCountChanged(
            Count{id: from, count: -1})));
        lib.notify(Box::new(LibNotification::FolderCountChanged(
            Count{id: to, count: 1})));
        return true;
    }
    false
}

pub fn cmd_list_all_labels(lib: &Library) -> bool {
    if let Ok(l) = lib.get_all_labels() {
        // XXX change this notification type
        for label in l {
            lib.notify(Box::new(LibNotification::AddedLabel(label)));
        }
        return true;
    }
    false
}

pub fn cmd_create_label(lib: &Library, name: &str, colour: &str) -> LibraryId {
    if let Ok(id) = lib.add_label(name, colour) {
        let l = Label::new(id, name, colour);
        lib.notify(Box::new(LibNotification::AddedLabel(l)));
        id
    } else {
        -1
    }
}

pub fn cmd_delete_label(lib: &Library, label_id: LibraryId) -> bool {
    if let Ok(_) = lib.delete_label(label_id) {
        lib.notify(Box::new(LibNotification::LabelDeleted(label_id)));
        true
    } else {
        false
    }
}

pub fn cmd_update_label(lib: &Library, label_id: LibraryId, name: &str,
                        colour: &str) -> bool {
    if let Ok(_) = lib.update_label(label_id, name, colour) {
        let label = Label::new(label_id, name, colour);
        lib.notify(Box::new(LibNotification::LabelChanged(label)));
        true
    } else {
        false
    }
}

pub fn cmd_process_xmp_update_queue(lib: &Library, write_xmp: bool) -> bool {
    lib.process_xmp_update_queue(write_xmp).is_ok()
}
