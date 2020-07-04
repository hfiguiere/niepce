/*
 * niepce - npc-fwk/toolkit/gdk_utils.rs
 *
 * Copyright (C) 2020 Hubert Figuière
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
use std::cmp;
use std::path::Path;
use std::slice;

use gdk_pixbuf;
use gdk_pixbuf::prelude::*;
use glib::translate::*;

/// Scale the pixbuf to fit in a square of %dim pixels
pub fn gdkpixbuf_scale_to_fit(
    pix: Option<&gdk_pixbuf::Pixbuf>,
    dim: i32,
) -> Option<gdk_pixbuf::Pixbuf> {
    if let Some(pix) = pix {
        let orig_h = pix.get_height();
        let orig_w = pix.get_width();
        let orig_dim = cmp::max(orig_h, orig_w);
        let ratio: f64 = dim as f64 / orig_dim as f64;
        let width = ratio * orig_w as f64;
        let height = ratio * orig_h as f64;
        pix.scale_simple(
            width as i32,
            height as i32,
            gdk_pixbuf::InterpType::Bilinear,
        )
    } else {
        None
    }
}

/// Rotate the pixbuf according to the Exif orientation value
pub fn gdkpixbuf_exif_rotate(
    pix: Option<&gdk_pixbuf::Pixbuf>,
    orientation: i32,
) -> Option<gdk_pixbuf::Pixbuf> {
    match orientation {
        0 | 1 => pix.cloned(),
        2 => pix.and_then(|p| p.flip(true)),
        3 => pix.and_then(|p| p.rotate_simple(gdk_pixbuf::PixbufRotation::Upsidedown)),
        4 => pix
            .and_then(|p| p.rotate_simple(gdk_pixbuf::PixbufRotation::Upsidedown))
            .and_then(|p| p.flip(true)),
        5 => pix
            .and_then(|p| p.rotate_simple(gdk_pixbuf::PixbufRotation::Clockwise))
            .and_then(|p| p.flip(false)),
        6 => pix.and_then(|p| p.rotate_simple(gdk_pixbuf::PixbufRotation::Clockwise)),
        7 => pix
            .and_then(|p| p.rotate_simple(gdk_pixbuf::PixbufRotation::Counterclockwise))
            .and_then(|p| p.flip(false)),
        8 => pix.and_then(|p| p.rotate_simple(gdk_pixbuf::PixbufRotation::Counterclockwise)),
        _ => None,
    }
}

// XXX bindgen
#[repr(i32)]
#[allow(dead_code)]
enum ORDataType {
    None = 0,
    Pixmap8RGB,
    Pixmap16RGB,
    JPEG,
    TIFF,
    PNG,
    RAW,
    CompressedRAW,
    Unknown,
}

#[repr(C)]
struct ORThumbnail {
    a: i32,
}
#[repr(C)]
struct ORRawFile {
    a: i32,
}

// XXX bindgen these too
extern "C" {
    fn or_rawfile_new(path: *const c_char, type_: i32) -> *mut ORRawFile;
    fn or_rawfile_get_orientation(rawfile: *const ORRawFile) -> i32;
    fn or_rawfile_get_thumbnail(
        rawfile: *const ORRawFile,
        size: u32,
        thumbnail: *mut ORThumbnail,
    ) -> i32;
    fn or_rawfile_release(rawfile: *mut ORRawFile) -> i32;
    fn or_thumbnail_new() -> *mut ORThumbnail;
    fn or_thumbnail_release(thumbnail: *mut ORThumbnail) -> i32;
    fn or_thumbnail_format(thumbnail: *const ORThumbnail) -> ORDataType;
    fn or_thumbnail_data(thumbnail: *const ORThumbnail) -> *const u8;
    fn or_thumbnail_data_size(thumbnail: *const ORThumbnail) -> usize;
    fn or_thumbnail_dimensions(thumbnail: *const ORThumbnail, x: *mut u32, y: *mut u32);
}

fn thumbnail_to_pixbuf(
    thumbnail: *const ORThumbnail,
    orientation: i32,
) -> Option<gdk_pixbuf::Pixbuf> {
    let format = unsafe { or_thumbnail_format(thumbnail) };
    let buf_size = unsafe { or_thumbnail_data_size(thumbnail) };
    let buf = unsafe { slice::from_raw_parts(or_thumbnail_data(thumbnail), buf_size) };

    let pixbuf = match format {
        ORDataType::Pixmap8RGB => {
            let mut x: u32 = 0;
            let mut y: u32 = 0;
            unsafe { or_thumbnail_dimensions(thumbnail, &mut x, &mut y) };

            let bytes = glib::Bytes::from(buf);
            Some(gdk_pixbuf::Pixbuf::from_bytes(
                &bytes,
                gdk_pixbuf::Colorspace::Rgb,
                false,
                8,
                x as i32,
                y as i32,
                x as i32 * 3,
            ))
        }
        ORDataType::JPEG | ORDataType::TIFF | ORDataType::PNG => {
            let loader = gdk_pixbuf::PixbufLoader::new();

            if let Err(err) = loader.write(buf) {
                err_out!("loader write error: {}", err);
            }

            if let Err(err) = loader.close() {
                err_out!("loader close error: {}", err);
            }
            loader.get_pixbuf()
        }
        _ => None,
    };

    gdkpixbuf_exif_rotate(pixbuf.as_ref(), orientation)
}

/// Extract the thumbnail from libopenraw
pub fn openraw_extract_rotated_thumbnail<P: AsRef<Path>>(
    path: P,
    dim: u32,
) -> Option<gdk_pixbuf::Pixbuf> {
    let mut pixbuf = None;
    let rf = unsafe { or_rawfile_new(path.as_ref().to_glib_none().0, 0) };
    if !rf.is_null() {
        let orientation = unsafe { or_rawfile_get_orientation(rf) };

        let thumbnail = unsafe { or_thumbnail_new() };
        let err = unsafe { or_rawfile_get_thumbnail(rf, dim, thumbnail) };
        if err == 0 {
            pixbuf = thumbnail_to_pixbuf(thumbnail, orientation);
        } else {
            err_out!("or_get_extract_thumbnail() failed with {}.", err);
        }
        let err = unsafe { or_thumbnail_release(thumbnail) };
        if err != 0 {
            err_out!("or_thumbnail_release() failed with {}", err);
        }
        unsafe { or_rawfile_release(rf) };
    }

    pixbuf
}
