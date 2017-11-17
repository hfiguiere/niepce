/*
 * niepce - niepce/ui/dialogs/requestnewfolder.rs
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

use glib::translate::*;
use gtk::prelude::*;
use gtk;
use gtk_sys;
use gtk::{
    Dialog,
    Entry,
    Label,
};

use libraryclient::{ClientInterfaceSync,LibraryClientWrapper};

#[no_mangle]
pub extern "C" fn dialog_request_new_folder(client: &mut LibraryClientWrapper,
                                            parent: *mut gtk_sys::GtkWindow) {
    let parent = unsafe { gtk::Window::from_glib_none(parent) };
    let dialog = Dialog::new_with_buttons(Some("New folder"), Some(&parent),
                                          gtk::DIALOG_MODAL,
                                          &[("OK", 0), ("Cancel", 1)]);
    let label = Label::new("Folder name:");
    dialog.get_content_area().pack_start(&label, true, false, 4);
    let entry = Entry::new();
    entry.set_text("foobar");
    dialog.get_content_area().pack_end(&entry, true, false, 4);

    dialog.get_content_area().show_all();
    let cancel = match dialog.run() {
        0 => false,
        _ => true,
    };
    let folder_name = entry.get_text();
    dialog.destroy();
    if !cancel {
        /*let id =*/ client.unwrap_mut().create_folder_sync(folder_name.unwrap(), None);
    }
}

