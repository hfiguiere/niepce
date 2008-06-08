/*
 * niepce - framework/controller.cpp
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

#include <boost/bind.hpp>

#include <gtkmm/widget.h>

#include "controller.h"


namespace framework {

	Controller::Controller()
		: m_widget(NULL)
	{
	}


	Controller::~Controller()
	{
	}

	Gtk::Widget * Controller::widget()
	{
		if(m_widget == NULL) 
		{
			m_widget = buildWidget();
		}
		return m_widget;
	}

	void
	Controller::add(const Controller::Ptr & sub)
	{
		m_subs.push_back(sub);
		sub->m_parent = shared_from_this();
		sub->_added();
	}

	void Controller::remove(const Ptr & sub)
	{
		std::list<Ptr>::iterator iter = std::find(m_subs.begin(), 
																							m_subs.end(), sub);
		if(iter != m_subs.end()) {
			(*iter)->clearParent();
			m_subs.erase(iter);
		}
	}

	bool Controller::canTerminate()
	{
		return true;
	}

	void Controller::terminate()
	{
	}

	void Controller::_added()
	{
	}

	void Controller::_ready()
	{
		std::for_each(m_subs.begin(), m_subs.end(),
					  boost::bind(&Controller::_ready, _1));
		on_ready();
	}

	void Controller::on_ready()
	{
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
