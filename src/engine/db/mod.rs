/*
 * niepce - eng/db/keyword.rs
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

mod keyword;

pub type LibraryId = i64;

use self::keyword::Keyword;
use libc::c_char;
use std::ffi::CStr;
use std::ffi::CString;

#[no_mangle]
pub extern fn engine_db_keyword_new(id: i64, keyword: *const c_char) -> *mut Keyword {
    let kw = Box::new(Keyword::new(id, &*unsafe { CStr::from_ptr(keyword) }.to_string_lossy()));
    Box::into_raw(kw)
}

#[no_mangle]
pub extern fn engine_db_keyword_id(this: &Keyword) -> i64 {
    this.id() as i64
}

#[no_mangle]
pub extern fn engine_db_keyword_keyword(this: &mut Keyword) -> *const c_char {
    this.cstr = CString::new(this.keyword().clone()).unwrap();
    this.cstr.as_ptr()
}

#[no_mangle]
pub extern fn engine_db_keyword_delete(kw: *mut Keyword) {
    unsafe { Box::from_raw(kw) };
}
