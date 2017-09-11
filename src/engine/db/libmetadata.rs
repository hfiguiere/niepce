/*
 * niepce - eng/db/libmetadata.rs
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

use std::ffi::CStr;
use rusqlite;
use exempi;

use fwk;
use fwk::{
    XmpMeta,
    make_xmp_date_time
};
use fwk::utils::exempi::NS_XAP;
use super::{
    FromDb,
    LibraryId
};
use root::eng::NiepceProperties as Np;
use root::fwk::{
    is_empty,
    is_integer,
    is_string,
    is_string_array,
    is_date,
    get_string_cstr,
    get_string_array,
    get_integer,
    get_date,
    string_array_len,
    string_array_at_cstr,
    PropertyValue
};

pub struct LibMetadata {
    xmp: XmpMeta,
    id: LibraryId
}

struct IndexToXmp {
    pub ns: String,
    pub property: String
}

fn property_index_to_xmp(meta: Np) -> Option<IndexToXmp> {
    let index = unsafe { ::root::eng::property_index_to_xmp(meta as u32) };
    if index.ns.is_null() || index.property.is_null() {
        err_out!("property {} not found", meta as u32);
        return None;
    }
    Some(IndexToXmp {
        ns: String::from(unsafe { CStr::from_ptr(index.ns) }.to_string_lossy()),
        property: String::from(unsafe { CStr::from_ptr(index.property) }.to_string_lossy())
    })
}


impl LibMetadata {

    pub fn new(id: LibraryId) -> LibMetadata {
        LibMetadata{
            xmp: XmpMeta::new(),
            id: id
        }
    }

    pub fn new_with_xmp(id: LibraryId, xmp: XmpMeta) -> LibMetadata {
        LibMetadata{
            xmp: xmp,
            id: id
        }
    }

    pub fn serialize_inline(&self) -> String {
        self.xmp.serialize_inline()
    }

    pub fn set_metadata(&mut self, meta: Np, value: &PropertyValue) -> bool {
        if let Some(ix) = property_index_to_xmp(meta) {
            if unsafe { is_empty(value) } {
                return self.xmp.xmp.delete_property(&ix.ns, &ix.property);
            } else if unsafe { is_integer(value) } {
                return self.xmp.xmp.set_property_i32(
                    &ix.ns, &ix.property, unsafe { get_integer(value) }, exempi::PROP_NONE);
            } else if unsafe { is_string(value) } {
                let cstr = unsafe { CStr::from_ptr(get_string_cstr(value)) };
                if cstr.to_bytes().len() == 0 {
                    return self.xmp.xmp.delete_property(&ix.ns, &ix.property);
                } else if !self.xmp.xmp.set_property(
                    &ix.ns, &ix.property, &*cstr.to_string_lossy(), exempi::PROP_NONE) {
                    if exempi::get_error() == exempi::Error::BadXPath {
                        return self.xmp.xmp.set_localized_text(
                            &ix.ns, &ix.property, "", "x-default",
                            &*cstr.to_string_lossy(), exempi::PROP_NONE);
                    }
                } else {
                    return true;
                }
            } else if unsafe { is_string_array(value) } {
                self.xmp.xmp.delete_property(&ix.ns, &ix.property);
                let a = unsafe { get_string_array(value) };
                let count = unsafe { string_array_len(a) };
                for i in 0..count {
                    let cstr = unsafe { CStr::from_ptr(string_array_at_cstr(a, i)) };
                    self.xmp.xmp.append_array_item(&ix.ns, &ix.property,
                                                   exempi::ARRAY_NONE,
                                                   &*cstr.to_string_lossy(),
                                                   exempi::ITEM_NONE);
                }
                return true;
            } else if unsafe { is_date(value) } {
                let d = unsafe { get_date(value) } as *const fwk::Date;
                return self.xmp.xmp.set_property_date(
                    &ix.ns, &ix.property, unsafe { &(*d) }.xmp_date(), exempi::PROP_NONE);
            }
            err_out!("error setting property {}:{} {}", ix.ns, ix.property,
                     exempi::get_error() as u32);
        }
        err_out!("Unknown property {:?}", meta);
        false
    }

    pub fn touch(&mut self) -> bool {
        let mut xmpdate = exempi::DateTime::new();
        if make_xmp_date_time(fwk::Date::now(), &mut xmpdate) {
            return self.xmp.xmp.set_property_date(NS_XAP, "MetadataDate",
                                                  &xmpdate, exempi::PROP_NONE);
        }
        false
    }
}

impl FromDb for LibMetadata {

    fn read_db_columns() -> &'static str {
        "id,xmp"
    }

    fn read_db_tables() -> &'static str {
        "files"
    }

    fn read_from(row: &rusqlite::Row) -> Self {
        let id: LibraryId = row.get(0);
        let xmp: String = row.get(1);

        let mut xmpmeta = XmpMeta::new();
        xmpmeta.unserialize(&xmp);
        LibMetadata::new_with_xmp(id, xmpmeta)
    }

}

#[no_mangle]
pub fn engine_libmetadata_get_id(meta: &LibMetadata) -> LibraryId {
    return meta.id;
}

#[no_mangle]
pub fn engine_libmetadata_get_xmp(meta: &LibMetadata) -> exempi::Xmp {
    return meta.xmp.xmp.clone();
}

#[no_mangle]
pub fn engine_libmetadata_get_xmpmeta(meta: &LibMetadata) -> *const XmpMeta {
    return &meta.xmp;
}
