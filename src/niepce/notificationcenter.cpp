/*
 * niepce - niepce/notificationcenter.cpp
 *
 * Copyright (C) 2009-2019 Hubert Figuière
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

#include <boost/any.hpp>

#include "fwk/base/debug.hpp"
#include "niepce/notificationcenter.hpp"

namespace niepce {

int32_t NotificationCenter::channel_callback(const eng::LibNotification *notification, void *data)
{
    DBG_ASSERT(data, "Notification center is NULL");
    if (data) {
        auto self = (NotificationCenter*)data;
        self->dispatch_lib_notification(notification);
        DBG_OUT("callback called");
    }
    return 1;
}

NotificationCenter::NotificationCenter()
    : m_channel(ffi::lcchannel_new(&channel_callback, this), ffi::lcchannel_delete)
{
}

NotificationCenter::~NotificationCenter()
{
}

void NotificationCenter::dispatch_lib_notification(const eng::LibNotification *n) const
{
    signal_lib_notification(*n);
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
