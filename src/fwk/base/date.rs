/*
 * niepce - fwk/base/date.rs
 *
 * Copyright (C) 2017-2018 Hubert Figuière
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

use libc;
use std::ffi::CString;

use chrono;
use chrono::{Datelike, Timelike};
use exempi;

pub type Time = i64;
pub type Date = chrono::DateTime<chrono::Utc>;

pub fn xmp_date_from(d: &chrono::DateTime<chrono::Utc>) -> exempi::DateTime {
    let mut xmp_date = exempi::DateTime::new();
    xmp_date.c.year = d.year();
    xmp_date.c.month = d.month() as i32;
    xmp_date.c.day = d.day() as i32;
    xmp_date.c.hour = d.hour() as i32;
    xmp_date.c.minute = d.minute() as i32;
    xmp_date.c.second = d.second() as i32;
    xmp_date.c.tz_sign = exempi::XmpTzSign::UTC;
    xmp_date.c.tz_hour = 0;
    xmp_date.c.tz_minute = 0;
    xmp_date.c.nano_second = 0;

    xmp_date
}

#[no_mangle]
pub unsafe extern "C" fn fwk_date_delete(date: *mut Date) {
    Box::from_raw(date);
}

#[no_mangle]
pub extern "C" fn fwk_date_to_string(date: &Date) -> *mut libc::c_char {
    CString::new(date.to_string().as_bytes()).unwrap().into_raw()
}
