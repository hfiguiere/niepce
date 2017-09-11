/*
 * niepce - niepce/notificationcenter.hpp
 *
 * Copyright (C) 2009, 2013 Hubert Figuiere
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
#include "engine/library/notification.hpp"
#include "niepce/notifications.hpp"
#include "niepce/notificationcenter.hpp"

namespace niepce {

NotificationCenter::NotificationCenter(uint64_t notif_id)
    : fwk::NotificationCenter(notif_id)
{
  subscribe(NOTIFICATION_LIB,
            sigc::mem_fun(*this, &NotificationCenter::dispatch_notification));
  subscribe(NOTIFICATION_THUMBNAIL,
            sigc::mem_fun(*this, &NotificationCenter::dispatch_notification));
}


void NotificationCenter::dispatch_notification(const fwk::Notification::Ptr &n)
{
    try {
        switch(n->type()) {
        case NOTIFICATION_LIB:
        {
            auto ln = boost::any_cast<eng::LibNotificationPtr>(n->data());
            signal_lib_notification(*ln);
            break;
        }
        case NOTIFICATION_THUMBNAIL:
        {
            eng::ThumbnailNotification tn
                = boost::any_cast<eng::ThumbnailNotification>(n->data());
            signal_thumbnail_notification(tn);
            break;
        }
        default:
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
