/*
 * niepce - framework/application.h
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


#ifndef _FRAMEWORK_APPLICATION_H_
#define _FRAMEWORK_APPLICATION_H_

#include <boost/function.hpp>
#include <glibmm/refptr.h>
#include <gtkmm/uimanager.h>
#include "configuration.h"


namespace framework {

	class Frame;

	class Application 
	{
	public:
		virtual ~Application();

		virtual Frame *makeMainFrame();

		Configuration & config()
			{ return m_config; }
		Glib::RefPtr<Gtk::UIManager> uiManager()
			{ 
				if(!m_refUIManager) {
					m_refUIManager = Gtk::UIManager::create();
				}
				return m_refUIManager; 
			}

		void quit();

		static Application *app();
		static int main(boost::function<Application* (void)> constructor, 
										int argc, char **argv);

		
	protected:
		Application();
		static Application *m_application; 

	private:
		Configuration                m_config;
		Glib::RefPtr<Gtk::UIManager> m_refUIManager;
	};

}

#endif
