/*
 * niepce - framework/metadatawidget.h
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

#ifndef __NIEPCE_FRAMEWORK_META_DATA_WIDGET_H__
#define __NIEPCE_FRAMEWORK_META_DATA_WIDGET_H__


#include <map>
#include <string>

#include <gtkmm/table.h>
#include <gtkmm/expander.h>

namespace framework {

class MetaDataWidget 
	: public Gtk::Expander
{
public:
	MetaDataWidget(const Glib::ustring & title);
	
	void add_data(const std::string & id, const Glib::ustring & label,
				  Gtk::Widget *w);
protected:
private:
	Gtk::Table    m_table;
	std::map<std::string, Gtk::Widget *> m_data_map;
};

}

#endif
