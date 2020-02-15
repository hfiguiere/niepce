/*
 * niepce - crates/npc-fwk/src/toolkit/widgets/rating_label.rs
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

use libc::c_int;
use std::cell::Cell;

use cairo;
use gdk::prelude::*;
use gdk_pixbuf::Pixbuf;
use glib::subclass;
use glib::subclass::prelude::*;
use glib::translate::*;
use glib::Type;
use gtk;
use gtk::prelude::*;
use gtk::subclass::prelude::*;
use gtk_sys;

use once_cell::unsync::Lazy;

struct Pixbufs {
    star: Pixbuf,
    unstar: Pixbuf,
}

const PIXBUFS: Lazy<Pixbufs> = Lazy::new(|| Pixbufs {
    star: Pixbuf::new_from_resource("/org/gnome/Niepce/pixmaps/niepce-set-star.png").unwrap(),
    unstar: Pixbuf::new_from_resource("/org/gnome/Niepce/pixmaps/niepce-unset-star.png").unwrap(),
});

glib_wrapper! {
    pub struct RatingLabel(
        Object<subclass::simple::InstanceStruct<RatingLabelPriv>,
        subclass::simple::ClassStruct<RatingLabelPriv>, RatingLabelClass>)
        @extends gtk::DrawingArea, gtk::Widget;

    match fn {
        get_type => || RatingLabelPriv::get_type().to_glib(),
    }
}

pub struct RatingLabelPriv {
    editable: Cell<bool>,
    rating: Cell<i32>,
}

impl RatingLabelPriv {
    fn set_editable(&self, editable: bool) {
        self.editable.set(editable);
    }

    fn set_rating(&self, rating: i32) {
        self.rating.set(rating);
        let w = self.get_instance();
        w.queue_draw();
    }
}

static PROPERTIES: [subclass::Property; 1] = [subclass::Property("rating", |rating| {
    glib::ParamSpec::int(
        rating,
        "Rating",
        "The rating value",
        0,
        5,
        0,
        glib::ParamFlags::READWRITE,
    )
})];

impl ObjectSubclass for RatingLabelPriv {
    const NAME: &'static str = "RatingLabel";
    type ParentType = gtk::DrawingArea;
    type Instance = subclass::simple::InstanceStruct<Self>;
    type Class = subclass::simple::ClassStruct<Self>;

    glib_object_subclass!();

    fn class_init(klass: &mut Self::Class) {
        klass.install_properties(&PROPERTIES);
        klass.add_signal(
            "rating-changed",
            glib::SignalFlags::RUN_LAST,
            &[Type::I32],
            Type::Unit,
        );
    }

    fn new() -> Self {
        Self {
            editable: Cell::new(true),
            rating: Cell::new(0),
        }
    }
}

impl ObjectImpl for RatingLabelPriv {
    glib_object_impl!();

    fn constructed(&self, obj: &glib::Object) {
        self.parent_constructed(obj);

        let widget = self.get_instance();
        widget.connect_realize(|w| {
            let priv_ = RatingLabelPriv::from_instance(w);
            if priv_.editable.get() {
                if let Some(win) = w.get_window() {
                    let mut mask = win.get_events();
                    mask |= gdk::EventMask::BUTTON_PRESS_MASK;
                    win.set_events(mask);
                }
            }
        });
    }

    fn set_property(&self, _obj: &glib::Object, id: usize, value: &glib::Value) {
        let prop = &PROPERTIES[id];

        match *prop {
            subclass::Property("rating", ..) => {
                let rating = value
                    .get_some()
                    .expect("type conformity checked by `Object::set_property`");
                self.set_rating(rating);
            }
            _ => unimplemented!(),
        }
    }

    fn get_property(&self, _obj: &glib::Object, id: usize) -> Result<glib::Value, ()> {
        let prop = &PROPERTIES[id];

        match *prop {
            subclass::Property("rating", ..) => Ok(self.rating.get().to_value()),
            _ => unimplemented!(),
        }
    }
}

pub trait RatingLabelExt {
    fn set_rating(&self, rating: i32);
}

impl RatingLabelExt for RatingLabel {
    fn set_rating(&self, rating: i32) {
        let priv_ = RatingLabelPriv::from_instance(self);
        priv_.set_rating(rating);
    }
}

impl RatingLabel {
    pub fn get_star() -> Pixbuf {
        PIXBUFS.star.clone()
    }

    pub fn get_unstar() -> Pixbuf {
        PIXBUFS.unstar.clone()
    }

    pub fn get_geometry() -> (i32, i32) {
        let star = Self::get_star();
        (star.get_width() * 5, star.get_height())
    }

    pub fn draw_rating(
        cr: &cairo::Context,
        rating: i32,
        star: &Pixbuf,
        unstar: &Pixbuf,
        x: f64,
        y: f64,
    ) {
        let rating = if rating == -1 { 0 } else { rating };

        let w = star.get_width();
        let h = star.get_height();
        let mut y = y;
        y -= h as f64;
        let mut x = x;
        for i in 1..=5 {
            if i <= rating {
                cr.set_source_pixbuf(star, x, y);
            } else {
                cr.set_source_pixbuf(unstar, x, y);
            }
            cr.paint();
            x += w as f64;
        }
    }

    pub fn rating_value_from_hit_x(x: f64) -> i32 {
        let width: f64 = Self::get_star().get_width().into();
        (x / width).round() as i32
    }

    pub fn new(rating: i32, editable: bool) -> Self {
        let obj: Self = glib::Object::new(Self::static_type(), &[])
            .expect("Failed to create RatingLabel")
            .downcast()
            .expect("Created RatingLabel is of the wrong type");

        let priv_ = RatingLabelPriv::from_instance(&obj);
        priv_.set_editable(editable);
        priv_.set_rating(rating);
        obj
    }
}

impl DrawingAreaImpl for RatingLabelPriv {}
impl WidgetImpl for RatingLabelPriv {
    fn button_press_event(&self, _widget: &gtk::Widget, event: &gdk::EventButton) -> Inhibit {
        if event.get_button() != 1 {
            Inhibit(false)
        } else {
            if let Some((x, _)) = event.get_coords() {
                let new_rating = RatingLabel::rating_value_from_hit_x(x);
                if new_rating != self.rating.get() {
                    self.set_rating(new_rating);
                    if let Err(err) = self.get_instance().emit("rating-changed", &[&new_rating]) {
                        err_out!("Emit signal 'rating-changed' failed: {}", err);
                    }
                }
            }
            Inhibit(true)
        }
    }

    fn draw(&self, _widget: &gtk::Widget, cr: &cairo::Context) -> Inhibit {
        let star = RatingLabel::get_star();
        let x = 0_f64;
        let y = star.get_height() as f64;
        RatingLabel::draw_rating(
            cr,
            self.rating.get(),
            &star,
            &RatingLabel::get_unstar(),
            x,
            y,
        );

        Inhibit(true)
    }

    fn get_preferred_width(&self, _widget: &gtk::Widget) -> (i32, i32) {
        let w = RatingLabel::get_star().get_width() * 5;
        (w, w)
    }

    fn get_preferred_height(&self, _widget: &gtk::Widget) -> (i32, i32) {
        let h = RatingLabel::get_star().get_height();
        (h, h)
    }
}

#[no_mangle]
pub extern "C" fn fwk_rating_label_new(rating: c_int, editable: bool) -> *mut gtk_sys::GtkWidget {
    RatingLabel::new(rating, editable)
        .upcast::<gtk::Widget>()
        .to_glib_full()
}

/// Set the rating for the %RatingLabel widget
///
/// # Safety
/// Dereference the widget pointer.
#[no_mangle]
pub unsafe extern "C" fn fwk_rating_label_set_rating(
    widget: *mut gtk_sys::GtkDrawingArea,
    rating: i32,
) {
    let rating_label = gtk::DrawingArea::from_glib_borrow(widget)
        .downcast::<RatingLabel>()
        .expect("Not a RatingLabel widget");
    rating_label.set_rating(rating);
}
