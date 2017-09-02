/*
 * niepce - engine/db/label.rs
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

use std::str::FromStr;
use rusqlite;

use super::LibraryId;
use super::FromDb;
use fwk::base::rgbcolour::RgbColour;

pub struct Label {
    id: LibraryId,
    label: String,
    colour: RgbColour,
}

impl Label {

    pub fn new(id: LibraryId, label: &str, colourstring: &str) -> Label {
        let colour = RgbColour::from_str(colourstring).unwrap_or(Default::default());
        Label {
            id: id,
            label: String::from(label),
            colour: colour
        }
    }

    pub fn id(&self) -> LibraryId {
        self.id
    }

    pub fn label(&self) -> &str {
        &self.label
    }

    pub fn set_label(&mut self, label: &str) {
        self.label = String::from(label)
    }

    pub fn colour(&self) -> &RgbColour {
        &self.colour
    }

    pub fn set_colour(&mut self, c: &RgbColour) {
        self.colour = c.clone();
    }
}

impl FromDb for Label {
    fn read_db_columns() -> &'static str {
        "id,name,color"
    }

    fn read_db_tables() -> &'static str {
        "labels"
    }

    fn read_from(row: &rusqlite::Row) -> Self {
        let label : String = row.get(1);
        let colour : String = row.get(2);
        Label::new(row.get(0), &label, &colour)
    }
}
