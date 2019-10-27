/*
 * niepce - fwk/base/propertyvalue.rs
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

use libc::c_char;
use std::ffi::{CStr, CString};

use super::date::Date;

#[derive(Clone, Debug)]
pub enum PropertyValue {
    Empty,
    Int(i32),
    String(String),
    StringArray(Vec<String>),
    Date(Date),
}

unsafe impl Send for PropertyValue {}

#[no_mangle]
pub unsafe extern "C" fn fwk_property_value_new_str(v: *const c_char) -> *mut PropertyValue {
    let cstr = CStr::from_ptr(v);
    let value = Box::new(PropertyValue::String(cstr.to_string_lossy().into_owned()));
    Box::into_raw(value)
}

#[no_mangle]
pub extern "C" fn fwk_property_value_new_int(v: i32) -> *mut PropertyValue {
    let value = Box::new(PropertyValue::Int(v));
    Box::into_raw(value)
}

#[no_mangle]
pub extern "C" fn fwk_property_value_new_date(v: &Date) -> *mut PropertyValue {
    let value = Box::new(PropertyValue::Date(*v));
    Box::into_raw(value)
}

#[no_mangle]
pub extern "C" fn fwk_property_value_new_string_array() -> *mut PropertyValue {
    let value = Box::new(PropertyValue::StringArray(vec![]));
    Box::into_raw(value)
}

#[no_mangle]
pub unsafe extern "C" fn fwk_property_value_delete(v: *mut PropertyValue) {
    if !v.is_null() {
        Box::from_raw(v);
    }
}

#[no_mangle]
pub unsafe extern "C" fn fwk_property_value_is_empty(v: *const PropertyValue) -> bool {
    match v.as_ref() {
        Some(&PropertyValue::Empty) => true,
        _ => false,
    }
}

#[no_mangle]
pub unsafe extern "C" fn fwk_property_value_is_integer(v: *const PropertyValue) -> bool {
    match v.as_ref() {
        Some(&PropertyValue::Int(_)) => true,
        _ => false,
    }
}

#[no_mangle]
pub unsafe extern "C" fn fwk_property_value_get_integer(v: *const PropertyValue) -> i32 {
    match v.as_ref() {
        Some(&PropertyValue::Int(i)) => i,
        _ => unreachable!(),
    }
}

#[no_mangle]
pub unsafe extern "C" fn fwk_property_value_is_date(v: *const PropertyValue) -> bool {
    match v.as_ref() {
        Some(&PropertyValue::Date(_)) => true,
        _ => false,
    }
}

#[no_mangle]
pub unsafe extern "C" fn fwk_property_value_get_date(v: *const PropertyValue) -> *const Date {
    match v.as_ref() {
        Some(&PropertyValue::Date(ref d)) => d,
        _ => unreachable!(),
    }
}

#[no_mangle]
pub unsafe extern "C" fn fwk_property_value_is_string(v: *const PropertyValue) -> bool {
    match v.as_ref() {
        Some(&PropertyValue::String(_)) => true,
        _ => false,
    }
}

#[no_mangle]
pub unsafe extern "C" fn fwk_property_value_get_string(v: *const PropertyValue) -> *mut c_char {
    match v.as_ref() {
        Some(&PropertyValue::String(ref s)) => CString::new(s.as_bytes()).unwrap().into_raw(),
        _ => unreachable!(),
    }
}

#[no_mangle]
pub unsafe extern "C" fn fwk_property_value_add_string(v: *mut PropertyValue, str: *const c_char) {
    match v.as_mut() {
        Some(&mut PropertyValue::StringArray(ref mut sa)) => {
            sa.push(CStr::from_ptr(str).to_string_lossy().into_owned());
        }
        _ => unreachable!(),
    }
}

#[no_mangle]
pub unsafe extern "C" fn fwk_property_value_count_string_array(v: *const PropertyValue) -> usize {
    match v.as_ref() {
        Some(&PropertyValue::StringArray(ref sa)) => sa.len(),
        _ => unreachable!(),
    }
}

#[no_mangle]
pub unsafe extern "C" fn fwk_property_value_get_string_at(
    v: *const PropertyValue,
    idx: usize,
) -> *mut c_char {
    match v.as_ref() {
        Some(&PropertyValue::StringArray(ref sa)) => {
            CString::new(sa[idx].as_bytes()).unwrap().into_raw()
        }
        _ => unreachable!(),
    }
}
