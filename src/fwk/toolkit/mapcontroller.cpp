/*
 * niepce - fwk/toolkit/mapcontroller.cpp
 *
 * Copyright (C) 2014 Hubert Figuiere
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

#include "mapcontroller.hpp"

#include <champlain-gtk/champlain-gtk.h>

namespace fwk {

Gtk::Widget *
MapController::buildWidget(const Glib::RefPtr<Gtk::UIManager> &)
{
  if(m_widget) {
    return m_widget;
  }

  GtkWidget * embed = gtk_champlain_embed_new();
  m_widget = Gtk::manage(Glib::wrap(embed));

  ChamplainView* clutter_map =
    gtk_champlain_embed_get_view(GTK_CHAMPLAIN_EMBED(embed));
  m_clutter_map = Glib::wrap(CLUTTER_ACTOR(clutter_map), true);

  return m_widget;
}


}
