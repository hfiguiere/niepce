/*
 * niepce - fwk/mod.rs
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

pub mod utils;
#[macro_use]
pub mod base;
pub mod toolkit;

pub use self::utils::exempi::{
    NsDef,
    ExempiManager,
    XmpMeta,
    gps_coord_from_xmp
};
pub use self::base::propertyvalue::PropertyValue;
pub use self::base::propertybag::PropertyBag;
pub use self::base::fractions::{
    fraction_to_decimal
};

pub use self::base::date::*;

pub use self::toolkit::mimetype::{
    MimeType
};

pub use root::fwk::FileList;

use std::f64;
use std::ffi::CStr;
use libc::c_char;

#[no_mangle]
pub extern fn fwk_gps_coord_from_xmp(cvalue: *const c_char) -> f64 {
    let value = unsafe { CStr::from_ptr(cvalue) };
    if let Ok(svalue) = value.to_str() {
        if let Some(coord) = gps_coord_from_xmp(svalue) {
            return coord;
        }
    }
    f64::NAN
}

#[no_mangle]
pub extern fn fwk_fraction_to_decimal(cvalue: *const c_char) -> f64 {
    let value = unsafe { CStr::from_ptr(cvalue) };
    if let Ok(svalue) = value.to_str() {
        if let Some(dec) = fraction_to_decimal(svalue) {
            return dec;
        }
    }
    f64::NAN
}
