/*
 * niepce - niepce/ui/image_list_store.rs
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

use std::ptr;

use gdk_pixbuf;
use gdk_pixbuf_sys;
use glib::translate::*;
use gtk;
use gtk::prelude::*;
use gtk::subclass::prelude::*;
use gtk_sys;

use npc_engine::db::libfile::{FileStatus, LibFile};
use npc_engine::db::LibraryId;
use npc_engine::library::notification::MetadataChange;
use npc_fwk::PropertyValue;

/// Wrap a libfile into something that can be in a glib::Value
#[derive(Clone)]
pub struct StoreLibFile(pub LibFile);

impl glib::subclass::boxed::BoxedType for StoreLibFile {
    const NAME: &'static str = "StoreLibFile";

    glib_boxed_type!();
}
glib_boxed_derive_traits!(StoreLibFile);

#[repr(i32)]
pub enum ColIndex {
    Thumb = 0,
    File = 1,
    StripThumb = 2,
    FileStatus = 3,
}

pub struct ImageListStore {
    store: gtk::ListStore,
}

impl ImageListStore {
    pub fn new() -> Self {
        let col_types: [glib::Type; 4] = [
            gdk_pixbuf::Pixbuf::static_type(),
            StoreLibFile::get_type(),
            gdk_pixbuf::Pixbuf::static_type(),
            glib::Type::I32,
        ];

        let store = gtk::ListStore::new(&col_types);

        Self { store }
    }

    pub fn get_file_id_at_path(&self, path: &gtk::TreePath) -> LibraryId {
        if let Some(iter) = self.store.get_iter(&path) {
            if let Ok(libfile) = self
                .store
                .get_value(&iter, ColIndex::File as i32)
                .get_some::<&StoreLibFile>()
            {
                return libfile.0.id();
            }
        }
        0
    }

    pub fn get_file(&self, iter: &gtk::TreeIter) -> Option<LibFile> {
        if let Ok(libfile) = self
            .store
            .get_value(&iter, ColIndex::File as i32)
            .get_some::<&StoreLibFile>()
        {
            return Some(libfile.0.clone());
        }
        None
    }

    pub fn add_row(
        &mut self,
        thumb: &gdk_pixbuf::Pixbuf,
        file: &LibFile,
        strip_thumb: &gdk_pixbuf::Pixbuf,
        status: FileStatus,
    ) -> gtk::TreeIter {
        let iter = self.store.append();
        let store_libfile = StoreLibFile(file.clone());
        let indices: [u32; 4] = [
            ColIndex::Thumb as u32,
            ColIndex::File as u32,
            ColIndex::StripThumb as u32,
            ColIndex::FileStatus as u32,
        ];
        assert!(thumb.ref_count() > 0);
        assert!(strip_thumb.ref_count() > 0);
        self.store.set(
            &iter,
            &indices,
            &[thumb, &store_libfile, strip_thumb, &(status as i32)],
        );
        iter
    }

    pub fn set_thumbnail(
        &mut self,
        iter: &gtk::TreeIter,
        thumb: &gdk_pixbuf::Pixbuf,
        strip_thumb: &gdk_pixbuf::Pixbuf,
    ) {
        let indices: [u32; 2] = [ColIndex::Thumb as u32, ColIndex::StripThumb as u32];
        assert!(thumb.ref_count() > 0);
        assert!(strip_thumb.ref_count() > 0);
        self.store.set(iter, &indices, &[thumb, strip_thumb]);
    }

    pub fn set_property(&self, iter: &gtk::TreeIter, change: &MetadataChange) {
        if let Ok(libfile) = self
            .store
            .get_value(&iter, ColIndex::File as i32)
            .get_some::<&StoreLibFile>()
        {
            assert!(libfile.0.id() == change.id);
            let meta = change.meta;
            if let PropertyValue::Int(value) = change.value {
                let mut file = libfile.0.clone();
                file.set_property(meta, value);
                self.store
                    .set_value(&iter, ColIndex::File as u32, &StoreLibFile(file).to_value());
            } else {
                err_out!("Wrong property type");
            }
        }
    }
}

#[no_mangle]
pub unsafe extern "C" fn npc_image_list_store_new() -> *mut ImageListStore {
    let box_ = Box::new(ImageListStore::new());
    Box::into_raw(box_)
}

#[no_mangle]
pub unsafe extern "C" fn npc_image_list_store_delete(self_: *mut ImageListStore) {
    assert!(!self_.is_null());
    Box::from_raw(self_);
}

/// Return the gobj for the GtkListStore. You must ref it to hold it.
#[no_mangle]
pub unsafe extern "C" fn npc_image_list_store_gobj(
    self_: &ImageListStore,
) -> *mut gtk_sys::GtkListStore {
    self_.store.to_glib_none().0
}

/// Return the ID of the file at the given GtkTreePath
#[no_mangle]
pub unsafe extern "C" fn npc_image_list_store_get_file_id_at_path(
    self_: &ImageListStore,
    path: *const gtk_sys::GtkTreePath,
) -> LibraryId {
    assert!(!path.is_null());
    self_.get_file_id_at_path(&from_glib_borrow(path))
}

#[no_mangle]
pub unsafe extern "C" fn npc_image_list_store_add_row(
    self_: &mut ImageListStore,
    thumb: *mut gdk_pixbuf_sys::GdkPixbuf,
    file: *const LibFile,
    strip_thumb: *mut gdk_pixbuf_sys::GdkPixbuf,
    status: FileStatus,
) -> gtk_sys::GtkTreeIter {
    assert!(!thumb.is_null());
    assert!(!strip_thumb.is_null());
    *self_
        .add_row(
            &from_glib_borrow(thumb),
            &*file,
            &from_glib_borrow(strip_thumb),
            status,
        )
        .to_glib_none()
        .0
}

#[no_mangle]
pub unsafe extern "C" fn npc_image_list_store_set_tnail(
    self_: &mut ImageListStore,
    iter: *mut gtk_sys::GtkTreeIter,
    thumb: *mut gdk_pixbuf_sys::GdkPixbuf,
    strip_thumb: *mut gdk_pixbuf_sys::GdkPixbuf,
) {
    assert!(!iter.is_null());
    assert!(!thumb.is_null());
    assert!(!strip_thumb.is_null());
    self_.set_thumbnail(
        &from_glib_borrow(iter),
        &from_glib_borrow(thumb),
        &from_glib_borrow(strip_thumb),
    )
}

#[no_mangle]
pub unsafe extern "C" fn npc_image_list_store_get_file(
    self_: &mut ImageListStore,
    iter: *mut gtk_sys::GtkTreeIter,
) -> *mut LibFile {
    assert!(!iter.is_null());
    if let Some(libfile) = self_.get_file(&from_glib_borrow(iter)) {
        Box::into_raw(Box::new(libfile))
    } else {
        ptr::null_mut()
    }
}

#[no_mangle]
pub unsafe extern "C" fn npc_image_list_store_set_property(
    self_: &mut ImageListStore,
    iter: *const gtk_sys::GtkTreeIter,
    change: *const MetadataChange,
) {
    assert!(!iter.is_null());
    assert!(!change.is_null());
    self_.set_property(&from_glib_borrow(mut_override(iter)), &*change);
}
