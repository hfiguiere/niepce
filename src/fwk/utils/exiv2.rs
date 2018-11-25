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

use super::exempi::{Flash, XmpMeta, NS_TIFF, NS_EXIF, NS_XAP, NS_AUX, xmp_date_from_exif};

/// Property conversion rules
#[derive(Clone, Copy, Debug)]
enum Conversion {
    /// Use value as is
    None,
    /// Convert from the Exif date format to a date
    ExifDate,
    /// Convert the Exif flash tag to the struct for XMP
    Flash,
}

/// Converstion result
#[derive(Clone, Debug)]
enum Converted {
    /// A String
    Str(String),
    /// An int 32
    Int(i32),
    /// an Xmp DateTime value
    Date(Option<exempi::DateTime>),
    /// The Flash XMP structure
    Flash(Flash),
}

lazy_static! {
    static ref exiv2_to_xmp: HashMap<&'static str, (&'static str, &'static str, Conversion)> = {
        [
            ("Exif.Image.DateTime", (NS_XAP, "ModifyDate", Conversion::ExifDate)),
            ("Exif.Image.ImageHeight", (NS_TIFF, "ImageHeight", Conversion::None)),
            ("Exif.Image.ImageWidth", (NS_TIFF, "ImageWidth", Conversion::None)),
            ("Exif.Image.Make", (NS_TIFF, "Make", Conversion::None)),
            ("Exif.Image.Model", (NS_TIFF, "Model", Conversion::None)),
            ("Exif.Image.Orientation", (NS_TIFF, "Orientation", Conversion::None)),
            ("Exif.Photo.ApertureValue", (NS_EXIF, "ApertureValue", Conversion::None)),
            ("Exif.Photo.ColorSpace", (NS_EXIF, "ColorSpace", Conversion::None)),
            ("Exif.Photo.DateTimeOriginal", (NS_EXIF, "DateTimeOriginal", Conversion::ExifDate)),
            ("Exif.Photo.DateTimeDigitized", (NS_XAP, "CreateDate", Conversion::ExifDate)),
            ("Exif.Photo.ExposureBiasValue", (NS_EXIF, "ExposureBiasValue", Conversion::None)),
            ("Exif.Photo.ExposureMode", (NS_EXIF, "ExposureMode", Conversion::None)),
            ("Exif.Photo.ExposureProgram", (NS_EXIF, "ExposureProgram", Conversion::None)),
            ("Exif.Photo.ExposureTime", (NS_EXIF, "ExposureTime", Conversion::None)),
            ("Exif.Photo.FNumber", (NS_EXIF, "FNumber", Conversion::None)),
            ("Exif.Photo.Flash", (NS_EXIF, "Flash", Conversion::Flash)),
            ("Exif.Photo.FocalLength", (NS_EXIF, "FocalLength", Conversion::None)),
            ("Exif.Photo.ISOSpeedRatings", (NS_EXIF, "ISOSpeedRatings", Conversion::None)),
            ("Exif.Photo.LightSource", (NS_EXIF, "LightSource", Conversion::None)),
            ("Exif.Photo.MeteringMode", (NS_EXIF, "MeteringMode", Conversion::None)),
            ("Exif.Photo.SceneCaptureType", (NS_EXIF, "SceneCaptureType", Conversion::None)),
            ("Exif.Photo.ShutterSpeedValue", (NS_EXIF, "ShutterSpeedValue", Conversion::None)),
            ("Exif.Photo.UserComment", (NS_EXIF, "UserComment", Conversion::None)),
            ("Exif.Photo.WhiteBalance", (NS_EXIF, "WhiteBalance", Conversion::None)),

            ("Exif.Canon.LensModel", (NS_AUX, "Lens", Conversion::None)),

            ("Exif.OlympusEq.LensModel", (NS_AUX, "Lens", Conversion::None)),
            ("Exif.OlympusEq.LensSerialNumber", (NS_AUX, "LensSerialNumber", Conversion::None))
        ].iter().cloned().collect()
    };
}

fn convert(conversion: Conversion, value: &str) -> Converted {
    match conversion {
        Conversion::None => Converted::Str(value.to_string()),
        Conversion::ExifDate => Converted::Date(xmp_date_from_exif(&value)),
        _ => {
            err_out!("Conversion error fro str to {:?}", conversion);
            Converted::Str(value.to_string())
        }
    }
}

fn convert_int(conversion: Conversion, int: i32) -> Converted {
    match conversion {
        Conversion::None => Converted::Int(int),
        Conversion::Flash => Converted::Flash(Flash::from(int)),
        _ => {
            err_out!("Conversion error from int to {:?}", conversion);
            Converted::Int(int)
        }
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
                                },
                                _ => {
                                    err_out!("Unknown conversion result {:?}", &converted);
                                }
                            }
                        }
                    },
                    Ok(rexiv2::TagType::UnsignedShort) |
                    Ok(rexiv2::TagType::UnsignedLong) |
                    Ok(rexiv2::TagType::SignedShort) |
                    Ok(rexiv2::TagType::SignedLong) => {
                        // XXX rexiv2 returns an i32, which is a problem for UnsignedLong
                        match xmp_prop.2 {
                            Conversion::Flash => {
                                if let Converted::Flash(flash) = convert_int(xmp_prop.2, meta.get_tag_numeric(&tag)) {
                                    flash.set_as_xmp_property(&mut xmp, NS_EXIF, "Flash");
                                }
                            },
                            Conversion::None => {
                                let value = meta.get_tag_numeric(&tag);
                                xmp.set_property_i32(xmp_prop.0, xmp_prop.1, value, exempi::PROP_NONE);
                            },
                            _ => {
                                err_out!("Unknown conversion from {:?} to {:?}", tagtype, xmp_prop.2);
                                let value = meta.get_tag_numeric(&tag);
                                xmp.set_property_i32(xmp_prop.0, xmp_prop.1, value, exempi::PROP_NONE);
                            }
                        }
                    },
                    Ok(rexiv2::TagType::UnsignedRational) |
                    Ok(rexiv2::TagType::SignedRational) => {
                        if let Some(value) = meta.get_tag_rational(&tag) {
                            let value_str = format!("{}/{}", value.numer(), value.denom());
                            xmp.set_property(xmp_prop.0, xmp_prop.1, &value_str, exempi::PROP_NONE);
                        }
                    },
                    Ok(rexiv2::TagType::Comment) => {
                        if let Ok(value) = meta.get_tag_string(&tag) {
                            xmp.set_property(xmp_prop.0, xmp_prop.1, &value, exempi::PROP_NONE);
                        }
                    },
                    _ => {
                        err_out!("Unhandled type {:?} for {}", tagtype, &tag);
                    }
                }
            } else {
                err_out!("Unknown property {}", &tag);
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

