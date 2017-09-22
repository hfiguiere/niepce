
use gio;
use glib_sys;
use glib_sys::gboolean;
use gio_sys;
use gio::prelude::*;

use libc::c_void;
use std::ffi::CStr;
use std::ffi::CString;
use std::ptr;
use std::path::Path;

#[derive(PartialEq, Debug)]
pub enum IsRaw {
    No,
    Yes
}

#[derive(PartialEq, Debug)]
pub enum MType {
    None,
    Image(IsRaw),
    Movie,
    Xmp
}

#[derive(Debug)]
pub struct MimeType {
//    name: String,
    mtype: MType,
}

fn guess_type(gmtype: &str) -> MType {
    if gio::content_type_is_a(&gmtype, "image/*") {
        if gio::content_type_is_a(&gmtype, "image/x-dcraw") {
            return MType::Image(IsRaw::Yes);
        }
        return MType::Image(IsRaw::No);
    } else if gio::content_type_is_a(&gmtype, "video/*") {
        return MType::Movie;
    }
    MType::None
}

fn guess_type_for_file(filename: &str) -> MType {
    let path = Path::new(filename);
    let file = gio::File::new_for_path(path);
    if let Ok(fileinfo) = file.query_info("*", gio::FILE_QUERY_INFO_NONE, None) {
        if let Some(gmtype) = fileinfo.get_content_type() {
            let t = guess_type_for_file(&gmtype);
            if t != MType::None {
                return t;
            }
        }
    }
    if let Some(ext) = path.extension() {
        if let Some(sext) = ext.to_str() {
            if sext == "xmp" {
                return MType::Xmp;
            }
        }
    }
    // alternative
    let mut uncertainty: gboolean = 0;
    let content_type = unsafe {
        gio_sys::g_content_type_guess(CString::new(filename).unwrap().as_ptr(),
                                      ptr::null_mut(), 0, &mut uncertainty)
    };
    let content_type_real = unsafe { CStr::from_ptr(content_type) };
    let t = guess_type(&*content_type_real.to_string_lossy());
    unsafe {
        glib_sys::g_free(content_type as *mut c_void)
    };

    t
}

impl MimeType {

    pub fn new(filename: &str) -> MimeType {
        MimeType {
//            name: String::from(filename),
            mtype: guess_type_for_file(filename),
        }
    }

    pub fn is_image(&self) -> bool {
        if let MType::Image(_) = self.mtype {
            true
        } else {
            false
        }
    }

    pub fn is_digicam_raw(&self) -> bool {
        if let MType::Image(ref b) = self.mtype {
            return *b == IsRaw::Yes;
        }
        false
    }

    pub fn is_xmp(&self) -> bool {
        self.mtype == MType::Xmp
    }

    pub fn is_movie(&self) -> bool {
        self.mtype == MType::Movie
    }
}


#[cfg(test)]
#[test]
fn mime_type_works() {

    let mimetype = MimeType::new("/foo/bar/img_0001.cr2");
    assert_eq!(guess_type_for_file("/foo/bar/img_0001.cr2"), MType::Image(IsRaw::Yes));
    assert!(mimetype.is_image());
    assert!(mimetype.is_digicam_raw());
}
