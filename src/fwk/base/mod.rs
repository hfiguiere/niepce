/*
 * niepce - fwk/base/mod.rs
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

use std::collections::BTreeSet;

pub mod date;
#[macro_use]
pub mod debug;
pub mod fractions;
pub mod propertybag;
pub mod propertyvalue;
pub mod rgbcolour;

pub type PropertyIndex = u32;
pub type PropertySet = BTreeSet<PropertyIndex>;

#[no_mangle]
pub extern "C" fn fwk_property_set_new() -> *mut PropertySet {
    Box::into_raw(Box::new(PropertySet::new()))
}

#[no_mangle]
pub extern "C" fn fwk_property_set_delete(set: *mut PropertySet) {
    unsafe { Box::from_raw(set); }
}

#[no_mangle]
pub extern "C" fn fwk_property_set_add(set: &mut PropertySet, v: PropertyIndex) {
    set.insert(v);
}
