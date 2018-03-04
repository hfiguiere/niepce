/*
 * niepce - engine/db/filebundle.rs
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

use std::ffi::OsString;
use std::path::Path;

use engine::db::libfile::FileType;
use fwk::MimeType;

/// Sidecar.
#[derive(Debug, PartialEq)]
pub enum Sidecar {
    /// Sidecar for Live image (MOV file form iPhone)
    Live(String),
}

/// FileBundle is a set of physical files group as one item.
/// Mostly sticking to the DCF specification.
pub struct FileBundle {
    /// Type of bundle
    bundle_type: FileType,
    /// Main file.
    main: String,
    /// XMP sidecar if it exists.
    xmp_sidecar: String,
    /// JPEG alternate for RAW_JPEG
    jpeg: String,
    /// Other sidecars: Live, Thumbnail
    sidecars: Vec<Sidecar>,
}

impl FileBundle {
    pub fn new() -> FileBundle {
        FileBundle {
            bundle_type: FileType::UNKNOWN,
            main: String::new(),
            xmp_sidecar: String::new(),
            jpeg: String::new(),
            sidecars: vec![],
        }
    }

    pub fn filter_bundles(files: &Vec<String>) -> Vec<FileBundle> {
        let mut bundles: Vec<FileBundle> = vec![];
        let mut sorted_files: Vec<&String> = files.iter().collect();
        sorted_files.sort();
        let mut current_base = OsString::new();
        let mut current_bundle: Option<FileBundle> = None;

        for f in sorted_files {
            let path = Path::new(&f);
            if let Some(basename) = path.file_stem() {
                if basename == current_base {
                    current_bundle.as_mut().unwrap().add(&f);
                } else {
                    if current_bundle.is_some() {
                        bundles.push(current_bundle.unwrap());
                    }
                    let mut bundle = FileBundle::new();
                    bundle.add(&f);
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
        dbg_out!("FileBundle::add path {}", path);
        let mime_type = MimeType::new(path);
        let mut added = true;

        if mime_type.is_image() {
            if mime_type.is_digicam_raw() {
                if !self.main.is_empty() && self.jpeg.is_empty() {
                    self.jpeg = self.main.clone();
                    self.bundle_type = FileType::RAW_JPEG;
                } else {
                    self.bundle_type = FileType::RAW;
                }
                self.main = String::from(path);
            } else {
                if !self.main.is_empty() {
                    self.jpeg = String::from(path);
                    self.bundle_type = FileType::RAW_JPEG;
                } else {
                    self.main = String::from(path);
                    self.bundle_type = FileType::IMAGE;
                }
            }
        } else if mime_type.is_xmp() {
            self.xmp_sidecar = String::from(path);
        } else if mime_type.is_movie() {
            match self.bundle_type {
                FileType::UNKNOWN => {
                    self.main = String::from(path);
                    self.bundle_type = FileType::VIDEO;
                }
                FileType::IMAGE => {
                    self.sidecars.push(Sidecar::Live(String::from(path)));
                }
                _ => {
                    dbg_out!("Ignoring movie file {}", path);
                    added = false;
                }
            }
        } else {
            dbg_out!("Unknown file {} of type {:?}", path, mime_type);
            added = false;
        }
        added
    }

    pub fn bundle_type(&self) -> FileType {
        self.bundle_type
    }

    pub fn main(&self) -> &str {
        &self.main
    }

    pub fn jpeg(&self) -> &str {
        &self.jpeg
    }

    pub fn xmp_sidecar(&self) -> &str {
        &self.xmp_sidecar
    }

    pub fn sidecars(&self) -> &Vec<Sidecar> {
        &self.sidecars
    }
}

#[cfg(test)]
mod test {
    use super::{FileBundle, Sidecar};
    use engine::db::libfile::FileType;

    #[test]
    fn test_filebundle() {
        let mut thelist: Vec<String> = vec![];

        thelist.push(String::from("/foo/bar/img_0001.cr2"));
        thelist.push(String::from("/foo/bar/img_0001.jpg"));
        thelist.push(String::from("/foo/bar/img_0001.xmp"));

        thelist.push(String::from("/foo/bar/dcs_0001.jpg"));
        thelist.push(String::from("/foo/bar/dcs_0001.nef"));
        thelist.push(String::from("/foo/bar/dcs_0001.xmp"));

        thelist.push(String::from("/foo/bar/img_0142.jpg"));
        thelist.push(String::from("/foo/bar/img_0142.mov"));

        thelist.push(String::from("/foo/bar/img_0143.mov"));
        thelist.push(String::from("/foo/bar/img_0143.jpg"));

        let bundles_list = FileBundle::filter_bundles(&thelist);

        assert_eq!(bundles_list.len(), 4);

        let mut iter = bundles_list.iter();
        if let Some(b) = iter.next() {
            assert_eq!(b.bundle_type(), FileType::RAW_JPEG);
            assert_eq!(b.main(), "/foo/bar/dcs_0001.nef");
            assert_eq!(b.jpeg(), "/foo/bar/dcs_0001.jpg");
            assert_eq!(b.xmp_sidecar(), "/foo/bar/dcs_0001.xmp");
        } else {
            assert!(false);
        }

        if let Some(b) = iter.next() {
            assert_eq!(b.bundle_type(), FileType::RAW_JPEG);
            assert_eq!(b.main(), "/foo/bar/img_0001.cr2");
            assert_eq!(b.jpeg(), "/foo/bar/img_0001.jpg");
            assert_eq!(b.xmp_sidecar(), "/foo/bar/img_0001.xmp");
        } else {
            assert!(false);
        }

        if let Some(b) = iter.next() {
            assert_eq!(b.bundle_type(), FileType::IMAGE);
            assert_eq!(b.main(), "/foo/bar/img_0142.jpg");
            assert!(b.jpeg().is_empty());
            assert!(b.xmp_sidecar().is_empty());
            assert_eq!(b.sidecars.len(), 1);
            assert_eq!(
                b.sidecars[0],
                Sidecar::Live(String::from("/foo/bar/img_0142.mov"))
            );
        } else {
            assert!(false);
        }

        if let Some(b) = iter.next() {
            assert_eq!(b.bundle_type(), FileType::IMAGE);
            assert_eq!(b.main(), "/foo/bar/img_0143.jpg");
            assert!(b.jpeg().is_empty());
            assert!(b.xmp_sidecar().is_empty());
            assert_eq!(b.sidecars.len(), 1);
            assert_eq!(
                b.sidecars[0],
                Sidecar::Live(String::from("/foo/bar/img_0143.mov"))
            );
        } else {
            assert!(false);
        }
    }
}
