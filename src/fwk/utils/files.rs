/*
 * niepce - fwk/utils/files.rs
 *
 * Copyright (C) 2018 Hubert Figuière
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

use glib::translate::*;
use gio;
use gio_sys;
use gio::prelude::*;

use fwk::toolkit::mimetype::{
    guess_type,
    MType
};

#[no_mangle]
pub unsafe extern "C" fn file_is_media(finfo: *mut gio_sys::GFileInfo) -> bool {
    let fileinfo = gio::FileInfo::from_glib_none(finfo);
    if let Some(gmtype) = fileinfo.get_content_type() {
        let t = guess_type(&gmtype);
        return match t {
            MType::Image(_) |
            MType::Movie => true,
            _ => false
        };
    }

    false
}
