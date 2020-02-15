/*
 * niepce - fwk/base/propertybag.rs
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

use std::collections::BTreeMap;
use std::ptr;

use crate::base::propertyvalue::PropertyValue;
use crate::base::PropertyIndex;

#[derive(Default)]
pub struct PropertyBag {
    pub bag: Vec<PropertyIndex>,
    pub map: BTreeMap<PropertyIndex, PropertyValue>,
}

impl PropertyBag {
    pub fn new() -> Self {
        PropertyBag::default()
    }

    pub fn is_empty(&self) -> bool {
        self.bag.is_empty()
    }

    pub fn len(&self) -> usize {
        self.bag.len()
    }

    pub fn set_value(&mut self, key: PropertyIndex, value: PropertyValue) -> bool {
        let ret = self.map.insert(key, value);
        if ret.is_some() {
            return true;
        }
        self.bag.push(key);
        false
    }
}

#[no_mangle]
pub extern "C" fn fwk_property_bag_new() -> *mut PropertyBag {
    Box::into_raw(Box::new(PropertyBag::new()))
}

/// Delete the %PropertyBag object
///
/// # Safety
/// Dereference the raw pointer.
#[no_mangle]
pub unsafe extern "C" fn fwk_property_bag_delete(bag: *mut PropertyBag) {
    Box::from_raw(bag);
}

#[no_mangle]
pub extern "C" fn fwk_property_bag_is_empty(b: &PropertyBag) -> bool {
    b.is_empty()
}

#[no_mangle]
pub extern "C" fn fwk_property_bag_len(b: &PropertyBag) -> usize {
    b.len()
}

#[no_mangle]
pub extern "C" fn fwk_property_bag_key_by_index(b: &PropertyBag, idx: usize) -> PropertyIndex {
    b.bag[idx]
}

#[no_mangle]
pub extern "C" fn fwk_property_bag_value(
    b: &PropertyBag,
    key: PropertyIndex,
) -> *mut PropertyValue {
    if b.map.contains_key(&key) {
        let value = Box::new(b.map[&key].clone());
        Box::into_raw(value)
    } else {
        ptr::null_mut()
    }
}

#[no_mangle]
pub extern "C" fn fwk_property_bag_set_value(
    b: &mut PropertyBag,
    key: PropertyIndex,
    v: &PropertyValue,
) -> bool {
    b.set_value(key, v.clone())
}
