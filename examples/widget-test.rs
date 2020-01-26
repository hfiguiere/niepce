/*
 * niepce - examples/widget-test.rs
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

extern crate gtk;
extern crate niepce_rust;

use gtk::prelude::*;
use niepce_rust::niepce::ui::thumb_nav::{ThumbNav, ThumbNavMode};

pub fn main() {
    if let Err(err) = gtk::init() {
        println!("main: gtk::init failed: {}", err);
        panic!();
    }

    let thumbview = gtk::IconView::new();
    let thn = ThumbNav::new(&thumbview, ThumbNavMode::OneRow, true);
    thn.set_size_request(-1, 134);

    let box_ = gtk::Box::new(gtk::Orientation::Vertical, 0);
    box_.pack_start(&thn, false, false, 0);

    let window = gtk::Window::new(gtk::WindowType::Toplevel);
    window.add(&box_);
    window.show_all();
    window.connect_delete_event(|_, _| {
        gtk::main_quit();
        gtk::Inhibit(false)
    });

    gtk::main();
}
