/*
 * niepce - lib.rs
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

extern crate chrono;
extern crate exempi;
extern crate glib_sys;
extern crate gio_sys;
extern crate gio;
extern crate libc;
extern crate rusqlite;
#[macro_use]
extern crate try_opt;

#[macro_use]
pub mod fwk;
pub mod engine;
pub mod libraryclient;
pub mod capi;

include!(concat!(env!("OUT_DIR"), "/bindings.rs"));
