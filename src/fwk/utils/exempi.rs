/*
 * niepce - fwk/utils/exempi.rs
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

use std::fs::File;
use std::io::prelude::*;
use std::path::Path;
use exempi;
use exempi::Xmp;


static NIEPCE_XMP_NAMESPACE: &'static str = "http://xmlns.figuiere.net/ns/niepce/1.0";
static NIEPCE_XMP_NS_PREFIX: &'static str = "niepce";
static UFRAW_INTEROP_NAMESPACE: &'static str = "http://xmlns.figuiere.net/ns/ufraw_interop/1.0";
static UFRAW_INTEROP_NS_PREFIX: &'static str = "ufrint";


static NS_TIFF: &'static str = "http://ns.adobe.com/tiff/1.0/";
static NS_XAP: &'static str = "http://ns.adobe.com/xap/1.0/";
static NS_EXIF: &'static str = "http://ns.adobe.com/exif/1.0/";
static NS_DC: &'static str = "http://purl.org/dc/elements/1.1/";

pub struct NsDef {
    ns: String,
    prefix: String,
}

pub struct ExempiManager {
}

impl ExempiManager {
    pub fn new(namespaces: Option<Vec<NsDef>>) -> ExempiManager {
        if exempi::init() {
            exempi::register_namespace(NIEPCE_XMP_NAMESPACE, NIEPCE_XMP_NS_PREFIX);
            exempi::register_namespace(UFRAW_INTEROP_NAMESPACE, UFRAW_INTEROP_NS_PREFIX);
        }
        if let Some(nslist) = namespaces {
            for nsdef in nslist {
                // TOOD check the return value
                exempi::register_namespace(nsdef.ns.as_str(), nsdef.prefix.as_str());
            }
        }
        ExempiManager {}
    }
}

impl Drop for ExempiManager {
    fn drop(&mut self) {
        exempi::terminate();
    }
}


pub struct XmpMeta {
    xmp: Xmp,
}


impl XmpMeta {
    pub fn new() -> XmpMeta {
        XmpMeta { xmp: exempi::Xmp::new() }
    }

    pub fn new_from_file(file: &str, sidecar_only: bool) -> Option<XmpMeta> {
        if !sidecar_only {
            let xmpfile = exempi::XmpFile::open_new(file, exempi::OPEN_READ);
            if xmpfile.is_some() {
                let xmp = xmpfile.unwrap().get_new_xmp();
                if xmp.is_some() {
                    return Some(XmpMeta { xmp: xmp.unwrap() });
                }
            }
        }
        let filepath = Path::new(file);
        let sidecar = filepath.with_extension("xmp");
        let sidecaropen = File::open(sidecar);
        if let Some(mut sidecarfile) = sidecaropen.ok() {
            let mut sidecarcontent = String::new();
            let result = sidecarfile.read_to_string(&mut sidecarcontent);
            if result.ok().is_some() {
                let mut xmp = exempi::Xmp::new();
                if xmp.parse(sidecarcontent.into_bytes().as_slice()) {
                    return Some(XmpMeta { xmp: xmp });
                }
            }
        }
        None
    }

    pub fn serialize_inline(&self) -> String {
        if let Some(xmpstr) = self.xmp.serialize_and_format(
            exempi::SERIAL_OMITPACKETWRAPPER | exempi::SERIAL_OMITALLFORMATTING,
            0, "", "", 0) {
            let buf = String::from(xmpstr.to_str());
            return buf;
        }
        String::new()
    }

    pub fn serialize(&self) -> String {
        if let Some(xmpstr) = self.xmp.serialize_and_format(
            exempi::SERIAL_OMITPACKETWRAPPER, 0, "\n", "", 0) {
            let buf = String::from(xmpstr.to_str());
            return buf;
        }
        String::new()
    }

    pub fn unserialize(&mut self, buf: &str) -> bool {
        self.xmp.parse(buf.as_bytes())
    }

    pub fn orientation(&self) -> Option<i32> {
        let mut flags: exempi::PropFlags = exempi::PropFlags::empty();
        return self.xmp.get_property_i32(NS_TIFF, "Orientation", &mut flags);
    }

    pub fn label(&self) -> Option<String> {
        let mut flags: exempi::PropFlags = exempi::PropFlags::empty();
        if let Some(xmpstring) = self.xmp.get_property(NS_XAP, "Label", &mut flags) {
            return Some(String::from(xmpstring.to_str()));
        }
        None
    }

    pub fn rating(&self) -> Option<i32> {
        let mut flags: exempi::PropFlags = exempi::PropFlags::empty();
        return self.xmp.get_property_i32(NS_XAP, "Rating", &mut flags);
    }

    pub fn flag(&self) -> Option<i32> {
        let mut flags: exempi::PropFlags = exempi::PropFlags::empty();
        return self.xmp.get_property_i32(NIEPCE_XMP_NAMESPACE, "Flag", &mut flags);
    }

    // XXX need fwk::Date()
    // pub fn creation_date()

    pub fn creation_date_str(&self) -> Option<String>
    {
        let mut flags: exempi::PropFlags = exempi::PropFlags::empty();
        if let Some(xmpstring) = self.xmp.get_property(NS_EXIF, "DateTimeOriginal", &mut flags) {
            return Some(String::from(xmpstring.to_str()));
        }
        None
    }

    // pub fn keywords
}

pub fn gps_coord_from_xmp(xmps: &str) -> Option<f64> {
    let mut current: &str = xmps;
    let degs: &str;

    // step 1 - degrees
    if let Some(sep) = current.find(',') {
        let (d, remainder) = current.split_at(sep);
        current = remainder;
        degs = d;
    } else {
        return None;
    }
    // step 2 - minutes
    if current.len() < 1 {
        return None;
    }
    // get rid of the comma
    let (_, current) = current.split_at(1);
    let orientation = match current.chars().last() {
        Some('N') | Some('E') =>
            1.0f64,
        Some('S') | Some('W') =>
            -1.0f64,
        _ => return None,
    };

    // extract minutes. There are two formats
    let fminutes: f64;
    if let Some(sep) = current.find(',') {
        // DD,mm,ss format
        if sep >= (current.len() - 1) {
            return None;
        }
        if current.len() <= 2 {
            return None;
        }
        let (minutes, seconds) = current.split_at(sep);
        let (_, seconds) = seconds.split_at(1);
        let (seconds, _) = seconds.split_at(seconds.len() - 1);
        if let Ok(m) = minutes.parse::<f64>() {
            if let Ok(s) = seconds.parse::<f64>() {
                fminutes = m + (s / 60f64);
            } else {
                return None;
            }
        } else {
            return None;
        }
    } else {
        // DD,mm.mm format
        let (minutes, _) = current.split_at(current.len() - 1);
        if let Ok(m) = minutes.parse::<f64>() {
            fminutes = m;
        } else {
            return None;
        }
    }

    if let Ok(mut deg) = degs.parse::<f64>() {
        if deg > 180.0 {
            return None;
        }
        deg += fminutes / 60.0;
        deg *= orientation;

        return Some(deg);
    }
    None
}


#[cfg(test)]
mod tests {
    #[test]
    fn xmp_meta_works() {
        use std::env;
        use std::path::PathBuf;
        use super::ExempiManager;
        use super::XmpMeta;

        let mut dir: PathBuf;
        if let Ok(pdir) = env::var("CARGO_MANIFEST_DIR") {
            dir = PathBuf::from(pdir);
            dir.push("src");
            dir.push("fwk");
            dir.push("utils");
        } else {
            dir = PathBuf::from(".");
        }
        dir.push("test.xmp");
        let xmpManager = ExempiManager::new(None);

        if let Some(xmpfile) = dir.to_str() {
            let meta = XmpMeta::new_from_file(xmpfile, true);

            assert!(meta.is_some());
            let meta = meta.unwrap();
            assert_eq!(meta.orientation().unwrap_or(0), 1);
            // test keywords()
        } else {
            assert!(false);
        }
    }

    #[test]
    fn gps_coord_from_works() {
        use super::gps_coord_from_xmp;

        let mut output = gps_coord_from_xmp("foobar");
        assert!(output.is_none());

        // malformed 1
        output = gps_coord_from_xmp("45,29.6681666667");
        assert!(output.is_none());

        // malformed 2
        output = gps_coord_from_xmp("45,W");
        assert!(output.is_none());

        // malformed 3
        output = gps_coord_from_xmp("45,29,N");
        assert!(output.is_none());

        // out of bounds
        output = gps_coord_from_xmp("200,29.6681666667N");
        assert!(output.is_none());

        // well-formed 1
        output = gps_coord_from_xmp("45,29.6681666667N");
        assert!(output.is_some());
        assert_eq!(output.unwrap(), 45.494469444445002181964810006320476531982421875);

        // well-formed 2
        output = gps_coord_from_xmp("73,38.2871666667W");
        assert!(output.is_some());
        assert_eq!(output.unwrap(), -73.6381194444449960201382054947316646575927734375);

        // well-formed 3
        output = gps_coord_from_xmp("45,29,30.45N");
        assert!(output.is_some());
        assert_eq!(output.unwrap(), 45.49179166666666418450404307805001735687255859375);
    }
}
