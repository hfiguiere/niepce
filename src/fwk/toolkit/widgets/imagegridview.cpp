/*
 * niepce - fwk/toolkit/widgets/imagegridview.cpp
 *
 * Copyright (C) 2011 Hubert Figuiere
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

namespace fwk {

void ClickableCellRenderer::hit(int x, int y)
{
  m_x = x;
  m_y = y;
  m_hit = true;
}

ImageGridView::ImageGridView()
  : Gtk::IconView()
{
}

bool ImageGridView::on_button_press_event(GdkEventButton *event)
{
  bool ret = Gtk::IconView::on_button_press_event(event);

  Gtk::CellRenderer* cell = NULL;
  bool found = get_item_at_pos(event->x, event->y, cell);
  if(found) {
    ClickableCellRenderer* clickable_cell = dynamic_cast<ClickableCellRenderer*>(cell);
    
    if(clickable_cell) {
      DBG_OUT("clickable cell");
      clickable_cell->hit(event->x, event->y);
    }
  }

  return ret;
}

}
