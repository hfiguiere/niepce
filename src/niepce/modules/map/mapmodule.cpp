/*
 * niepce - modules/map/mapmodule.cpp
 *
 * Copyright (C) 2014-2017 Hubert Figuiere
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

#include <gtkmm/box.h>

#include "fwk/base/debug.hpp"
#include "fwk/toolkit/application.hpp"
#include "engine/db/properties.hpp"
#include "mapmodule.hpp"

namespace mapm {

MapModule::MapModule(const ui::IModuleShell & shell)
    : m_shell(shell)
    , m_box(nullptr)
    , m_active(false)
{
}

void MapModule::dispatch_action(const std::string & /*action_name*/)
{
}


void MapModule::set_active(bool active)
{
    m_active = active;
}


Gtk::Widget * MapModule::buildWidget()
{
    if(m_widget) {
        return m_widget;
    }

    m_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));
    m_widget = m_box;

    m_map = fwk::MapController::Ptr(new fwk::MapController());
    add(m_map);
    auto w = m_map->buildWidget();
    if (w) {
        m_box->pack_start(*w);
    }

    return m_widget;
}

void
MapModule::on_lib_notification(const eng::LibNotification &ln)
{
    if (!m_active) {
        return;
    }
    switch(ln.type) {
    case eng::Library::NotifyType::METADATA_QUERIED:
    {
        DBG_ASSERT(ln.param.type() == typeid(eng::LibMetadata::Ptr),
                   "incorrect data type for the notification");
        auto lm = boost::any_cast<eng::LibMetadata::Ptr>(ln.param);
        DBG_OUT("received metadata in MapModule");

        if (lm) {
            fwk::PropertyBag properties;
            const fwk::PropertySet propset = { eng::NpExifGpsLongProp,
                                               eng::NpExifGpsLatProp };
            lm->to_properties(propset, properties);
            double latitude, longitude;
            latitude = longitude = NAN;
            auto result = properties.get_value_for_property(eng::NpExifGpsLongProp);
            if (!result.empty()) {
                fwk::PropertyValue val = result.unwrap();
                // it is a string
                if (is_string(val)) {
                    longitude = fwk::XmpMeta::gpsCoordFromXmp(
                        fwk::get_string(val));
                }
            }
            result = properties.get_value_for_property(eng::NpExifGpsLatProp);
            if (!result.empty()) {
                fwk::PropertyValue val = result.unwrap();
                // it is a string
                if (is_string(val)) {
                    latitude = fwk::XmpMeta::gpsCoordFromXmp(
                        fwk::get_string(val));
                }
            }

            if (!std::isnan(latitude) && !std::isnan(longitude)) {
                m_map->centerOn(latitude, longitude);
            }
        }
        break;
    }
    default:
        break;
    }
}

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
