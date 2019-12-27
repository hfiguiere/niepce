/*
 * niepce - ui/librarycellrenderer.h
 *
 * Copyright (C) 2008-2019 Hubert Figuière
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

#pragma once

#include <glibmm/property.h>
#include <gtkmm/cellrendererpixbuf.h>
#include <cairomm/surface.h>

#include "engine/db/libfile.hpp"
#include "fwk/toolkit/widgets/imagegridview.hpp"

namespace ui {

class IModuleShell;

class LibraryCellRenderer
    : public Gtk::CellRendererPixbuf
    , public fwk::ClickableCellRenderer
{
public:
    LibraryCellRenderer(const IModuleShell& shell);

    virtual void get_preferred_width_vfunc(Gtk::Widget& widget, int& minimum_width, int& natural_width) const override;
    virtual void get_preferred_height_vfunc(Gtk::Widget& widget, int& minimum_height, int& natural_height) const override;

    virtual void render_vfunc(const Cairo::RefPtr<Cairo::Context>& cr,
                               Gtk::Widget& widget,
                               const Gdk::Rectangle& background_area,
                               const Gdk::Rectangle& cell_area,
                               Gtk::CellRendererState flags) override;
    /** call when the cell is clicked */
    virtual bool activate_vfunc(GdkEvent* event, Gtk::Widget& widget,const Glib::ustring & path,
                                const Gdk::Rectangle& background_area,
                                const Gdk::Rectangle& cell_area, Gtk::CellRendererState flags) override;
    void set_size(int _size)
        { m_size = _size; }
    int size() const
        { return m_size; }

    void set_pad(int _pad)
        { m_pad = _pad; }
    int pad() const
        { return m_pad; }
    void set_drawborder(bool val)
        { m_drawborder = val; }
    void set_drawemblem(bool val)
        { m_drawemblem = val; }
    void set_drawrating(bool val)
        { m_drawrating = val; }
    void set_drawlabel(bool val)
        { m_drawlabel = val; }
    void set_drawflag(bool val)
        { m_drawflag = val; }

    Glib::PropertyProxy_ReadOnly<eng::LibFilePtr> property_libfile() const;
    Glib::PropertyProxy<eng::LibFilePtr>          property_libfile();
    sigc::signal<void, int, int> signal_rating_changed;
protected:
    /* drawing implementations */
    void draw_thumbnail(const Cairo::RefPtr<Cairo::Context>& cr,
                        Glib::RefPtr<Gdk::Pixbuf>& pixbuf,
                        const GdkRectangle& r) const;
    void draw_flag(const Cairo::RefPtr<Cairo::Context>& cr,
                   int flag_value, const GdkRectangle& r) const;
    void draw_status(const Cairo::RefPtr<Cairo::Context>& cr,
                     eng::FileStatus status, const GdkRectangle& r) const;

private:
    const IModuleShell& m_shell;
    int                                 m_size;
    int                                 m_pad;
    bool                                m_drawborder;
    bool                                m_drawemblem;
    bool                                m_drawrating;
    bool                                m_drawlabel;
    bool                                m_drawflag;
    bool m_drawstatus;
    Glib::Property<eng::LibFilePtr>   m_libfileproperty;
    Glib::Property<eng::FileStatus> m_statusproperty;

    Glib::RefPtr<Gdk::Pixbuf>  m_raw_format_emblem;
    Glib::RefPtr<Gdk::Pixbuf>  m_rawjpeg_format_emblem;
    Glib::RefPtr<Gdk::Pixbuf>  m_img_format_emblem;
    Glib::RefPtr<Gdk::Pixbuf>  m_video_format_emblem;
    Glib::RefPtr<Gdk::Pixbuf>  m_unknown_format_emblem;

    Glib::RefPtr<Gdk::Pixbuf>  m_status_missing;

    Glib::RefPtr<Gdk::Pixbuf>  m_flag_reject;
    Glib::RefPtr<Gdk::Pixbuf>  m_flag_pick;
};

}
/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:80
  End:
*/
