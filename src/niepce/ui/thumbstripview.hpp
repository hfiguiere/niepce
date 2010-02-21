/*
 * niepce - niepce/ui/thumbstripview.hpp
 *
 * Copyright (C) 2009 Hubert Figuiere
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

#ifndef _THUMB_STRIP_VIEW_HPP_
#define _THUMB_STRIP_VIEW_HPP_

#include <gtkmm/iconview.h>
#include <gtkmm/cellrendererpixbuf.h>

#include "niepce/ui/imageliststore.hpp"

namespace ui {

class ThumbStripView
  : public Gtk::IconView
{
public:
    ThumbStripView(const Glib::RefPtr<ui::ImageListStore> & store);

    void set_model(const Glib::RefPtr<ui::ImageListStore> & store);
    const Glib::RefPtr<ui::ImageListStore> & get_model() const;
    void set_item_height(int height);

private:
    void on_parent_set(Gtk::Widget* previous_parent);
    void on_visible_range_changed();
    void on_adjustment_changed();
    void on_drag_data_get(const Glib::RefPtr<Gdk::DragContext>&,
                          Gtk::SelectionData&,guint,guint);
    void update_visible_range(int, int);
    void add_range(int, int);
    void clear_range (int start_thumb, int end_thumb);

    gint m_start_thumb; /* the first visible thumbnail */
    gint m_end_thumb;   /* the last visible thumbnail  */
    Glib::RefPtr<ui::ImageListStore> m_store;
    Gtk::CellRendererPixbuf  *m_renderer;
};


}

#endif
/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/