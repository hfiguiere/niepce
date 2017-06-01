/*
 * niepce - fwk/notification.h
 *
 * Copyright (C) 2007-2013 Hubert Figuiere
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





#ifndef __FWK_NOTIFICATION_H__
#define __FWK_NOTIFICATION_H__

#include <memory>
#include <boost/any.hpp>

namespace fwk {

	/** A notification to post to the notification center */
	class Notification
	{
	public:
		typedef std::shared_ptr<Notification> Ptr;

		Notification(int _type)
			: m_type(_type)
			{}
		~Notification()
			{}
		int type() const
			{ return m_type; }
		const boost::any & data() const
			{ return m_data; }
		void setData(const boost::any & d)
			{ m_data = d; }
	private:
		int        m_type;
		boost::any m_data;
	};

}


#endif
