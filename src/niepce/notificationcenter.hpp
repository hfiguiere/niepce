/*
 * niepce - niepce/notificationcenter.hpp
 *
 * Copyright (C) 2009-2013 Hubert Figuiere
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


#ifndef __NIEPCE_NOTIFICATIONCENTER_HPP_
#define __NIEPCE_NOTIFICATIONCENTER_HPP_

#include <memory>
#include <sigc++/signal.h>

#include "fwk/toolkit/notificationcenter.hpp"
#include "engine/library/notification.hpp"
#include "engine/library/thumbnailnotification.hpp"


namespace niepce {


class NotificationCenter
  : public fwk::NotificationCenter
{
public:
  typedef std::shared_ptr<NotificationCenter> Ptr;
  static Ptr make(uint64_t notif_id)
    {
      Ptr nc = Ptr(new NotificationCenter(notif_id));
      nc->attach();
      return nc;
    }

  sigc::signal<void, const eng::LibNotification &> signal_lib_notification;
  sigc::signal<void, const eng::ThumbnailNotification &> signal_thumbnail_notification;

protected:
  NotificationCenter(uint64_t notif_id);

private:
  void dispatch_notification(const fwk::Notification::Ptr &n);
};

}

#endif
/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  c-basic-offset: 2
  tab-width: 2
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
