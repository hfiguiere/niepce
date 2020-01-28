/*
 * niepce - fwk/toolkit/widgets/imagegridview.hpp
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

#pragma once

#include <gtkmm/iconview.h>

namespace fwk {

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
