/*
 * niepce - lib.rs
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

extern crate exempi;
extern crate libc;

pub mod fwk;

use std::f64;
use std::ffi::CStr;
use libc::c_char;

#[no_mangle]
pub extern fn fwk_gps_coord_from_xmp(cvalue: *const c_char) -> f64 {
    let value = unsafe { CStr::from_ptr(cvalue) };
    if let Ok(svalue) = value.to_str() {
        if let Some(coord) = fwk::gps_coord_from_xmp(svalue) {
            return coord;
        }
    }
    f64::NAN
}
