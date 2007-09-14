/*
 * niepce - ui/niepcewindow.h
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


#ifndef _UI_NIEPCEWINDOW_H_
#define _UI_NIEPCEWINDOW_H_

#include <gtkmm/treemodel.h>
#include <gtkmm/box.h>
#include <gtkmm/menubar.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/paned.h>

#include "framework/frame.h"
#include "libraryclient/libraryclient.h"
#include "ui/librarymainviewcontroller.h"

namespace Gtk {
	class TreeView;
	class IconView;
}

namespace ui {

	class NiepceWindow
		: public framework::Frame
	{
	public:
		NiepceWindow();
		~NiepceWindow();

	protected:
		virtual Gtk::Widget * buildWidget();

	private:

		void on_action_file_import();
		void on_action_file_quit();
		void on_open_library();
		
		void init_ui();
		void init_actions();

		void open_library(const std::string & libMoniker);
		
		class LibraryTreeColumns 
			: public Gtk::TreeModelColumnRecord
		{
		public:
			
			LibraryTreeColumns()
				{ 
					add(m_id);
					add(m_label);  
				}
			
			Gtk::TreeModelColumn<int> m_id;
			Gtk::TreeModelColumn<Glib::ustring> m_label;
		};

		LibraryTreeColumns             m_librarycolumns;
		Gtk::VBox                      m_vbox;
		Gtk::HPaned                    m_hbox;
		Gtk::TreeView*                 m_librarytree;
		LibraryMainViewController::Ptr m_mainviewctrl; // the main views stacked.
		Gtk::Statusbar                 m_statusBar;
		Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;
		libraryclient::LibraryClient::Ptr m_libClient;
	};

}


#endif
