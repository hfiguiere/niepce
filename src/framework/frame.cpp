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

#include <list>
#include <vector>
#include <boost/bind.hpp>

#include <gtkmm/window.h>

#include "utils/debug.h"
#include "utils/boost.h"
#include "frame.h"
#include "application.h"



namespace framework {

	Frame::Frame()
		: m_window(new Gtk::Window()),
			m_glade(NULL)
	{
		connectSignals();
	}


	Frame::Frame(const std::string & gladeFile, const Glib::ustring & widgetName)
		: m_window(NULL),
			m_glade(Gnome::Glade::Xml::create(gladeFile))
	{
		if (m_glade) {
			m_window = static_cast<Gtk::Window*>(m_glade->get_widget(widgetName));
			connectSignals();
		}
	}


	void Frame::connectSignals()
	{
		m_window->signal_delete_event().connect(
			sigc::hide(sigc::mem_fun(this, &Frame::_close)));
		m_window->signal_hide().connect(
			sigc::retype_return<void>(sigc::mem_fun(this, &Frame::_close)));
	}

	Frame::~Frame()
	{
	}


	void Frame::set_icon_from_theme(const Glib::ustring & name)
	{
		using Glib::RefPtr;
		using Gtk::IconTheme;
		using std::vector;
		using std::list;
		
		RefPtr< IconTheme > icon_theme(Application::app()->getIconTheme());
		vector<int> icon_sizes(icon_theme->get_icon_sizes(name));
		
		list< RefPtr <Gdk::Pixbuf> > icons;

		std::for_each(icon_sizes.begin(), icon_sizes.end(),
					  // store the icon
					  boost::bind(&std::list< RefPtr<Gdk::Pixbuf> >::push_back, 
								  boost::ref(icons), 
								  // load the icon
								  boost::bind( &IconTheme::load_icon, 
											   boost::ref(icon_theme), 
											   boost::ref(name), _1, 
											   Gtk::ICON_LOOKUP_USE_BUILTIN)));
		gtkWindow().set_icon_list(icons);
	}

	void Frame::set_title(const std::string & title)
	{
		gtkWindow().set_title(Glib::ustring(title));
	}


	bool Frame::_close()
	{
		if(Controller::Ptr parent = m_parent.lock()) {
			parent->remove(shared_from_this());
		}		
		return false;
	}
}
