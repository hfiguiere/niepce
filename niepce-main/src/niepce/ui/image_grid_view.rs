/*
 * niepce - niepce/ui/image_grid_view.rs
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

use glib::subclass;
use glib::subclass::prelude::*;
use glib::translate::*;
use gtk;
use gtk::prelude::*;
use gtk::subclass::prelude::*;
use gtk::subclass::widget::WidgetImplExt;

use super::library_cell_renderer::LibraryCellRenderer;
use npc_fwk::toolkit::clickable_cell_renderer::ClickableCellRenderer;

glib_wrapper! {
    pub struct ImageGridView(
        Object<subclass::simple::InstanceStruct<ImageGridViewPriv>,
        subclass::simple::ClassStruct<ImageGridViewPriv>,
        ImageGridViewClass>)
        @extends gtk::IconView, gtk::Container, gtk::Widget;

    match fn {
        get_type => || ImageGridViewPriv::get_type().to_glib(),
    }
}

impl ImageGridView {
    pub fn new(store: &gtk::TreeModel) -> Self {
        glib::Object::new(Self::static_type(), &[("model", store)])
            .expect("Failed to create ImageGridView")
            .downcast()
            .expect("Created ImageGridView is of the wrong type")
    }
}

pub struct ImageGridViewPriv {}

static PROPERTIES: [subclass::Property; 0] = [];

impl ObjectSubclass for ImageGridViewPriv {
    const NAME: &'static str = "ImageGridView";
    type ParentType = gtk::IconView;
    type Instance = subclass::simple::InstanceStruct<Self>;
    type Class = subclass::simple::ClassStruct<Self>;

    glib_object_subclass!();

    fn class_init(klass: &mut Self::Class) {
        klass.install_properties(&PROPERTIES);
    }

    fn new() -> Self {
        Self {}
    }
}

impl ObjectImpl for ImageGridViewPriv {
    glib_object_impl!();

    fn constructed(&self, obj: &glib::Object) {
        self.parent_constructed(obj);
    }
}

impl WidgetImpl for ImageGridViewPriv {
    fn button_press_event(&self, widget: &gtk::Widget, event: &gdk::EventButton) -> gtk::Inhibit {
        let r = self.parent_button_press_event(widget, event);

        if let Some((x, y)) = event.get_coords() {
            if let Some(iconview) = widget.downcast_ref::<gtk::IconView>() {
                if let Some((_, cell)) = iconview.get_item_at_pos(x as i32, y as i32) {
                    if let Ok(mut cell) = cell.downcast::<LibraryCellRenderer>() {
                        cell.hit(x as i32, y as i32);
                    }
                }
            }
        }

        r
    }
}

impl ContainerImpl for ImageGridViewPriv {}

impl IconViewImpl for ImageGridViewPriv {}

#[no_mangle]
pub unsafe extern "C" fn npc_image_grid_view_new(
    store: *mut gtk_sys::GtkTreeModel,
) -> *mut gtk_sys::GtkWidget {
    ImageGridView::new(&gtk::TreeModel::from_glib_full(store))
        .upcast::<gtk::Widget>()
        .to_glib_full()
}
