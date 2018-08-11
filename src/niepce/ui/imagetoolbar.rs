/*
 * niepce - ui/imagetoolbar.rs
 *
 * Copyright (C) 2018 Hubert Figuiere
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

#[no_mangle]
pub extern "C" fn image_toolbar_new() -> *mut gtk_sys::GtkToolbar {
    let toolbar = gtk::Toolbar::new();

    let icon = gtk::Image::new_from_icon_name(
        "go-previous-symbolic", gtk::IconSize::SmallToolbar.into());
    let tool_item = gtk::ToolButton::new(Some(&icon), None);
    tool_item.set_action_name("shell.PrevImage");
    toolbar.add(&tool_item);

    let icon = gtk::Image::new_from_icon_name(
        "go-next-symbolic", gtk::IconSize::SmallToolbar.into());
    let tool_item = gtk::ToolButton::new(Some(&icon), None);
    tool_item.set_action_name("shell.NextImage");
    toolbar.add(&tool_item);

    let separator = gtk::SeparatorToolItem::new();
    toolbar.add(&separator);

    let icon = gtk::Image::new_from_icon_name(
        "object-rotate-left-symbolic", gtk::IconSize::SmallToolbar.into());
    let tool_item = gtk::ToolButton::new(Some(&icon), None);
    tool_item.set_action_name("shell.RotateLeft");
    toolbar.add(&tool_item);

    let icon = gtk::Image::new_from_icon_name(
        "object-rotate-right-symbolic", gtk::IconSize::SmallToolbar.into());
    let tool_item = gtk::ToolButton::new(Some(&icon), None);
    tool_item.set_action_name("shell.RotateRight");
    toolbar.add(&tool_item);


    toolbar.to_glib_full()
}
