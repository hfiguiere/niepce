/*
 * niepce - fwk/toolkit/widgets/imagegridview.hpp
 *
 * Copyright (C) 2011-2013 Hubert Figuiere
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

#ifndef IMAGE_GRID_VIEW_H__
#define IMAGE_GRID_VIEW_H__

#include <gtkmm/iconview.h>
#include <gtkmm/cellareabox.h>

namespace fwk {

/**
 * Clickable Cell Renderer
 * To work around https://bugzilla.gnome.org/show_bug.cgi?id=664368
 */
class ClickableCellRenderer
{
public:
  ClickableCellRenderer()
    : m_x(0)
    , m_y(0)
    , m_hit(false)
  {
  }
  /**
   * Hit.
   */
  void hit(int x, int y);
  int x() const 
  {
    return m_x;
  }
  int y() const
  {
    return m_y;
  }
  bool is_hit() const
  {
    return m_hit;
  }
  void reset_hit()
  {
    m_hit = false;
  }
private:
  int m_x, m_y;
  bool m_hit;
};

class ImageGridView
  : public Gtk::IconView
{
public:
  ImageGridView();
  ImageGridView(const Glib::RefPtr<Gtk::TreeModel> & model);

  /**
   * Used to work around the activate that don't pass an event.
   * https://bugzilla.gnome.org/show_bug.cgi?id=664368
   */
  virtual bool on_button_press_event(GdkEventButton *event) override;

private:
};

}

#endif
