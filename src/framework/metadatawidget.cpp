/*
 * niepce - framework/metadatawidget.cpp
 *
 * Copyright (C) 2008 Hubert Figuiere
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


#include <utility>

#include <gtkmm/label.h>

#include "metadatawidget.h"



namespace framework {

	MetaDataWidget::MetaDataWidget(const Glib::ustring & title)
		: Gtk::Expander(title),
		  m_table(1, 2, false)
	{
		add(m_table);
	}


	void MetaDataWidget::add_data(const std::string & id, const Glib::ustring & label,
								 Gtk::Widget *w)
	{
		Gtk::Label *labelw = Gtk::manage(new Gtk::Label(Glib::ustring("<b>") 
														+ label + "</b>"));
		labelw->set_alignment(0, 0.5);
		labelw->set_use_markup(true);
		int n_row;
		if(m_data_map.empty()) {
			n_row = 0;
		}
		else {
			n_row = m_table.property_n_rows();
			m_table.resize(n_row + 1, 2);
		}
		m_table.attach(*labelw, 0, 1, n_row, n_row+1, Gtk::FILL, Gtk::SHRINK, 4, 0);
		m_table.attach(*w, 1, 2, n_row, n_row+1, Gtk::EXPAND|Gtk::FILL, Gtk::SHRINK, 4, 0);
		m_data_map.insert(std::make_pair(id, w));
	}

}
