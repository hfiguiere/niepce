/*
 * niepce - niepce/ui/library_cell_renderer.rs
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

use libc::c_void;
use once_cell::unsync::Lazy;
use std::cell::{Cell, RefCell};
use std::ptr;

use cairo;
use gdk;
use gdk::prelude::*;
use gdk_pixbuf::Pixbuf;
use glib::subclass;
use glib::subclass::prelude::*;
use glib::translate::*;
use glib::Type;
use gtk;
use gtk::prelude::*;
use gtk::subclass::prelude::*;
use gtk::CellRendererPixbufClass;

use crate::niepce::ui::image_list_store::StoreLibFile;
use npc_engine::db::libfile::{FileStatus, FileType};
use npc_fwk::base::rgbcolour::RgbColour;
use npc_fwk::toolkit::clickable_cell_renderer::ClickableCellRenderer;
use npc_fwk::toolkit::widgets::rating_label::RatingLabel;

const CELL_PADDING: i32 = 4;

struct Emblems {
    raw: Pixbuf,
    raw_jpeg: Pixbuf,
    img: Pixbuf,
    video: Pixbuf,
    unknown: Pixbuf,
    status_missing: Pixbuf,
    flag_reject: Pixbuf,
    flag_pick: Pixbuf,
}

const EMBLEMS: Lazy<Emblems> = Lazy::new(|| Emblems {
    raw: Pixbuf::new_from_resource("/org/gnome/Niepce/pixmaps/niepce-raw-fmt.png").unwrap(),
    raw_jpeg: Pixbuf::new_from_resource("/org/gnome/Niepce/pixmaps/niepce-rawjpeg-fmt.png")
        .unwrap(),
    img: Pixbuf::new_from_resource("/org/gnome/Niepce/pixmaps/niepce-img-fmt.png").unwrap(),
    video: Pixbuf::new_from_resource("/org/gnome/Niepce/pixmaps/niepce-video-fmt.png").unwrap(),
    unknown: Pixbuf::new_from_resource("/org/gnome/Niepce/pixmaps/niepce-unknown-fmt.png").unwrap(),
    status_missing: Pixbuf::new_from_resource("/org/gnome/Niepce/pixmaps/niepce-missing.png")
        .unwrap(),
    flag_reject: Pixbuf::new_from_resource("/org/gnome/Niepce/pixmaps/niepce-flag-reject.png")
        .unwrap(),
    flag_pick: Pixbuf::new_from_resource("/org/gnome/Niepce/pixmaps/niepce-flag-pick.png").unwrap(),
});

glib_wrapper! {
    pub struct LibraryCellRenderer(
        Object<subclass::simple::InstanceStruct<LibraryCellRendererPriv>,
        subclass::simple::ClassStruct<LibraryCellRendererPriv>,
        LibraryCellRendererClass>)
        @extends gtk::CellRendererPixbuf, gtk::CellRenderer;

    match fn {
        get_type => || LibraryCellRendererPriv::get_type().to_glib(),
    }
}

impl LibraryCellRenderer {
    /// Create a library cell renderer.
    /// callback: an optional callback used to get a colour for labels.
    /// callback_data: raw pointer passed as is to the callback.
    pub fn new(callback: Option<GetColourCallback>, callback_data: *const c_void) -> Self {
        let obj: Self = glib::Object::new(
            Self::static_type(),
            &[("mode", &gtk::CellRendererMode::Activatable)],
        )
        .expect("Failed to create Library Cell Renderer")
        .downcast()
        .expect("Created Library Cell Renderer is of wrong type");

        if callback.is_some() {
            let priv_ = LibraryCellRendererPriv::from_instance(&obj);
            priv_.get_colour_callback.replace(callback);
            priv_.callback_data.set(callback_data);
        }

        obj
    }

    /// Create a new thumb renderer, basicall a LibraryCellRender with some options.
    /// Mostly just draw the thumbnail.
    /// Doesn't need the get_colour_callback
    pub fn new_thumb_renderer() -> Self {
        let cell_renderer = Self::new(None, ptr::null());

        cell_renderer.set_pad(0);
        cell_renderer.set_size(100);
        cell_renderer.set_drawborder(false);
        cell_renderer.set_drawemblem(false);
        cell_renderer.set_drawrating(false);
        cell_renderer.set_drawlabel(false);
        cell_renderer.set_drawflag(false);

        cell_renderer
    }
}

/// Option to set for the LibraryCellRenderer
pub trait LibraryCellRendererExt {
    /// Set padding
    fn set_pad(&self, pad: i32);
    /// Set size
    fn set_size(&self, size: i32);
    /// Whether to draw the border
    fn set_drawborder(&self, draw: bool);
    /// Whether to draw the emblem
    fn set_drawemblem(&self, draw: bool);
    /// Whether to draw the rating
    fn set_drawrating(&self, draw: bool);
    /// Whether to draw the label
    fn set_drawlabel(&self, draw: bool);
    /// Whether to draw the flag
    fn set_drawflag(&self, draw: bool);
}

impl LibraryCellRendererExt for LibraryCellRenderer {
    fn set_pad(&self, pad: i32) {
        let priv_ = LibraryCellRendererPriv::from_instance(self);
        priv_.pad.set(pad);
    }
    fn set_size(&self, size: i32) {
        let priv_ = LibraryCellRendererPriv::from_instance(self);
        priv_.size.set(size);
    }
    fn set_drawborder(&self, draw: bool) {
        let priv_ = LibraryCellRendererPriv::from_instance(self);
        priv_.drawborder.set(draw);
    }
    fn set_drawemblem(&self, draw: bool) {
        let priv_ = LibraryCellRendererPriv::from_instance(self);
        priv_.draw_emblem.set(draw);
    }
    fn set_drawrating(&self, draw: bool) {
        let priv_ = LibraryCellRendererPriv::from_instance(self);
        priv_.draw_rating.set(draw);
    }
    fn set_drawlabel(&self, draw: bool) {
        let priv_ = LibraryCellRendererPriv::from_instance(self);
        priv_.draw_label.set(draw);
    }
    fn set_drawflag(&self, draw: bool) {
        let priv_ = LibraryCellRendererPriv::from_instance(self);
        priv_.draw_flag.set(draw);
    }
}

#[derive(Default)]
struct ClickableCell {
    x: i32,
    y: i32,
    hit: bool,
}

impl ClickableCellRenderer for LibraryCellRenderer {
    fn hit(&mut self, x: i32, y: i32) {
        let priv_ = LibraryCellRendererPriv::from_instance(self);
        priv_
            .clickable_cell
            .replace(ClickableCell { x, y, hit: true });
    }

    fn x(&self) -> i32 {
        let priv_ = LibraryCellRendererPriv::from_instance(self);
        priv_.clickable_cell.borrow().x
    }

    fn y(&self) -> i32 {
        let priv_ = LibraryCellRendererPriv::from_instance(self);
        priv_.clickable_cell.borrow().y
    }

    fn is_hit(&self) -> bool {
        let priv_ = LibraryCellRendererPriv::from_instance(self);
        priv_.clickable_cell.borrow().hit
    }

    fn reset_hit(&mut self) {
        let priv_ = LibraryCellRendererPriv::from_instance(self);
        priv_.clickable_cell.borrow_mut().hit = false;
    }
}

/// Callback type to get the label colour.
/// Return false if none is returned.
type GetColourCallback = unsafe extern "C" fn(i32, *mut RgbColour, *const c_void) -> bool;

pub struct LibraryCellRendererPriv {
    libfile: RefCell<Option<StoreLibFile>>,
    status: Cell<FileStatus>,
    size: Cell<i32>,
    pad: Cell<i32>,
    drawborder: Cell<bool>,
    draw_emblem: Cell<bool>,
    draw_rating: Cell<bool>,
    draw_label: Cell<bool>,
    draw_flag: Cell<bool>,
    draw_status: Cell<bool>,
    clickable_cell: RefCell<ClickableCell>,
    get_colour_callback: RefCell<Option<GetColourCallback>>,
    callback_data: Cell<*const c_void>,
}

impl LibraryCellRendererPriv {
    fn set_status(&self, status: FileStatus) {
        self.status.set(status);
    }

    fn set_libfile(&self, libfile: Option<StoreLibFile>) {
        self.libfile.replace(libfile);
    }

    fn do_draw_thumbnail(&self, cr: &cairo::Context, pixbuf: &Pixbuf, r: &gdk::Rectangle) {
        let w = pixbuf.get_width();
        let h = pixbuf.get_height();
        let offset_x = (self.size.get() - w) / 2;
        let offset_y = (self.size.get() - h) / 2;
        let x: f64 = (r.x + self.pad.get() + offset_x).into();
        let y: f64 = (r.y + self.pad.get() + offset_y).into();

        cr.set_source_rgb(1.0, 1.0, 1.0);
        cr.rectangle(x, y, w.into(), h.into());
        cr.stroke();

        cr.set_source_pixbuf(&pixbuf, x, y);
        cr.paint();
    }

    fn do_draw_flag(cr: &cairo::Context, flag: i32, r: &gdk::Rectangle) {
        if flag == 0 {
            return;
        }
        let pixbuf = match flag {
            -1 => EMBLEMS.flag_reject.clone(),
            1 => EMBLEMS.flag_pick.clone(),
            _ => return,
        };

        let w = pixbuf.get_width();
        let x: f64 = (r.x + r.width - CELL_PADDING - w).into();
        let y: f64 = (r.y + CELL_PADDING).into();
        cr.set_source_pixbuf(&pixbuf, x, y);
        cr.paint();
    }

    fn do_draw_status(cr: &cairo::Context, status: FileStatus, r: &gdk::Rectangle) {
        if status == FileStatus::Ok {
            return;
        }
        let x: f64 = (r.x + CELL_PADDING).into();
        let y: f64 = (r.y + CELL_PADDING).into();
        cr.set_source_pixbuf(&EMBLEMS.status_missing, x, y);
        cr.paint();
    }

    fn do_draw_format_emblem(cr: &cairo::Context, emblem: &Pixbuf, r: &gdk::Rectangle) -> i32 {
        let w = emblem.get_width();
        let h = emblem.get_height();
        let left = CELL_PADDING + w;
        let x: f64 = (r.x + r.width - left).into();
        let y: f64 = (r.y + r.height - CELL_PADDING - h).into();
        cr.set_source_pixbuf(emblem, x, y);
        cr.paint();
        left
    }

    fn do_draw_label(cr: &cairo::Context, right: i32, colour: RgbColour, r: &gdk::Rectangle) {
        const LABEL_SIZE: i32 = 15;
        let x: f64 = (r.x + r.width - CELL_PADDING - right - CELL_PADDING - LABEL_SIZE).into();
        let y: f64 = (r.y + r.height - CELL_PADDING - LABEL_SIZE).into();

        cr.rectangle(x, y, LABEL_SIZE.into(), LABEL_SIZE.into());
        cr.set_source_rgb(1.0, 1.0, 1.0);
        cr.stroke();
        cr.rectangle(x, y, LABEL_SIZE.into(), LABEL_SIZE.into());
        let rgb: gdk::RGBA = colour.into();
        cr.set_source_rgba(rgb.red, rgb.green, rgb.blue, rgb.alpha);
        cr.fill();
    }

    fn get_colour(&self, label_id: i32) -> Option<RgbColour> {
        if let Some(f) = *self.get_colour_callback.borrow() {
            unsafe {
                let mut c = RgbColour::default();
                if f(label_id, &mut c, self.callback_data.get()) {
                    return Some(c);
                }
            }
        }
        None
    }
}

static PROPERTIES: [subclass::Property; 2] = [
    subclass::Property("libfile", |libfile| {
        glib::ParamSpec::boxed(
            libfile,
            "Library File",
            "File from the library in the cell",
            StoreLibFile::get_type(),
            glib::ParamFlags::READWRITE,
        )
    }),
    subclass::Property("status", |status| {
        glib::ParamSpec::int(
            status,
            "File Status",
            "Status of the file in the cell",
            FileStatus::Ok as i32,
            FileStatus::Missing as i32,
            FileStatus::Ok as i32,
            glib::ParamFlags::READWRITE,
        )
    }),
];

impl ObjectSubclass for LibraryCellRendererPriv {
    const NAME: &'static str = "LibraryCellRenderer";
    type ParentType = gtk::CellRendererPixbuf;
    type Instance = subclass::simple::InstanceStruct<Self>;
    type Class = subclass::simple::ClassStruct<Self>;

    glib_object_subclass!();

    fn class_init(klass: &mut Self::Class) {
        klass.install_properties(&PROPERTIES);
        klass.add_signal(
            "rating-changed",
            glib::SignalFlags::RUN_LAST,
            &[Type::I64, Type::I32],
            Type::Unit,
        );
    }

    fn new() -> Self {
        Self {
            libfile: RefCell::new(None),
            status: Cell::new(FileStatus::Ok),
            size: Cell::new(160),
            pad: Cell::new(16),
            drawborder: Cell::new(true),
            draw_emblem: Cell::new(true),
            draw_rating: Cell::new(true),
            draw_label: Cell::new(true),
            draw_flag: Cell::new(true),
            draw_status: Cell::new(true),
            clickable_cell: RefCell::new(ClickableCell::default()),
            get_colour_callback: RefCell::new(None),
            callback_data: Cell::new(ptr::null()),
        }
    }
}

impl ObjectImpl for LibraryCellRendererPriv {
    glib_object_impl!();

    fn constructed(&self, obj: &glib::Object) {
        self.parent_constructed(obj);
    }

    fn set_property(&self, _obj: &glib::Object, id: usize, value: &glib::Value) {
        let prop = &PROPERTIES[id];
        match *prop {
            subclass::Property("libfile", ..) => {
                let libfile = value
                    .get::<&StoreLibFile>()
                    .expect("type conformity checked by `Object::set_property`")
                    .map(|f| f.clone());
                self.set_libfile(libfile);
            }
            subclass::Property("status", ..) => {
                let status: i32 = value
                    .get_some()
                    .expect("type conformity checked by `Object::set_property`");
                self.set_status(FileStatus::from(status));
            }
            _ => unimplemented!(),
        }
    }

    fn get_property(&self, _obj: &glib::Object, id: usize) -> Result<glib::Value, ()> {
        let prop = &PROPERTIES[id];

        match *prop {
            subclass::Property("libfile", ..) => Ok(self.libfile.borrow().to_value()),
            subclass::Property("status", ..) => Ok((self.status.get() as i32).to_value()),
            _ => unimplemented!(),
        }
    }
}

impl CellRendererPixbufImpl for LibraryCellRendererPriv {}

impl CellRendererImpl for LibraryCellRendererPriv {
    fn get_preferred_width<P: IsA<gtk::Widget>>(
        &self,
        _renderer: &gtk::CellRenderer,
        _widget: &P,
    ) -> (i32, i32) {
        let maxdim: i32 = self.size.get() + self.pad.get() * 2;
        (maxdim, maxdim)
    }

    fn get_preferred_height<P: IsA<gtk::Widget>>(
        &self,
        _renderer: &gtk::CellRenderer,
        _widget: &P,
    ) -> (i32, i32) {
        let maxdim: i32 = self.size.get() + self.pad.get() * 2;
        (maxdim, maxdim)
    }

    fn render<P: IsA<gtk::Widget>>(
        &self,
        _renderer: &gtk::CellRenderer,
        cr: &cairo::Context,
        widget: &P,
        _background_area: &gdk::Rectangle,
        cell_area: &gdk::Rectangle,
        flags: gtk::CellRendererState,
    ) {
        let self_ = self.get_instance();
        let xpad = self_.get_property_xpad();
        let ypad = self_.get_property_ypad();

        let mut r = cell_area.clone();
        r.x += xpad as i32;
        r.y += ypad as i32;

        let file = self.libfile.borrow();

        let style_context = widget.get_style_context();

        style_context.save();
        style_context.set_state(if flags.contains(gtk::CellRendererState::SELECTED) {
            gtk::StateFlags::SELECTED
        } else {
            gtk::StateFlags::NORMAL
        });
        gtk::render_background(
            &style_context,
            cr,
            (r.x).into(),
            (r.y).into(),
            (r.width).into(),
            (r.height).into(),
        );

        if self.drawborder.get() {
            gtk::render_frame(
                &style_context,
                cr,
                (r.x).into(),
                (r.y).into(),
                (r.width).into(),
                (r.height).into(),
            );
        }
        style_context.restore();

        if let Some(pixbuf) = self_.get_property_pixbuf() {
            self.do_draw_thumbnail(cr, &pixbuf, &r);
        }
        if self.draw_rating.get() {
            let rating = match &*file {
                Some(f) => f.0.rating(),
                None => 0,
            };
            let x: f64 = (r.x + CELL_PADDING).into();
            let y: f64 = (r.y + r.height - CELL_PADDING).into();
            RatingLabel::draw_rating(
                cr,
                rating,
                &RatingLabel::get_star(),
                &RatingLabel::get_unstar(),
                x,
                y,
            );
        }
        if self.draw_flag.get() {
            match &*file {
                Some(f) => Self::do_draw_flag(cr, f.0.flag(), &r),
                None => {}
            }
        }

        let status = self.status.get();
        if self.draw_status.get() && status != FileStatus::Ok {
            Self::do_draw_status(cr, status, &r);
        }

        if self.draw_emblem.get() {
            let file_type = match &*file {
                Some(f) => f.0.file_type(),
                None => FileType::UNKNOWN,
            };
            let emblem: Pixbuf = match file_type {
                FileType::RAW => EMBLEMS.raw.clone(),
                FileType::RAW_JPEG => EMBLEMS.raw_jpeg.clone(),
                FileType::IMAGE => EMBLEMS.img.clone(),
                FileType::VIDEO => EMBLEMS.video.clone(),
                FileType::UNKNOWN => EMBLEMS.unknown.clone(),
            };
            let left = Self::do_draw_format_emblem(cr, &emblem, &r);

            if self.draw_label.get() {
                let label_id = match &*file {
                    Some(f) => f.0.label(),
                    None => 0,
                };
                if label_id != 0 {
                    if let Some(colour) = self.get_colour(label_id) {
                        Self::do_draw_label(cr, left, colour, &r);
                    }
                }
            }
        }
    }

    fn activate<P: IsA<gtk::Widget>>(
        &self,
        _renderer: &gtk::CellRenderer,
        _event: Option<&gdk::Event>,
        _widget: &P,
        _path: &str,
        _background_area: &gdk::Rectangle,
        cell_area: &gdk::Rectangle,
        _flags: gtk::CellRendererState,
    ) -> bool {
        let mut instance = self
            .get_instance()
            .downcast::<LibraryCellRenderer>()
            .unwrap();

        if instance.is_hit() {
            instance.reset_hit();

            // hit test with the rating region
            let xpad = instance.get_property_xpad();
            let ypad = instance.get_property_ypad();
            let mut r = cell_area.clone();
            r.x += xpad as i32;
            r.y += ypad as i32;

            let (rw, rh) = RatingLabel::get_geometry();
            let rect = gdk::Rectangle {
                x: r.x + CELL_PADDING,
                y: r.y + r.height - rh - CELL_PADDING,
                width: rw,
                height: rh,
            };
            let x = instance.x();
            let y = instance.y();
            dbg_out!(
                "r({}, {}, {}, {}) p({}, {})",
                rect.x,
                rect.y,
                rect.width,
                rect.height,
                x,
                y
            );
            let hit = (rect.x <= x)
                && (rect.x + rect.width >= x)
                && (rect.y <= y)
                && (rect.y + rect.height >= y);
            if !hit {
                dbg_out!("not a hit");
                return false;
            }

            // hit test for the rating value
            let new_rating = RatingLabel::rating_value_from_hit_x((x - rect.x).into());
            dbg_out!("new_rating {}", new_rating);

            let file = self.libfile.borrow();
            if let Some(f) = &*file {
                if f.0.rating() != new_rating {
                    // emit signal if changed
                    if let Err(err) = instance.emit("rating-changed", &[&f.0.id(), &new_rating]) {
                        err_out!("Can't emit rating-changed signal: {}", err);
                    }
                }
            }
            true
        } else {
            false
        }
    }
}

// allow subclassing this
pub trait LibraryCellRendererImpl: CellRendererPixbufImpl + 'static {}

unsafe impl<T: ObjectSubclass + LibraryCellRendererImpl> IsSubclassable<T>
    for LibraryCellRendererClass
{
    fn override_vfuncs(&mut self) {
        <CellRendererPixbufClass as IsSubclassable<T>>::override_vfuncs(self);
    }
}

#[no_mangle]
pub unsafe extern "C" fn npc_library_cell_renderer_new(
    get_colour: Option<unsafe extern "C" fn(i32, *mut RgbColour, *const c_void) -> bool>,
    callback_data: *const c_void,
) -> *mut gtk_sys::GtkCellRenderer {
    LibraryCellRenderer::new(get_colour, callback_data)
        .upcast::<gtk::CellRenderer>()
        .to_glib_full()
}

/// Hit test for the cellrenderer.
#[no_mangle]
pub unsafe extern "C" fn npc_library_cell_renderer_hit(
    ptr: *mut gtk_sys::GtkCellRenderer,
    x: i32,
    y: i32,
) {
    let mut renderer = gtk::CellRenderer::from_glib_borrow(ptr)
        .downcast::<LibraryCellRenderer>()
        .expect("Expected a LibraryCellRenderer");
    renderer.hit(x, y);
}
