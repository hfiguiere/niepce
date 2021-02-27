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

extern crate gdk_pixbuf;
extern crate gio;
extern crate glib;
extern crate gtk;
extern crate niepce_rust;
extern crate npc_fwk;

use gio::{resources_register, Resource};
use glib::{Bytes, Error};
use gtk::prelude::*;

use niepce_rust::niepce::ui::image_grid_view::ImageGridView;
use niepce_rust::niepce::ui::thumb_nav::{ThumbNav, ThumbNavMode};
use niepce_rust::niepce::ui::thumb_strip_view::ThumbStripView;
use npc_fwk::toolkit::widgets::rating_label::RatingLabel;

fn init() -> Result<(), Error> {
    /*
        // load the gresource binary at build time and include/link it into the final
        // binary.
        let res_bytes = include_bytes!("gresource.gresource");

        // Create Resource it will live as long the value lives.
        let gbytes = Bytes::from_static(res_bytes.as_ref());
        let resource = Resource::from_data(&gbytes)?;

        // Register the resource so it won't be dropped and will continue to live in
        // memory.
        resources_register(&resource);
    */
    Ok(())
}

pub fn main() {
    if let Err(err) = gtk::init() {
        println!("main: gtk::init failed: {}", err);
        panic!();
    }

    if let Err(err) = init() {
        println!("main: init failed: {}", err);
        panic!();
    }

    let model = gtk::ListStore::new(&[gdk_pixbuf::Pixbuf::static_type()]);
    let thumbview = ThumbStripView::new(model.upcast_ref::<gtk::TreeModel>());
    let thn = ThumbNav::new(
        &thumbview.upcast::<gtk::IconView>(),
        ThumbNavMode::OneRow,
        true,
    );
    thn.set_size_request(-1, 134);

    let box_ = gtk::Box::new(gtk::Orientation::Vertical, 0);
    let rating = RatingLabel::new(3, true);

    let image_grid = ImageGridView::new(model.upcast_ref::<gtk::TreeModel>());
    box_.pack_start(&rating, false, false, 0);
    box_.pack_start(&image_grid, true, true, 0);
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
