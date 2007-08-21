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

#include <boost/scoped_ptr.hpp>
#include <boost/function.hpp>

#include <gtkmm/main.h>

#include "application.h"
#include "frame.h"
#include "libraryclient/libraryclient.h"

using libraryclient::LibraryClient;

namespace framework {

	Application *Application::m_application = NULL; 

	Application::Application()
		: m_refUIManager()
	{
	}


	Application::~Application()
	{
	}


	Application *Application::app()
	{
		if (m_application == NULL) {
			m_application = new Application();
		}
		return m_application;
	}


	/** Main loop. 
	 * @param constructor the Application object constructor
	 * @param argc
	 * @param argv
	 * @return main return code
	 */
	int Application::main(boost::function<Application* (void)> constructor, 
												int argc, char **argv)
	{
		Gtk::Main kit(argc, argv);
		Application * app = constructor();

		LibraryClient::Ptr library(new LibraryClient("local:.dir"));

    Frame *window(app->makeMainFrame());		

    Gtk::Main::run(window->gtkWindow());

		return 0;
	}

	Frame *Application::makeMainFrame()
	{
		return new Frame;
	}

	void Application::quit()
	{
		Gtk::Main::quit();
	}
}

