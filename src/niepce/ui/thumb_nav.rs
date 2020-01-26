/*
 * niepce - niepce/ui/thumb_nav.rs
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

use std::cell::{Cell, RefCell};
use std::rc::Rc;

use once_cell::unsync::OnceCell;

use glib::subclass;
use glib::subclass::prelude::*;
use glib::translate::*;
use gtk;
use gtk::prelude::*;
use gtk::subclass::prelude::*;

const SCROLL_INC: f64 = 1.;
const SCROLL_MOVE: f64 = 20.;
const SCROLL_TIMEOUT: u32 = 20;

#[repr(i32)]
#[derive(Clone, Copy, PartialEq)]
pub enum ThumbNavMode {
    OneRow,
    OneColumn,
    MultipleRows,
    MultipleColumns,
    Invalid,
}

impl Into<i32> for ThumbNavMode {
    fn into(self) -> i32 {
        match self {
            ThumbNavMode::OneRow => 0,
            ThumbNavMode::OneColumn => 1,
            ThumbNavMode::MultipleRows => 2,
            ThumbNavMode::MultipleColumns => 3,
            ThumbNavMode::Invalid => 4,
        }
    }
}

impl From<i32> for ThumbNavMode {
    fn from(value: i32) -> Self {
        match value {
            0 => ThumbNavMode::OneRow,
            1 => ThumbNavMode::OneColumn,
            2 => ThumbNavMode::MultipleRows,
            3 => ThumbNavMode::MultipleColumns,
            _ => ThumbNavMode::Invalid,
        }
    }
}

glib_wrapper! {
    pub struct ThumbNav(
        Object<subclass::simple::InstanceStruct<ThumbNavPriv>,
        subclass::simple::ClassStruct<ThumbNavPriv>,
        ThumbNavClass>)
        @extends gtk::Box, gtk::Container, gtk::Widget;

    match fn {
        get_type => || ThumbNavPriv::get_type().to_glib(),
    }
}

impl ThumbNav {
    pub fn new(thumbview: &gtk::IconView, mode: ThumbNavMode, show_buttons: bool) -> Self {
        let mode_n: i32 = mode.into();
        glib::Object::new(
            Self::static_type(),
            &[
                ("mode", &mode_n),
                ("show-buttons", &show_buttons),
                ("thumbview", thumbview),
                ("homogeneous", &false),
                ("spacing", &0),
            ],
        )
        .expect("Failed to create Thumbnail Navigator")
        .downcast()
        .expect("Created Thumbnail Navigator is of wrong type")
    }
}

struct ThumbNavWidgets {
    button_left: gtk::Button,
    button_right: gtk::Button,
    sw: gtk::ScrolledWindow,
}

pub struct ThumbNavPriv {
    mode: Cell<ThumbNavMode>,
    show_buttons: Cell<bool>,

    left_i: Cell<f64>,
    right_i: Cell<f64>,
    widgets: OnceCell<ThumbNavWidgets>,
    thumbview: RefCell<Option<gtk::IconView>>,
}

pub trait ThumbNavExt {
    /// Get whether we show the left and right scroll buttons.
    fn get_show_buttons(&self) -> bool;
    /// Set whether we show the left and right scroll buttons.
    fn set_show_buttons(&self, show_buttons: bool);
    /// Get the navigation mode.
    fn get_mode(&self) -> ThumbNavMode;
    /// Set the navigation mode.
    fn set_mode(&self, mode: ThumbNavMode);
}

impl ThumbNavExt for ThumbNav {
    fn get_show_buttons(&self) -> bool {
        let priv_ = ThumbNavPriv::from_instance(self);
        priv_.show_buttons.get()
    }

    fn set_show_buttons(&self, show_buttons: bool) {
        let priv_ = ThumbNavPriv::from_instance(self);
        priv_.set_show_buttons(show_buttons);
    }

    fn get_mode(&self) -> ThumbNavMode {
        let priv_ = ThumbNavPriv::from_instance(self);
        priv_.mode.get()
    }

    fn set_mode(&self, mode: ThumbNavMode) {
        let priv_ = ThumbNavPriv::from_instance(self);
        priv_.set_mode(mode);
    }
}

impl ThumbNavPriv {
    fn left_button_clicked(&self) {
        if let Some(adj) = self.widgets.get().unwrap().sw.get_hadjustment() {
            let adj = Rc::new(adj);
            let i = self.left_i.clone();
            gtk::timeout_add(SCROLL_TIMEOUT, move || ThumbNavPriv::scroll_left(&i, &adj));
        }
    }

    fn right_button_clicked(&self) {
        if let Some(adj) = self.widgets.get().unwrap().sw.get_hadjustment() {
            let adj = Rc::new(adj);
            let i = self.right_i.clone();
            gtk::timeout_add(SCROLL_TIMEOUT, move || ThumbNavPriv::scroll_right(&i, &adj));
        }
    }

    fn adj_changed(&self, adj: &gtk::Adjustment) {
        if let Some(widgets) = self.widgets.get() {
            let upper = adj.get_upper();
            let page_size = adj.get_page_size();
            widgets.button_right.set_sensitive(upper > page_size)
        }
    }

    fn adj_value_changed(&self, adj: &gtk::Adjustment) {
        let upper = adj.get_upper();
        let page_size = adj.get_page_size();
        let value = adj.get_value();

        if let Some(w) = self.widgets.get() {
            w.button_left.set_sensitive(value > 0.0);
            w.button_right.set_sensitive(value < upper - page_size);
        }
    }

    fn scroll_left(ref_i: &Cell<f64>, adj: &gtk::Adjustment) -> glib::Continue {
        let value = adj.get_value();
        let i = ref_i.get();

        if i == SCROLL_MOVE || value - SCROLL_INC < 0.0 {
            ref_i.set(0.0);
            if let Err(err) = adj.emit("value-changed", &[]) {
                err_out!("signal emit value-changed {}", err);
            }
            return Continue(false);
        }

        ref_i.set(i + 1.0);

        let move_ = f64::min(SCROLL_MOVE, value);
        adj.set_value(value - move_);

        Continue(true)
    }

    fn scroll_right(ref_i: &Cell<f64>, adj: &gtk::Adjustment) -> glib::Continue {
        let upper = adj.get_upper();
        let page_size = adj.get_page_size();
        let value = adj.get_value();
        let i = ref_i.get();

        if i == SCROLL_MOVE || value + SCROLL_INC > upper - page_size {
            ref_i.set(0.0);
            return Continue(false);
        }

        ref_i.set(i + 1.0);

        let move_ = f64::min(SCROLL_MOVE, upper - page_size - value);
        adj.set_value(value + move_);

        Continue(true)
    }

    fn set_show_buttons(&self, show_buttons: bool) {
        self.show_buttons.set(show_buttons);

        let widgets = &self.widgets.get().unwrap();
        if show_buttons && self.mode.get() == ThumbNavMode::OneRow {
            widgets.button_left.show_all();
            widgets.button_right.show_all();
        } else {
            widgets.button_left.hide();
            widgets.button_right.hide();
        }
    }

    fn set_mode(&self, mode: ThumbNavMode) {
        self.mode.set(mode);

        match mode {
            ThumbNavMode::OneRow => {
                if let Some(thumbview) = &*self.thumbview.borrow() {
                    thumbview.set_size_request(-1, -1);
                    if let Err(err) = thumbview.set_property("item-height", &100) {
                        err_out!(
                            "ThumbNav::set_mode: set property 'item-height' failed: {}",
                            err
                        );
                    }
                }
                self.widgets
                    .get()
                    .unwrap()
                    .sw
                    .set_policy(gtk::PolicyType::Always, gtk::PolicyType::Never);

                self.set_show_buttons(self.show_buttons.get());
            }
            ThumbNavMode::OneColumn
            | ThumbNavMode::MultipleRows
            | ThumbNavMode::MultipleColumns => {
                if let Some(thumbview) = &*self.thumbview.borrow() {
                    thumbview.set_columns(1);

                    thumbview.set_size_request(-1, -1);
                    if let Err(err) = thumbview.set_property("item-height", &-1) {
                        err_out!(
                            "ThumbNav::set_mode: set property 'item-height' failed: {}",
                            err
                        );
                    }
                }
                if let Some(widgets) = self.widgets.get() {
                    widgets
                        .sw
                        .set_policy(gtk::PolicyType::Never, gtk::PolicyType::Always);

                    widgets.button_left.hide();
                    widgets.button_right.hide();
                }
            }
            _ => {}
        }
    }

    fn add_thumbview(&self) {
        if let Some(ref thumbview) = &*self.thumbview.borrow() {
            if let Some(widgets) = self.widgets.get() {
                widgets.sw.add(thumbview);
                widgets.sw.show_all();
            }
        } else {
            err_out!("No thumbview to add");
        }
    }
}

static PROPERTIES: [subclass::Property; 3] = [
    subclass::Property("show-buttons", |show_buttons| {
        glib::ParamSpec::boolean(
            show_buttons,
            "Show Buttons",
            "Whether to show navigation buttons or not",
            true, // Default value
            glib::ParamFlags::READWRITE,
        )
    }),
    subclass::Property("thumbview", |thumbview| {
        glib::ParamSpec::object(
            thumbview,
            "Thumbnail View",
            "The internal thumbnail viewer widget",
            gtk::IconView::static_type(),
            glib::ParamFlags::READWRITE | glib::ParamFlags::CONSTRUCT_ONLY,
        )
    }),
    subclass::Property("mode", |mode| {
        glib::ParamSpec::int(
            mode,
            "Mode",
            "Thumb navigator mode",
            ThumbNavMode::OneRow.into(),
            ThumbNavMode::MultipleRows.into(),
            ThumbNavMode::OneRow.into(),
            glib::ParamFlags::READWRITE,
        )
    }),
];

impl ObjectSubclass for ThumbNavPriv {
    const NAME: &'static str = "NpcThumbNav";
    type ParentType = gtk::Box;
    type Instance = subclass::simple::InstanceStruct<Self>;
    type Class = subclass::simple::ClassStruct<Self>;

    glib_object_subclass!();

    fn class_init(klass: &mut Self::Class) {
        klass.install_properties(&PROPERTIES);
    }

    fn new() -> Self {
        Self {
            mode: Cell::new(ThumbNavMode::OneRow),
            show_buttons: Cell::new(true),
            left_i: Cell::new(0.0),
            right_i: Cell::new(0.0),
            widgets: OnceCell::new(),
            thumbview: RefCell::new(None),
        }
    }
}

impl ObjectImpl for ThumbNavPriv {
    glib_object_impl!();

    fn constructed(&self, obj: &glib::Object) {
        self.parent_constructed(obj);

        let self_ = obj.downcast_ref::<ThumbNav>().unwrap();

        let button_left = gtk::Button::new();
        button_left.set_relief(gtk::ReliefStyle::None);
        let arrow =
            gtk::Image::new_from_icon_name(Some(&"pan-start-symbolic"), gtk::IconSize::Button);
        button_left.add(&arrow);
        button_left.set_size_request(20, 0);
        self_.pack_start(&button_left, false, false, 0);
        button_left.connect_clicked(clone!(@weak self_ => move |_| {
            let priv_ = ThumbNavPriv::from_instance(&self_);
            priv_.left_button_clicked();
        }));

        let sw = gtk::ScrolledWindow::new(gtk::NONE_ADJUSTMENT, gtk::NONE_ADJUSTMENT);
        sw.set_shadow_type(gtk::ShadowType::In);
        sw.set_policy(gtk::PolicyType::Always, gtk::PolicyType::Never);
        if let Some(adj) = sw.get_hadjustment() {
            adj.connect_changed(clone!(@weak self_ => move |adj| {
                let priv_ = ThumbNavPriv::from_instance(&self_);
                priv_.adj_changed(adj);
            }));
            adj.connect_value_changed(clone!(@weak self_ => move |adj| {
                let priv_ = ThumbNavPriv::from_instance(&self_);
                priv_.adj_value_changed(adj);
            }));
        }
        self_.pack_start(&sw, true, true, 0);

        let button_right = gtk::Button::new();
        button_right.set_relief(gtk::ReliefStyle::None);
        let arrow =
            gtk::Image::new_from_icon_name(Some(&"pan-end-symbolic"), gtk::IconSize::Button);
        button_right.add(&arrow);
        button_right.set_size_request(20, 0);
        self_.pack_start(&button_right, false, false, 0);
        button_right.connect_clicked(clone!(@weak self_ => move |_| {
            let priv_ = ThumbNavPriv::from_instance(&self_);
            priv_.right_button_clicked();
        }));
        let adj = sw.get_hadjustment();

        if self
            .widgets
            .set(ThumbNavWidgets {
                button_left,
                button_right,
                sw,
            })
            .is_err()
        {
            err_out!("Widgets already set.");
        }

        if let Some(adj) = adj {
            // The value-changed signal might not be emitted because the value is already 0.
            // Ensure the state first.
            self.adj_value_changed(&adj);
            if let Err(err) = adj.emit("value-changed", &[]) {
                err_out!("ThumbNav::constructed: signal emit failed: {}", err);
            }
        }

        self.add_thumbview();
    }

    fn set_property(&self, _obj: &glib::Object, id: usize, value: &glib::Value) {
        let prop = &PROPERTIES[id];
        match *prop {
            subclass::Property("show-buttons", ..) => {
                let show_buttons = value
                    .get_some()
                    .expect("type conformity checked by `Object::set_property`");
                self.set_show_buttons(show_buttons);
            }
            subclass::Property("thumbview", ..) => {
                let thumbview: Option<gtk::IconView> = value
                    .get()
                    .expect("type conformity checked by `Object::set_property`");
                self.thumbview.replace(thumbview);
                self.add_thumbview();
            }
            subclass::Property("mode", ..) => {
                let mode: i32 = value
                    .get_some()
                    .expect("type conformity checked by `Object::set_property`");
                self.set_mode(ThumbNavMode::from(mode));
            }
            _ => unimplemented!(),
        }
    }

    fn get_property(&self, _obj: &glib::Object, id: usize) -> Result<glib::Value, ()> {
        let prop = &PROPERTIES[id];

        match *prop {
            subclass::Property("show-buttons", ..) => Ok(self.show_buttons.get().to_value()),
            subclass::Property("thumbview", ..) => Ok(self.thumbview.borrow().to_value()),
            subclass::Property("mode", ..) => {
                let n: i32 = self.mode.get().into();
                Ok(n.to_value())
            }
            _ => unimplemented!(),
        }
    }
}

impl WidgetImpl for ThumbNavPriv {}

impl ContainerImpl for ThumbNavPriv {}

impl BoxImpl for ThumbNavPriv {}

#[no_mangle]
pub unsafe extern "C" fn npc_thumb_nav_new(
    thumbview: *mut gtk_sys::GtkIconView,
    mode: ThumbNavMode,
    show_buttons: bool,
) -> *mut gtk_sys::GtkWidget {
    ThumbNav::new(
        &gtk::IconView::from_glib_full(thumbview),
        mode,
        show_buttons,
    )
    .upcast::<gtk::Widget>()
    .to_glib_full()
}
