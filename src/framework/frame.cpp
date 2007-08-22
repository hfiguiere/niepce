/*
 * niepce - framework/application.cpp
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


#include <gtkmm/window.h>

#include "frame.h"


namespace framework {

	Frame::Frame()
		: m_window(new Gtk::Window()),
			m_glade(NULL)
	{
	}


	Frame::Frame(const std::string & gladeFile, const Glib::ustring & widgetName)
		: m_window(NULL),
			m_glade(Gnome::Glade::Xml::create(gladeFile))
	{
		if (m_glade) {
			m_window = static_cast<Gtk::Window*>(m_glade->get_widget(widgetName));
		}
	}

	Frame::~Frame()
	{
	}
}
