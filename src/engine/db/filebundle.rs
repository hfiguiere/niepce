/*
 * niepce - engine/db/filebundle.rs
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

use libc::c_char;
use std::ffi::CStr;
use std::ffi::CString;
use std::ffi::OsString;
use std::path::Path;

use engine::db::libfile::FileType;
use fwk::MimeType;
use fwk::FileList;

pub struct FileBundle {
    file_type: FileType,
    main: String,
    pub main_c: CString,
    xmp_sidecar: String,
    pub xmp_sidecar_c: CString,
    jpeg: String,
    pub jpeg_c: CString,
}

impl FileBundle {

    pub fn new() -> FileBundle {
        FileBundle {
            file_type: FileType::UNKNOWN,
            main: String::from(""),
            main_c: CString::new("").unwrap(),
            xmp_sidecar: String::from(""),
            xmp_sidecar_c: CString::new("").unwrap(),
            jpeg: String::from(""),
            jpeg_c: CString::new("").unwrap(),
        }
    }

    pub fn filter_bundles(files: &mut FileList) -> Vec<FileBundle> {
        let mut bundles: Vec<FileBundle> = vec!();
        unsafe { files.sort(); }
        let len = unsafe { files.size() };
        let mut current_base = OsString::new();
        let mut current_bundle: Option<FileBundle> = None;

        for i in 0..len {
            let f = unsafe { files.at_cstr(i) };
            let cstr = unsafe { CStr::from_ptr(f) }.to_string_lossy();
            let path = Path::new(&*cstr);
            if let Some(basename) = path.file_stem() {
                if basename == current_base {
                    current_bundle.as_mut().unwrap().add(&basename.to_string_lossy());
                } else {
                    if current_bundle.is_some() {
                        bundles.push(current_bundle.unwrap());
                    }
                    let mut bundle = FileBundle::new();
                    bundle.add(&cstr);
                    current_base = basename.to_os_string();
                    current_bundle = Some(bundle);
                }
            }
        }
        if current_bundle.is_some() {
            bundles.push(current_bundle.unwrap());
        }
        bundles
    }

    pub fn add(&mut self, path: &str) -> bool {
        dbg_out!("path {}", path);
        let mime_type = MimeType::new(path);
        let mut added = true;

        if mime_type.is_image() {
            if mime_type.is_digicam_raw() {
                println!("is RAW");
                if !self.main.is_empty() && self.jpeg.is_empty() {
                    println!("+JPEG");
                    self.jpeg = self.main.clone();
                    self.file_type = FileType::RAW_JPEG;
                } else {
                    println!("just RAW");
                    self.file_type = FileType::RAW;
                }
                self.main = String::from(path);
            } else {
                println!("is JPEG?");
                if !self.main.is_empty() {
                    println!("RAW+JPEG");
                    self.jpeg = String::from(path);
                    self.file_type = FileType::RAW_JPEG;
                } else {
                    println!("JPEG");
                    self.main = String::from(path);
                    self.file_type = FileType::IMAGE;
                }
            }
        } else if mime_type.is_xmp() {
            self.xmp_sidecar = String::from(path);
        } else if mime_type.is_movie() {
            self.main = String::from(path);
            self.file_type = FileType::VIDEO;
        } else {
            dbg_out!("Unknown file {} of type {:?}", path, mime_type);
            added = false;
        }
        added
    }

    pub fn main(&self) -> &str {
        &self.main
    }

    pub fn jpeg(&self) -> &str {
        &self.jpeg
    }

    pub fn sidecar(&self) -> &str {
        &self.xmp_sidecar
    }
}

#[no_mangle]
pub extern fn engine_db_filebundle_new() -> *mut FileBundle {
    let fb = Box::new(FileBundle::new());
    Box::into_raw(fb)
}

#[no_mangle]
pub extern fn engine_db_filebundle_delete(fb: *mut FileBundle) {
    unsafe { Box::from_raw(fb) };
}

#[no_mangle]
pub extern fn engine_db_filebundle_sidecar(obj: &mut FileBundle) -> *const c_char {
    obj.xmp_sidecar_c = CString::new(obj.sidecar()).unwrap();
    obj.xmp_sidecar_c.as_ptr()
}

#[no_mangle]
pub extern fn engine_db_filebundle_main(obj: &mut FileBundle) -> *const c_char {
    obj.main_c = CString::new(obj.main()).unwrap();
    obj.main_c.as_ptr()
}

#[no_mangle]
pub extern fn engine_db_filebundle_jpeg(obj: &mut FileBundle) -> *const c_char {
    obj.jpeg_c = CString::new(obj.jpeg()).unwrap();
    obj.jpeg_c.as_ptr()
}

#[no_mangle]
pub extern fn engine_db_filebundle_add(obj: &mut FileBundle, f: *const c_char) -> c_char {
    obj.add(&*unsafe { CStr::from_ptr(f) }.to_string_lossy()) as c_char
}
