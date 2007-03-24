/*
 * niepce - framework/application.cpp
 *
 * Copyright (C) 2007 Hubert Figuiere
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  
 * 02110-1301, USA
 */

#include <boost/scoped_ptr.hpp>

#include <gtkmm.h>

#include "application.h"
#include "frame.h"

namespace framework {

	Application *Application::m_application = NULL; 

	Application::Application()
	{
	}


	Application *Application::instance()
	{
		if (m_application == NULL) {
			m_application = new Application();
		}
		return m_application;
	}


	int Application::main(int argc, char **argv)
	{
		Application * app = instance();

		Gtk::Main kit(argc, argv);

    boost::scoped_ptr<Frame> window(app->makeMainFrame());		

    Gtk::Main::run(window->gtkWindow());

		return 0;
	}

	Frame *Application::makeMainFrame()
	{
		return new Frame;
	}

}

