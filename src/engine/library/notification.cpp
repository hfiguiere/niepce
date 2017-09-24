/* -*- mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode:nil; -*- */
/*
 * niepce - engine/library/notification.cpp
 *
 * Copyright (C) 2017 Hubert Figuière
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

#include "engine/library/notification.hpp"
#include "engine/db/keyword.hpp"
#include "fwk/base/debug.hpp"
#include "fwk/toolkit/notificationcenter.hpp"
#include "niepce/notifications.hpp"

namespace {

/**
 * Wrap a pointer (from Rust) into a shared_ptr<>
 */
eng::LibNotificationPtr
engine_library_notification_wrap(eng::LibNotification* n)
{
    return eng::LibNotificationPtr(n, &ffi::engine_library_notification_delete);
}
}

extern "C" void
engine_library_notify(uint64_t notif_id,
                      eng::LibNotification* notification)
{
    auto ln = engine_library_notification_wrap(notification);
    auto wnc = fwk::NotificationCenter::get_nc(notif_id);
    auto nc = wnc.lock();
    if (nc) {
        DBG_OUT("notif");
        // pass the notification
        fwk::Notification::Ptr n(
            new fwk::Notification(niepce::NOTIFICATION_LIB));
        n->setData(boost::any(ln));
        nc->post(std::move(n));
    } else {
        DBG_OUT("try to send a notification without notification center");
    }
}
