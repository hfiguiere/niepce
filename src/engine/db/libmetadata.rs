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
use std::mem::transmute;
use rusqlite;
use chrono::Utc;
use exempi;

use fwk::{
    PropertyValue,
    PropertyBag,
    PropertySet,
    XmpMeta,
    xmp_date_from
};
use fwk::utils::exempi::{NS_XAP, NS_DC};
use super::{
    FromDb,
    LibraryId
};
use root::eng::NiepceProperties as Np;
use engine::db::libfile::FileType;

pub struct LibMetadata {
    xmp: XmpMeta,
    id: LibraryId,
    pub sidecars: Vec<String>,
    pub file_type: FileType,
    pub name: String,
    pub folder: String,
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
        LibMetadata {
            xmp: XmpMeta::new(),
            id: id,
            sidecars: vec![], file_type: FileType::UNKNOWN,
            name: String::new(),
            folder: String::new()
        }
    }

    pub fn new_with_xmp(id: LibraryId, xmp: XmpMeta) -> LibMetadata {
        LibMetadata {
            xmp: xmp,
            id: id,
            sidecars: vec![],
            file_type: FileType::UNKNOWN,
            name: String::new(),
            folder: String::new()
        }
    }

    pub fn serialize_inline(&self) -> String {
        self.xmp.serialize_inline()
    }

    fn get_metadata(&self, meta: Np) -> Option<PropertyValue> {

        let index_to_xmp = try_opt!(property_index_to_xmp(meta));

        let mut prop_flags = exempi::PROP_NONE;
        let mut xmp_result = self.xmp.xmp.get_property(&index_to_xmp.ns, &index_to_xmp.property,
                                                       &mut prop_flags);
        if xmp_result.is_some() && prop_flags.contains(exempi::ARRAY_IS_ALTTEXT) {
            let mut value = exempi::XmpString::new();
            let mut actual_lang = exempi::XmpString::new();
            if self.xmp.xmp.get_localized_text(&index_to_xmp.ns, &index_to_xmp.property, "",
                                               "x-default", &mut actual_lang,
                                               &mut value, &mut prop_flags) {
                xmp_result = Some(value);
            }
        }
        Some(PropertyValue::String(String::from(try_opt!(xmp_result).to_str())))
    }

    pub fn set_metadata(&mut self, meta: Np, value: &PropertyValue) -> bool {
        if let Some(ix) = property_index_to_xmp(meta) {
            match value {
                &PropertyValue::Empty =>
                    return self.xmp.xmp.delete_property(&ix.ns, &ix.property),
                &PropertyValue::Int(i) =>
                    return self.xmp.xmp.set_property_i32(
                        &ix.ns, &ix.property, i, exempi::PROP_NONE),
                &PropertyValue::String(ref s) => {
                    if s.is_empty() {
                        return self.xmp.xmp.delete_property(&ix.ns, &ix.property);
                    } else if !self.xmp.xmp.set_property(
                        &ix.ns, &ix.property, s, exempi::PROP_NONE) {
                        if exempi::get_error() == exempi::Error::BadXPath {
                            return self.xmp.xmp.set_localized_text(
                                &ix.ns, &ix.property, "", "x-default",
                                s, exempi::PROP_NONE);
                        }
                    } else {
                        return true;
                    }
                },
                &PropertyValue::StringArray(ref sa) => {
                    self.xmp.xmp.delete_property(&ix.ns, &ix.property);
                    for i in 0..sa.len() {
                        self.xmp.xmp.append_array_item(&ix.ns, &ix.property,
                                                       exempi::PROP_VALUE_IS_ARRAY,
                                                       &sa[i],
                                                       exempi::PROP_NONE);
                    }
                    return true;
                },
                &PropertyValue::Date(ref d) => {
                    let xmp_date = xmp_date_from(d);
                    return self.xmp.xmp.set_property_date(
                        &ix.ns, &ix.property, &xmp_date, exempi::PROP_NONE);
                }
            }
            err_out!("error setting property {}:{} {}", ix.ns, ix.property,
                     exempi::get_error() as u32);
            return false;
        }
        err_out!("Unknown property {:?}", meta);
        false
    }

    pub fn to_properties(&self, propset: &PropertySet) -> PropertyBag {
        let mut props = PropertyBag::new();
        for prop_id in propset {
            let prop_id_np: Np = unsafe { transmute(*prop_id) };
            match prop_id_np {
                Np::NpXmpRatingProp =>
                    if let Some(rating) = self.xmp.rating() {
                        props.set_value(*prop_id, PropertyValue::Int(rating));
                    },
                Np::NpXmpLabelProp =>
                    if let Some(label) = self.xmp.label() {
                        props.set_value(*prop_id, PropertyValue::String(label));
                    },
                Np::NpTiffOrientationProp =>
                    if let Some(orientation) = self.xmp.orientation() {
                        props.set_value(*prop_id, PropertyValue::Int(orientation));
                    },
                Np::NpExifDateTimeOriginalProp =>
                    if let Some(date) = self.xmp.creation_date() {
                        props.set_value(*prop_id, PropertyValue::Date(date));
                    },
                Np::NpIptcKeywordsProp => {
                    let mut iter = exempi::XmpIterator::new(&self.xmp.xmp, NS_DC,
                                                            "subject",
                                                            exempi::ITER_JUST_LEAF_NODES);
                    let mut keywords: Vec<String> = vec!();
                    let mut schema = exempi::XmpString::new();
                    let mut name = exempi::XmpString::new();
                    let mut value = exempi::XmpString::new();
                    let mut flags = exempi::PropFlags::empty();
                    while iter.next(&mut schema, &mut name, &mut value, &mut flags) {
                        keywords.push(String::from(value.to_str()));
                    }
                    props.set_value(*prop_id, PropertyValue::StringArray(keywords));
                }
                Np::NpFileNameProp => {
                    props.set_value(*prop_id, PropertyValue::String(self.name.clone()));
                }
                Np::NpFileTypeProp => {
                    // XXX this to string convert should be elsewhere
                    let file_type = match self.file_type {
                        FileType::UNKNOWN => "Unknown",
                        FileType::RAW => "RAW",
                        FileType::RAW_JPEG => "RAW + JPEG",
                        FileType::IMAGE => "Image",
                        FileType::VIDEO => "Video",
                    };
                    props.set_value(*prop_id, PropertyValue::String(String::from(file_type)));
                }
                Np::NpFileSizeProp => {}
                Np::NpFolderProp => {
                    props.set_value(*prop_id, PropertyValue::String(self.folder.clone()));
                }
                Np::NpSidecarsProp => {
                    props.set_value(*prop_id, PropertyValue::StringArray(self.sidecars.clone()));
                }
                _ => {
                    if let Some(propval) = self.get_metadata(prop_id_np) {
                        props.set_value(*prop_id, propval);
                    } else {
                        dbg_out!("missing prop {}", prop_id);
                    }
                }
            }
        }
        props
    }

    pub fn touch(&mut self) -> bool {
        let xmpdate = xmp_date_from(&Utc::now());
        return self.xmp
            .xmp
            .set_property_date(NS_XAP, "MetadataDate", &xmpdate, exempi::PROP_NONE);
    }
}

impl FromDb for LibMetadata {
    fn read_db_columns() -> &'static str {
        "files.id,xmp,file_type,files.name,folders.name"
    }

    fn read_db_tables() -> &'static str {
        "files LEFT JOIN folders ON folders.id = files.parent_id"
    }

    fn read_from(row: &rusqlite::Row) -> Self {
        let id: LibraryId = row.get(0);
        let xmp: String = row.get(1);

        let mut xmpmeta = XmpMeta::new();
        xmpmeta.unserialize(&xmp);
        let mut libmeta = LibMetadata::new_with_xmp(id, xmpmeta);
        let col: i32 = row.get(2);
        libmeta.file_type = FileType::from(col);
        libmeta.name = row.get(3);
        libmeta.folder = row.get(4);
        libmeta
    }
}

#[no_mangle]
pub extern "C" fn engine_libmetadata_get_id(meta: &LibMetadata) -> LibraryId {
    meta.id
}

#[no_mangle]
pub extern "C" fn engine_libmetadata_to_properties(meta: &LibMetadata, propset: &PropertySet)
                                        -> *mut PropertyBag {
    let result = Box::new(meta.to_properties(propset));
    Box::into_raw(result)
}
