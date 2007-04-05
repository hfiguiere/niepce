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
#include <gtkmm/treeview.h>
#include <gtkmm/liststore.h>
#include <gtkmm/box.h>
#include <gtkmm/menubar.h>

#include "framework/frame.h"


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

	private:
		Gtk::TreeView m_librarytree;
		ModelColumns m_librarycolumns;
		Gtk::VBox m_vbox;
		Gtk::MenuBar m_menuBar;
	};

}


#endif
