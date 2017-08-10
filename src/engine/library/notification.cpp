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

#include "fwk/base/debug.hpp"
#include "niepce/notifications.hpp"
#include "engine/library/notification.hpp"
#include "engine/db/keyword.hpp"
#include "fwk/toolkit/notificationcenter.hpp"

namespace {

void notify(uint64_t notif_id, eng::LibNotification&& ln)
{
    auto wnc = fwk::NotificationCenter::get_nc(notif_id);
    auto nc = wnc.lock();
    if (nc) {
        DBG_OUT("notif");
        // pass the notification
        fwk::Notification::Ptr n(new fwk::Notification(niepce::NOTIFICATION_LIB));
        n->setData(boost::any(ln));
        nc->post(std::move(n));
    } else {
        DBG_OUT("try to send a notification without notification center");
    }
}

}

// Rust glue
extern "C" {

void lib_notification_notify_new_lib_created(uint64_t notif_id)
{
    eng::LibNotification ln =
        eng::LibNotification::make<eng::LibNotification::Type::NEW_LIBRARY_CREATED>({});
    notify(notif_id, std::move(ln));
}

void lib_notification_notify_xmp_needs_update(uint64_t notif_id)
{
    eng::LibNotification ln =
        eng::LibNotification::make<eng::LibNotification::Type::XMP_NEEDS_UPDATE>({});
    notify(notif_id, std::move(ln));
}

void lib_notification_notify_kw_added(uint64_t notif_id, eng::Keyword* keyword)
{
    eng::KeywordPtr kw = eng::keyword_wrap(keyword);
    eng::LibNotification ln =
        eng::LibNotification::make<eng::LibNotification::Type::ADDED_KEYWORD>({kw});
    notify(notif_id, std::move(ln));
}


}
