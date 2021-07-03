/*
 * niepce - niepce/ui/image_grid_view.rs
 *
 * Copyright (C) 2020-2021 Hubert Figuière
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

use glib::subclass::prelude::*;
use glib::translate::*;
use gtk::prelude::*;
use gtk::subclass::prelude::*;
use gtk::subclass::widget::WidgetImplExt;

use super::library_cell_renderer::LibraryCellRenderer;
use npc_fwk::toolkit::clickable_cell_renderer::ClickableCellRenderer;

glib::wrapper! {
    pub struct ImageGridView(
        ObjectSubclass<ImageGridViewPriv>)
        @extends gtk::IconView, gtk::Container, gtk::Widget;
}

impl ImageGridView {
    pub fn new(store: &gtk::TreeModel) -> Self {
        glib::Object::new(&[("model", store)]).expect("Failed to create ImageGridView")
    }
}

pub struct ImageGridViewPriv {}

#[glib::object_subclass]
impl ObjectSubclass for ImageGridViewPriv {
    const NAME: &'static str = "ImageGridView";
    type Type = ImageGridView;
    type ParentType = gtk::IconView;

    fn new() -> Self {
        Self {}
    }
}

impl ObjectImpl for ImageGridViewPriv {
    fn constructed(&self, obj: &ImageGridView) {
        self.parent_constructed(obj);
    }
}

impl WidgetImpl for ImageGridViewPriv {
    fn button_press_event(&self, widget: &ImageGridView, event: &gdk::EventButton) -> gtk::Inhibit {
        let r = self.parent_button_press_event(widget, event);

        if let Some((x, y)) = event.coords() {
            if let Some((_, cell)) = widget.item_at_pos(x as i32, y as i32) {
                if let Ok(mut cell) = cell.downcast::<LibraryCellRenderer>() {
                    cell.hit(x as i32, y as i32);
                }
            }
        }

        r
    }
}

impl ContainerImpl for ImageGridViewPriv {}

impl IconViewImpl for ImageGridViewPriv {}

/// # Safety
/// Use raw pointers.
#[no_mangle]
pub unsafe extern "C" fn npc_image_grid_view_new(
    store: *mut gtk_sys::GtkTreeModel,
) -> *mut gtk_sys::GtkWidget {
    ImageGridView::new(&gtk::TreeModel::from_glib_full(store))
        .upcast::<gtk::Widget>()
        .to_glib_full()
}
