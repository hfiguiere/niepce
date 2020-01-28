/*
 * niepce - fwk/toolkit/widgets/imagegridview.cpp
 *
 * Copyright (C) 2011-2020 Hubert Figuière
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

#include "fwk/base/debug.hpp"
#include "imagegridview.hpp"

#include "rust_bindings.hpp"

namespace fwk {

ImageGridView::ImageGridView(const Glib::RefPtr<Gtk::TreeModel> & model)
  : Gtk::IconView(model)
{
}

ImageGridView::ImageGridView()
  : Gtk::IconView()
{
}

bool ImageGridView::on_button_press_event(GdkEventButton *event)
{
  bool ret = Gtk::IconView::on_button_press_event(event);

  Gtk::CellRenderer* cell = nullptr;
  bool found = get_item_at_pos(event->x, event->y, cell);
  if (found) {
    if (strcmp(G_OBJECT_TYPE_NAME(cell->gobj()), "LibraryCellRenderer") == 0) {
      DBG_OUT("clickable cell");
      ffi::npc_library_cell_renderer_hit(GTK_CELL_RENDERER(cell->gobj()), event->x, event->y);
    }
  }

  return ret;
}

}
