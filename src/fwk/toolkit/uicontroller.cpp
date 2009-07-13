/*
 * niepce - fwk/toolkit/uicontroller.hpp
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

#include "fwk/base/debug.hpp"
#include "uicontroller.hpp"

namespace fwk {

UiController::UiController()
    : m_widget(NULL)
    , m_ui_merge_id(0)
{
}

UiController::~UiController()
{
    if(m_uimanager && m_ui_merge_id) {
        m_uimanager->remove_ui(m_ui_merge_id);
    }
}


Gtk::Widget * UiController::widget() const
{
    DBG_ASSERT(!m_parent.expired(), "must be attached");
    return m_widget;
}

}


/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
