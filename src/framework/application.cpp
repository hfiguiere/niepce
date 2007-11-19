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
#include <boost/bind.hpp>

#include <gtkmm/main.h>
#include <gtkmm/aboutdialog.h>

#include "application.h"
#include "frame.h"


namespace framework {

	Application::Ptr Application::m_application;

	Application::Application()
		: m_refUIManager()
	{
	}


	Application::~Application()
	{
	}

	/** no widget for applications */
	Gtk::Widget * Application::buildWidget()
	{
		return NULL;
	}

	Application::Ptr Application::app()
	{
		return m_application;
	}

	Glib::RefPtr<Gtk::IconTheme> Application::getIconTheme() const
	{
		return Gtk::IconTheme::get_default();
	}

	/** Main loop. 
	 * @param constructor the Application object constructor
	 * @param argc
	 * @param argv
	 * @return main return code
	 */
	int Application::main(boost::function<Application::Ptr (void)> constructor, 
												int argc, char **argv)
	{
		Gnome::Conf::init();
		Gtk::Main kit(argc, argv);
		Application::Ptr app = constructor();

		kit.signal_run().connect(sigc::mem_fun(get_pointer(app), 
											   &Application::_ready));
		Frame::Ptr window(app->makeMainFrame());
		app->add(window);
		
		Gtk::Main::run(window->gtkWindow());
	
		return 0;
	}


	void Application::terminate()
	{
		std::for_each(m_subs.begin(), m_subs.end(),
									boost::bind(&Controller::terminate, _1));
		std::for_each(m_subs.begin(), m_subs.end(),
									boost::bind(&Controller::clearParent, _1));		
		m_subs.clear();
	}


	void Application::quit()
	{
		terminate();
		Gtk::Main::quit();
	}

	void Application::about()
	{
		on_about();
	}

	/** adding a controller to an application build said controller
	 * widget 
	 */
	void Application::add(const Controller::Ptr & sub)
	{
		Controller::add(sub);
		sub->buildWidget();
	}

	void Application::on_about()
	{
		Gtk::AboutDialog dlg;
		dlg.run();
	}

}

