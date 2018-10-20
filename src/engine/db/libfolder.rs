/*
 * niepce - eng/db/libfolder.rs
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

use libc::c_char;
use std::ffi::CString;
use rusqlite;

use super::FromDb;
use super::LibraryId;

#[repr(i32)]
#[derive(Clone)]
pub enum FolderVirtualType {
    NONE = 0,
    TRASH = 1
}

impl From<i32> for FolderVirtualType {
    fn from(t: i32) -> Self {
        match t {
            0 => FolderVirtualType::NONE,
            1 => FolderVirtualType::TRASH,
            _ => FolderVirtualType::NONE,
        }
    }
}

#[derive(Clone)]
pub struct LibFolder {
    id: LibraryId,
    /// Name of the folder
    name: String,
    /// Path of the folder.
    path: Option<String>,
    locked: bool,
    expanded: bool,
    virt: FolderVirtualType,
    parent: LibraryId,
    cstr: CString,
}

impl LibFolder {
    pub fn new(id: LibraryId, name: &str, path: Option<String>) -> LibFolder {
        LibFolder {
            id, name: String::from(name),
            path,
            locked: false,
            expanded: false, virt: FolderVirtualType::NONE,
            parent: 0,
            cstr: CString::new("").unwrap(),
        }
    }

    pub fn id(&self) -> LibraryId {
        self.id
    }

    pub fn name(&self) -> &String {
        &self.name
    }

    pub fn locked(&self) -> bool {
        self.locked
    }

    pub fn set_locked(&mut self, locked: bool) {
        self.locked = locked;
    }

    pub fn expanded(&self) -> bool {
        self.expanded
    }

    pub fn set_expanded(&mut self, expanded: bool) {
        self.expanded = expanded;
    }

    pub fn virtual_type(&self) -> FolderVirtualType {
        self.virt.to_owned()
    }

    pub fn set_virtual_type(&mut self, virt: FolderVirtualType) {
        self.virt = virt;
    }

    pub fn parent(&self) -> LibraryId {
        self.parent
    }

    pub fn set_parent(&mut self, parent: LibraryId) {
        self.parent = parent;
    }
}

impl FromDb for LibFolder {

    fn read_db_columns() -> &'static str {
        "id,name,virtual,locked,expanded,path,parent_id"
    }

    fn read_db_tables() -> &'static str {
        "folders"
    }

    fn read_db_where_id() -> &'static str {
        "id"
    }

    fn read_from(row: &rusqlite::Row) -> Self {
        let id: LibraryId = row.get(0);
        let name: String = row.get(1);
        let virt_type: i32 = row.get(2);
        let locked = row.get(3);
        let expanded = row.get(4);
        let path: Option<String> = row.get_checked(5).ok();
        let parent = row.get(6);

        let mut libfolder = LibFolder::new(id, &name, path);
        libfolder.set_parent(parent);
        libfolder.set_virtual_type(FolderVirtualType::from(virt_type));
        libfolder.set_locked(locked);
        libfolder.set_expanded(expanded);

        libfolder
    }
}

#[no_mangle]
pub extern "C" fn engine_db_libfolder_id(obj: &LibFolder) -> i64 {
    obj.id() as i64
}

#[no_mangle]
pub extern "C" fn engine_db_libfolder_name(obj: &mut LibFolder) -> *const c_char {
    obj.cstr = CString::new(obj.name().clone()).unwrap();
    obj.cstr.as_ptr()
}

#[no_mangle]
pub extern "C" fn engine_db_libfolder_virtual_type(obj: &LibFolder) -> FolderVirtualType {
    obj.virtual_type()
}

#[no_mangle]
pub extern "C" fn engine_db_libfolder_expanded(obj: &LibFolder) -> bool {
    obj.expanded
}

#[no_mangle]
pub extern fn engine_db_libfolder_set_locked(obj: &mut LibFolder, locked: bool) {
    obj.set_locked(locked);
}

#[no_mangle]
pub extern fn engine_db_libfolder_set_expanded(obj: &mut LibFolder, expanded: bool) {
    obj.set_expanded(expanded);
}

#[no_mangle]
pub extern fn engine_db_libfolder_set_virtual_type(obj: &mut LibFolder, t: FolderVirtualType) {
    obj.set_virtual_type(t);
}
