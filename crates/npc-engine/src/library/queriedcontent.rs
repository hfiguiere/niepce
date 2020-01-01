/*
 * niepce - npc-engine/library/queriedcontent.rs
 *
 * Copyright (C) 2020 Hubert Figuière
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

use crate::db::libfile::LibFile;
use crate::db::LibraryId;

/// Queried content to pass a list of LibFile and the id of the container.
#[derive(Clone)]
pub struct QueriedContent {
    id: LibraryId,
    content: Vec<LibFile>,
}

impl QueriedContent {
    pub fn new(id: LibraryId) -> Self {
        QueriedContent {
            id,
            content: vec![],
        }
    }

    pub fn push(&mut self, f: LibFile) {
        self.content.push(f);
    }
}

#[no_mangle]
pub extern "C" fn engine_queried_content_id(obj: &QueriedContent) -> LibraryId {
    obj.id
}

#[no_mangle]
pub extern "C" fn engine_queried_content_size(obj: &QueriedContent) -> u64 {
    obj.content.len() as u64
}

#[no_mangle]
pub extern "C" fn engine_queried_content_get(obj: &QueriedContent, idx: usize) -> *mut LibFile {
    Box::into_raw(Box::new(obj.content[idx].clone()))
}
