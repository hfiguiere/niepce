/*
 * niepce - modules/map/mapmodule.cpp
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

#include <gtkmm/box.h>

#include "fwk/base/debug.hpp"
#include "fwk/toolkit/application.hpp"
#include "mapmodule.hpp"

namespace mapm {

MapModule::MapModule(const ui::IModuleShell & shell,
                               const Glib::RefPtr<Gtk::ActionGroup> & action_group)
    : m_shell(shell)
    , m_actionGroup(action_group)
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


Gtk::Widget * MapModule::buildWidget(const Glib::RefPtr<Gtk::UIManager> & manager)
{
    if(m_widget) {
        return m_widget;
    }

    m_box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));
    m_widget = m_box;

    m_map = fwk::MapController::Ptr(new fwk::MapController());
    add(m_map);
    Gtk::Widget* w = m_map->buildWidget(manager);
    if (w) {
        m_box->pack_start(*w);
    }

    return m_widget;
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
