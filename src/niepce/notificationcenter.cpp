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
#include "niepce/notifications.hpp"
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

NotificationCenter::NotificationCenter(uint64_t notif_id)
    : fwk::NotificationCenter(notif_id)
    , m_channel(ffi::lcchannel_new(&channel_callback, this), ffi::lcchannel_delete)
{
    subscribe(NOTIFICATION_THUMBNAIL,
              sigc::mem_fun(*this, &NotificationCenter::dispatch_notification));
}

NotificationCenter::~NotificationCenter()
{
    ffi::lcchannel_destroy(m_channel.get());
}

void NotificationCenter::dispatch_lib_notification(const eng::LibNotification *n) const
{
    signal_lib_notification(*n);
}

void NotificationCenter::dispatch_notification(const fwk::Notification::Ptr &n) const
{
    try {
        switch(n->type()) {
        case NOTIFICATION_THUMBNAIL:
        {
            eng::ThumbnailNotification tn
                = boost::any_cast<eng::ThumbnailNotification>(n->data());
            signal_thumbnail_notification(tn);
            break;
        }
        default:
            ERR_OUT("Unhandled notification type %d", n->type());
            break;
        }
    }
    catch(const boost::bad_any_cast & e)
    {
        ERR_OUT("improper notification data: %s", e.what());
    }
    catch(const std::exception & e)
    {
        ERR_OUT("other exception notification type %d of %s: %s", n->type(),
                n->data().type().name(),
                e.what());
    }
    catch(...)
    {
        ERR_OUT("unknown exception");
    }
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
