/*
 * niepce - ui/librarymainview.cpp
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

#include <gtkmm/togglebutton.h>

#include "utils/debug.h"
#include "ui/librarymainview.h"

namespace ui {

	LibraryMainView::LibraryMainView()
		: Gtk::VBox(),
		  m_currentpage(-1)
	{
		set_spacing(4);
		m_mainbar.set_layout(Gtk::BUTTONBOX_START);
		m_mainbar.set_spacing(4);
		m_notebook.set_show_tabs(false);
		pack_start(m_mainbar, Gtk::PACK_SHRINK);
		pack_start(m_notebook);
	}

	int
	LibraryMainView::append_page(Gtk::Widget & w, const Glib::ustring & label)
	{
		int idx;
		
		Gtk::ToggleButton* button = Gtk::manage(new Gtk::ToggleButton(label));
		m_mainbar.pack_start(*button);
		idx = m_notebook.append_page(w, label);
		sigc::connection conn = button->signal_toggled().connect(
			sigc::bind(sigc::mem_fun(this, &LibraryMainView::set_current_page),
					   idx, button));
		if(m_currentpage == -1) {
			set_current_page(idx, button);
		}
		if((int)m_buttons.size() < idx + 1) {
			m_buttons.resize(idx + 1);
		}
		m_buttons[idx] = std::make_pair(button, conn);
		return idx;
	}
	
	void LibraryMainView::activate_page(int idx)
	{
		if(m_currentpage != idx) {
			Gtk::ToggleButton * btn = m_buttons[idx].first;
			set_current_page(idx, btn);
		}
	}


	void LibraryMainView::set_current_page(int idx, Gtk::ToggleButton * btn)
	{
		m_notebook.set_current_page(idx);
		if(m_currentpage >= 0) {
			m_buttons[m_currentpage].second.block();
			m_buttons[m_currentpage].first->set_active(false);
			m_buttons[m_currentpage].second.unblock();
		}
		btn->set_active(true);
		m_currentpage = idx;
	}
}

