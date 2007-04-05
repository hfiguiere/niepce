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

#include <gtkmm/window.h>

#include "niepcewindow.h"


namespace ui {


	NiepceWindow::NiepceWindow()
		: framework::Frame()//GLADEDIR "mainwindow.glade", "mainwindow")
	{
//		Glib::RefPtr<Gnome::Glade::Xml> & g(glade());
		Gtk::Window & win(gtkWindow());

		win.add(m_vbox);
		m_vbox.pack_start(m_menuBar, Gtk::PACK_SHRINK);
		
		Glib::RefPtr<Gtk::ListStore> liststore = Gtk::ListStore::create(m_librarycolumns);
		m_librarytree.set_model(liststore);
		Gtk::TreeModel::iterator iter = liststore->append();
		Gtk::TreeModel::Row row = *iter;
		row[m_librarycolumns.m_col_text] = "foo";

		m_vbox.pack_start(m_librarytree);

		win.set_size_request(600, 400);
		win.show_all_children();
	}
}
