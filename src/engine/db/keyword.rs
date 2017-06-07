/*
 * niepce - engine/db/keyword.rs
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

use super::LibraryId;
use std::ffi::CString;

pub struct Keyword {
    id: LibraryId,
    keyword: String,
    pub cstr: CString,
}

impl Keyword {
    pub fn new(id: LibraryId, keyword: &str) -> Keyword {
        Keyword {
            id: id, keyword: String::from(keyword),
            cstr: CString::new("").unwrap()
        }
    }

    pub fn id(&self) -> LibraryId {
        self.id
    }

    pub fn keyword(&self) -> &String {
        &self.keyword
    }
}
