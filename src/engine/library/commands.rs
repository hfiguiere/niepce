/*
 * niepce - engine/library/commands.rs
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

use std::os::raw::c_void;

use fwk::PropertyValue;
use engine::db::LibraryId;
use engine::db::library;
use engine::db::library::{
    Library,
    Managed
};
use engine::db::filebundle::FileBundle;
use engine::db::keyword::Keyword;
use engine::db::label::Label;
use engine::db::libfolder::LibFolder;
use super::notification::LibNotification;
use super::notification::{
    Content,
    Count,
    FileMove,
    MetadataChange,
};
use root::eng::NiepceProperties as Np;

pub fn cmd_list_all_keywords(lib: &Library) -> bool {
    match  lib.get_all_keywords() {
        Ok(list) => {
            // XXX change this to "LoadKeywords"
            for kw in list {
                lib.notify(LibNotification::AddedKeyword(kw));
            }
            true
        },
        Err(err) => {
            err_out_line!("get all keywords failed: {:?}", err);
            false
        }
    }
}

pub fn cmd_list_all_folders(lib: &Library) -> bool {
    match lib.get_all_folders() {
        Ok(list) => {
            // XXX change this to "LoadedFolders"
            for folder in list {
                lib.notify(LibNotification::AddedFolder(folder));
            }
            true
        },
        Err(err) => {
            err_out_line!("get_all_folders failed: {:?}", err);
            false
        }
    }
}

//
// Get the folder for import. Create it if needed otherwise return the one that exists
//
fn get_folder_for_import(lib: &Library, folder: &str) -> library::Result<LibFolder> {
    match lib.get_folder(folder) {
        Ok(lf) => Ok(lf),
        Err(library::Error::NotFound) => {
            // folder doesn't exist, we'll create it
            if let Some(name) = Library::leaf_name_for_pathname(folder) {
                match lib.add_folder(&name, Some(String::from(folder))) {
                    Ok(lf) =>  {
                        let libfolder = lf.clone();
                        lib.notify(LibNotification::AddedFolder(lf));
                        Ok(libfolder)
                    },
                    Err(err) => {
                        err_out_line!("Add folder failed {:?}", err);
                        Err(err)
                    }
                }
            } else {
                err_out_line!("Can't get folder name.");
                Err(library::Error::InvalidResult)
            }
        },
        Err(err) => {
            err_out_line!("get folder failed: {:?}", err);
            Err(err)
        }
    }
}

pub fn cmd_import_files(lib: &Library, folder: &str, files: &[String],
                        manage: Managed) -> bool {
    dbg_assert!(manage == Managed::NO, "managing file is currently unsupported");

    let bundles = FileBundle::filter_bundles(files);
    match get_folder_for_import(lib, folder) {
        Ok(libfolder) =>  {
            let folder_id = libfolder.id();
            for bundle in bundles {
                if let Err(err) = lib.add_bundle(folder_id, &bundle, manage) {
                    err_out!("Add bundle failed: {:?}", err);
                }
            }
            lib.notify(LibNotification::AddedFiles);
            true
        },
        Err(err) => {
            err_out_line!("Get folder for import {:?}", err);
            false
        }
    }
}

pub fn cmd_create_folder(lib: &Library, name: &str, path: Option<String>) -> LibraryId {
    match lib.add_folder(name, path) {
        Ok(lf) =>  {
            let id = lf.id();
            lib.notify(LibNotification::AddedFolder(lf));
            id
        },
        Err(err) =>  {
            err_out_line!("Folder creation failed {:?}", err);
            -1
        }
    }
}

pub fn cmd_delete_folder(lib: &Library, id: LibraryId) -> bool {
    match lib.delete_folder(id) {
        Ok(_) => {
            lib.notify(LibNotification::FolderDeleted(id));
            true
        },
        Err(err) => {
            err_out_line!("Delete folder failed {:?}", err);
            false
        }
    }
}

pub fn cmd_request_metadata(lib: &Library, file_id: LibraryId) -> bool {
    match lib.get_metadata(file_id) {
        Ok(lm) => {
            lib.notify(LibNotification::MetadataQueried(lm));
            true
        },
        Err(err) => {
            err_out_line!("Get metadata failed {:?}", err);
            false
        }
    }
}

pub fn cmd_query_folder_content(lib: &Library, folder_id: LibraryId) -> bool {
    match lib.get_folder_content(folder_id) {
        Ok(fl) => {
            let mut value =
                LibNotification::FolderContentQueried(unsafe { Content::new(folder_id) });
            if let LibNotification::FolderContentQueried(ref mut content) = value {
                for f in fl {
                    unsafe { content.push(Box::into_raw(Box::new(f)) as *mut c_void) };
                }
            }
            lib.notify(value);
            true
        },
        Err(err) => {
            err_out_line!("Get folder content failed {:?}", err);
            false
        }
    }
}

pub fn cmd_set_metadata(lib: &Library, id: LibraryId, meta: Np,
                        value: &PropertyValue) -> bool {
    match lib.set_metadata(id, meta, value) {
        Ok(_) => {
            lib.notify(LibNotification::MetadataChanged(
                MetadataChange::new(id, meta as u32, value.clone())));
            true
        },
        Err(err) => {
            err_out_line!("set_matadata failed: {:?}", err);
            false
        }
    }
}

pub fn cmd_count_folder(lib: &Library, id: LibraryId) -> bool {
    match lib.count_folder(id) {
        Ok(count) => {
            lib.notify(LibNotification::FolderCounted(
                Count{id, count}));
            true
        },
        Err(err) => {
            err_out_line!("count_folder failed: {:?}", err);
            false
        }
    }
}

pub fn cmd_add_keyword(lib: &Library, keyword: &str) -> LibraryId {
    match lib.make_keyword(keyword) {
        Ok(id) => {
            lib.notify(LibNotification::AddedKeyword(Keyword::new(id, keyword)));
            id
        },
        Err(err) => {
            err_out_line!("make_keyword failed: {:?}", err);
            -1
        }
    }
}

pub fn cmd_query_keyword_content(lib: &Library, keyword_id: LibraryId) -> bool {
    match lib.get_keyword_content(keyword_id) {
        Ok(fl) => {
            let mut content = unsafe { Content::new(keyword_id) };
            for f in fl {
                unsafe { content.push(Box::into_raw(Box::new(f)) as *mut c_void) };
            }
            lib.notify(LibNotification::KeywordContentQueried(content));
            true
        },
        Err(err) => {
            err_out_line!("get_keyword_content failed: {:?}", err);
            false
        }
    }
}

pub fn cmd_count_keyword(lib: &Library, id: LibraryId) -> bool {
    match lib.count_keyword(id) {
        Ok(count) => {
            lib.notify(LibNotification::KeywordCounted(
                Count{id, count}));
            true
        },
        Err(err) => {
            err_out_line!("count_keyword failed: {:?}", err);
            false
        }
    }
}

pub fn cmd_write_metadata(lib: &Library, file_id: LibraryId) -> bool {
    match lib.write_metadata(file_id) {
        Ok(_) => true,
        Err(err) => {
            err_out_line!("write_metadata failed: {:?}", err);
            false
        }
    }
}

pub fn cmd_move_file_to_folder(lib: &Library, file_id: LibraryId, from: LibraryId,
                               to: LibraryId) -> bool {
    match lib.move_file_to_folder(file_id, to) {
        Ok(_) => {
            lib.notify(LibNotification::FileMoved(
                FileMove{file: file_id, from, to}));
            lib.notify(LibNotification::FolderCountChanged(
                Count{id: from, count: -1}));
            lib.notify(LibNotification::FolderCountChanged(
                Count{id: to, count: 1}));
            true
        },
        Err(err) => {
            err_out_line!("move file to folder failed: {:?}", err);
            false
        }
    }
}

pub fn cmd_list_all_labels(lib: &Library) -> bool {
    match lib.get_all_labels() {
        Ok(l) => {
            // XXX change this notification type
            for label in l {
                lib.notify(LibNotification::AddedLabel(label));
            }
            true
        },
        Err(err) => {
            err_out_line!("get_all_labels failed: {:?}", err);
            false
        }
    }
}

pub fn cmd_create_label(lib: &Library, name: &str, colour: &str) -> LibraryId {
    match lib.add_label(name, colour) {
        Ok(id) => {
            let l = Label::new(id, name, colour);
            lib.notify(LibNotification::AddedLabel(l));
            id
        },
        Err(err) => {
            err_out_line!("add_label failed: {:?}", err);
            -1
        }
    }
}

pub fn cmd_delete_label(lib: &Library, label_id: LibraryId) -> bool {
    match lib.delete_label(label_id) {
        Ok(_) => {
            lib.notify(LibNotification::LabelDeleted(label_id));
            true
        },
        Err(err) => {
            err_out_line!("delete label failed: {:?}", err);
            false
        }
    }
}

pub fn cmd_update_label(lib: &Library, label_id: LibraryId, name: &str,
                        colour: &str) -> bool {
    match lib.update_label(label_id, name, colour) {
        Ok(_) => {
            let label = Label::new(label_id, name, colour);
            lib.notify(LibNotification::LabelChanged(label));
            true
        },
        Err(err) => {
            err_out_line!("update label failed: {:?}", err);
            false
        }
    }
}

pub fn cmd_process_xmp_update_queue(lib: &Library, write_xmp: bool) -> bool {
    match lib.process_xmp_update_queue(write_xmp) {
        Ok(_) => true,
        Err(err) => {
            err_out_line!("process_xmp_update_queue failed: {:?}", err);
            false
        }
    }
}
