/*
 * niepce - niepce/ui/dialogs/confirm.rs
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

use glib::translate::*;
use gtk;
use gtk::prelude::*;
use gtk::MessageDialog;
use gtk_sys;

#[no_mangle]
pub unsafe extern "C" fn dialog_confirm(
    message: *const c_char,
    parent: *mut gtk_sys::GtkWindow,
) -> bool {
    let mut result: bool = false;
    let msg = CStr::from_ptr(message).to_string_lossy();
    let parent = gtk::Window::from_glib_none(parent);
    let dialog = MessageDialog::new(
        Some(&parent),
        gtk::DialogFlags::MODAL,
        gtk::MessageType::Question,
        gtk::ButtonsType::YesNo,
        &*msg,
    );

    if dialog.run() == gtk::ResponseType::Yes.into() {
        result = true;
    }
    dialog.destroy();

    result
}
