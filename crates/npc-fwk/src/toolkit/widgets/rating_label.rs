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

use cairo;
use gdk::prelude::*;
use gdk_pixbuf::Pixbuf;
use once_cell::unsync::Lazy;

struct Pixbufs {
    star: Pixbuf,
    unstar: Pixbuf,
}

const PIXBUFS: Lazy<Pixbufs> = Lazy::new(|| Pixbufs {
    star: Pixbuf::new_from_resource("/org/gnome/Niepce/pixmaps/niepce-set-star.png").unwrap(),
    unstar: Pixbuf::new_from_resource("/org/gnome/Niepce/pixmaps/niepce-unset-star.png").unwrap(),
});

pub struct RatingLabel {}

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
        for i in 1..5 {
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
}
