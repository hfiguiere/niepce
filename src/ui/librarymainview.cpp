/*
 * niepce - ui/librarymainview.h
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

#include <gtkmm/button.h>

#include "ui/librarymainview.h"

namespace ui {
	
	LibraryMainView::LibraryMainView()
		: Gtk::VBox()
	{
		m_mainbar.set_layout(Gtk::BUTTONBOX_START);
		m_notebook.set_show_tabs(false);
		pack_start(m_mainbar, Gtk::PACK_SHRINK);
		pack_start(m_notebook);
	}

	int
	LibraryMainView::append_page(Gtk::Widget & w, const Glib::ustring & label)
	{
		Gtk::Button *button = Gtk::manage(new Gtk::Button(label));
		m_mainbar.pack_start(*button);
		return m_notebook.append_page(w, label);
	}
	
}

