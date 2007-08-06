/*
 * niepce - ui/niepcewindow.h
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


#ifndef _UI_NIEPCEWINDOW_H_
#define _UI_NIEPCEWINDOW_H_

#include <gtkmm/treemodel.h>
#include <gtkmm/box.h>
#include <gtkmm/menubar.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/notebook.h>
#include <gtkmm/paned.h>
#include <gtkmm/buttonbox.h>

#include "framework/frame.h"

namespace Gtk {
	class TreeView;
	class IconView;
}

namespace ui {

	class ModelColumns : public Gtk::TreeModelColumnRecord
	{
	public:
		
		ModelColumns()
			{ add(m_col_text); add(m_col_number); }
		
		Gtk::TreeModelColumn<Glib::ustring> m_col_text;
		Gtk::TreeModelColumn<int> m_col_number;
	};

	class NiepceWindow
		: public framework::Frame
	{
	public:
		NiepceWindow();
		~NiepceWindow();

	private:

		void on_action_file_import();
		void on_action_file_quit();
		
		
		void init_ui();
		void init_actions();

		ModelColumns                   m_librarycolumns;
		Gtk::VBox                      m_vbox;
		Gtk::HPaned                    m_hbox;
		Gtk::VBox                      m_vbox2;
		Gtk::HButtonBox                m_mainbar;
		Gtk::TreeView*                 m_librarytree;
		Gtk::Notebook*                 m_mainview; // the main views stacked.
		Gtk::IconView*                 m_imageview;
		Gtk::Statusbar                 m_statusBar;
		Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;
	};

}


#endif
