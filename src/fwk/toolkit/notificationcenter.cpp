/*
 * niepce - fwk/notification.h
 *
 * Copyright (C) 2007 Hubert Figuiere
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
#include <functional>

#include <boost/bind.hpp>
#include <boost/bind/apply.hpp>
#include <boost/function_equal.hpp>

#include <glibmm/dispatcher.h>

#include "fwk/utils/mtqueue.hpp"
#include "notificationcenter.hpp"

namespace fwk {

	class NotificationCenter::Priv
	{
	public:
		typedef std::list< subscriber_t > SubscriberList;
		Glib::Dispatcher                     m_dispatcher;
		sigc::connection                     m_dispatchConn;
		utils::MtQueue< Notification::Ptr >  m_notificationQueue;
		std::map< int, SubscriberList >      m_subscribers;
	};


	NotificationCenter::NotificationCenter()
		: p( new Priv )
	{
		p->m_dispatchConn = p->m_dispatcher.connect(
			sigc::mem_fun(this, &NotificationCenter::_dispatch));
	}

	NotificationCenter::~NotificationCenter()
	{
		p->m_dispatchConn.disconnect();
		delete p;
	}


	void NotificationCenter::subscribe(int type, const subscriber_t & s)
	{
		
		// TODO make sure it is not yet subscribed
		p->m_subscribers[type].push_back(s);
	}

	void NotificationCenter::unsubscribe(int /*type*/, const subscriber_t & /*s*/)
	{
//		m_subscribers.remove_if(boost::bind(&boost::function_equal, _1, s));
	}

	void NotificationCenter::post(const Notification::Ptr & n)
	{ 
		p->m_notificationQueue.add(n);
		p->m_dispatcher.emit();
	}
	
	void NotificationCenter::_dispatch(void)
	{
		Notification::Ptr notif( p->m_notificationQueue.pop() );

		Notification::mutex_t::Lock lock(notif->mutex());
		const Priv::SubscriberList & subscriber_list(p->m_subscribers[notif->type()]);
		std::for_each(subscriber_list.begin(), subscriber_list.end(), 
					  bind(boost::apply<void>(), _1, boost::ref(notif)));
	}
}

