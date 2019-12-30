/*
 * niepce - fwk/toolkit/mapcontroller.cpp
 *
 * Copyright (C) 2014-2019 Hubert Figuière
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

#include <gtkmm/widget.h>

namespace fwk {

class MapController::Priv {
public:
    Priv()
        : m_clutter_map(nullptr)
        {
        }
    ~Priv()
        {
            if (m_clutter_map) {
                g_object_unref(G_OBJECT(m_clutter_map));
            }
        }
    ChamplainView* m_clutter_map;
};

MapController::MapController()
    : UiController()
    , m_priv(new Priv)
{
}

MapController::~MapController()
{
    delete m_priv;
}

Gtk::Widget *
MapController::buildWidget()
{
    if (m_widget) {
        return m_widget;
    }

    auto embed = gtk_champlain_embed_new();
    m_widget = Gtk::manage(Glib::wrap(embed));

    m_priv->m_clutter_map = gtk_champlain_embed_get_view(GTK_CHAMPLAIN_EMBED(embed));
    g_object_ref(G_OBJECT(m_priv->m_clutter_map));

    // Default position. Somewhere over Montréal, QC
    setZoomLevel(10);
    centerOn(45.5030854, -73.5698944);

    return m_widget;
}

void MapController::centerOn(double lat, double longitude)
{
    champlain_view_center_on(m_priv->m_clutter_map, lat, longitude);
}

void MapController::zoomIn()
{
    champlain_view_zoom_in(m_priv->m_clutter_map);
}

void MapController::zoomOut()
{
    champlain_view_zoom_out(m_priv->m_clutter_map);
}

void MapController::setZoomLevel(uint8_t level)
{
    champlain_view_set_zoom_level(m_priv->m_clutter_map, level);
}

}
/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  c-basic-offset:4
  tab-width:4
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
