/*
 * niepce - fwk/utils/exiv2.rs
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

use std::ffi::OsStr;
use std::collections::HashMap;

use exempi;
use rexiv2;

use super::exempi::{XmpMeta, NS_TIFF, NS_EXIF, NS_XAP, xmp_date_from_exif};

#[derive(Clone, Copy)]
enum Conversion {
    None,
    ExifDate,
}

#[derive(Clone)]
enum Converted {
    Str(String),
    Date(Option<exempi::DateTime>),
}

lazy_static! {
    static ref exiv2_to_xmp: HashMap<&'static str, (&'static str, &'static str, Conversion)> = {
        [
            ("Exif.Photo.DateTimeOriginal", (NS_EXIF, "DateTimeOriginal", Conversion::ExifDate)),
            ("Exif.Photo.DateTimeDigitized", (NS_XAP, "CreateDate", Conversion::ExifDate)),
            ("Exif.Image.DateTime", (NS_XAP, "ModifyDate", Conversion::ExifDate)),
            ("Exif.Image.Make", (NS_TIFF, "Make", Conversion::None)),
            ("Exif.Image.Model", (NS_TIFF, "Model", Conversion::None))
        ].iter().cloned().collect()
    };
}

fn convert(conversion: Conversion, value: &str) -> Converted {
    match conversion {
        Conversion::None => Converted::Str(value.to_string()),
        Conversion::ExifDate => Converted::Date(xmp_date_from_exif(&value)),
    }
}

pub fn xmp_from_exiv2<S: AsRef<OsStr>>(file: S) -> Option<XmpMeta> {
    if let Ok(meta) = rexiv2::Metadata::new_from_path(file) {
        let mut xmp = exempi::Xmp::new();
        let mut all_tags : Vec<String> = vec!();
        if let Ok(mut tags) = meta.get_exif_tags() {
            all_tags.append(&mut tags);
        }
        if let Ok(mut tags) = meta.get_iptc_tags() {
            all_tags.append(&mut tags);
        }
        if let Ok(mut tags) = meta.get_xmp_tags() {
            all_tags.append(&mut tags);
        }
        for tag in all_tags {
            if let Some(xmp_prop) = exiv2_to_xmp.get(tag.as_str()) {
                let tagtype = rexiv2::get_tag_type(&tag);
                match tagtype {
                    Ok(rexiv2::TagType::AsciiString) => {
                        if let Ok(value) = meta.get_tag_string(&tag) {
                            let converted = convert(xmp_prop.2, &value);
                            match converted {
                                Converted::Str(s) => {
                                    xmp.set_property(xmp_prop.0, xmp_prop.1, &s, exempi::PROP_NONE);
                                },
                                Converted::Date(d) => {
                                    if let Some(d) = d {
                                        xmp.set_property_date(xmp_prop.0, xmp_prop.1, &d, exempi::PROP_NONE);
                                    } else {
                                        err_out!("Couldn't convert Exif date {}", &value);
                                    }
                                }
                            }
                        }
                    },
                    Ok(rexiv2::TagType::UnsignedShort) |
                    Ok(rexiv2::TagType::UnsignedLong) |
                    Ok(rexiv2::TagType::SignedShort) |
                    Ok(rexiv2::TagType::SignedLong) => {
                        // XXX rexiv2 returns an i32, which is a problem for UnsignedLong
                        let value = meta.get_tag_numeric(&tag);
                        xmp.set_property_i32(xmp_prop.0, xmp_prop.1, value, exempi::PROP_NONE);
                    },
                    Ok(rexiv2::TagType::UnsignedRational) |
                    Ok(rexiv2::TagType::SignedRational) => {
                        if let Some(value) = meta.get_tag_rational(&tag) {
                            let value_str = format!("{}/{}", value.numer(), value.denom());
                            xmp.set_property(xmp_prop.0, xmp_prop.1, &value_str, exempi::PROP_NONE);
                        }
                        println!("value {} = {:?}", &tag, meta.get_tag_rational(&tag));
                    },
                    _ => {
                        println!("Unhandled type {:?} for {}", tagtype, &tag);
                    }
                }
            } else {
                println!("Unknown property {}", &tag);
            }
        }
        meta.get_gps_info();

        let mut options = exempi::PROP_NONE;
        if let Ok(date) = xmp.get_property_date(NS_XAP, "ModifyDate", &mut options) {
            if xmp.set_property_date(NS_XAP, "MetadataDate", &date, exempi::PROP_NONE).is_err() {
                err_out!("Error setting MetadataDate");
            }
        } else {
            err_out!("Couldn't get the ModifyDate");
        }
        Some(XmpMeta::new_with_xmp(xmp))
    } else {
        None
    }
}

