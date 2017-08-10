/*
 * niepce - fwk/toolkit/notification.cpp
 *
 * Copyright (C) 2007-2017 Hubert Figuière
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

#include <list>
#include <map>

#include <glibmm/dispatcher.h>

#include "fwk/base/singleton.hpp"
#include "fwk/utils/mtqueue.hpp"
#include "notificationcenter.hpp"

namespace fwk {

class NotificationCenter::Priv
{
public:
    uint64_t m_id;
    Glib::Dispatcher                     m_dispatcher;
    fwk::MtQueue< Notification::Ptr >    m_notificationQueue;
    std::map< int, subscription_t >      m_subscribers;
};

typedef fwk::Singleton<std::map<uint64_t, std::weak_ptr<NotificationCenter>>> NotificationCenterRegistry;

std::weak_ptr<NotificationCenter> NotificationCenter::get_nc(uint64_t notif_id)
{
    auto iter = NotificationCenterRegistry::obj().find(notif_id);
    if (iter == NotificationCenterRegistry::obj().end()) {
        return NotificationCenter::Ptr();
    }
    return iter->second;
}

NotificationCenter::NotificationCenter(uint64_t notif_id)
    : p( new Priv )
{
    p->m_id = notif_id;
    p->m_dispatcher.connect(
        sigc::mem_fun(this, &NotificationCenter::_dispatch));
}

NotificationCenter::~NotificationCenter()
{
    NotificationCenterRegistry::obj().erase(p->m_id);
    delete p;
}

void NotificationCenter::attach()
{
    NotificationCenterRegistry::obj()[p->m_id] = shared_from_this();
}

uint64_t NotificationCenter::id() const
{
    return p->m_id;
}

void NotificationCenter::subscribe(int type, const subscriber_t & s)
{
    // TODO make sure it is not yet subscribed
    p->m_subscribers[type].connect(s);
}

void NotificationCenter::unsubscribe(int /*type*/, const subscriber_t & /*s*/)
{
//		m_subscribers.remove_if(std::bind(&boost::function_equal, _1, s));
}

void NotificationCenter::post(Notification::Ptr&& n)
{
    /* called out of thread */
    /* MUST be thread safe */
    p->m_notificationQueue.add(n);
    p->m_dispatcher.emit();
}

void NotificationCenter::_dispatch(void)
{
    /* this is not pop() like in STL. */
    Notification::Ptr notif( p->m_notificationQueue.pop() );

    p->m_subscribers[notif->type()](notif);
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
