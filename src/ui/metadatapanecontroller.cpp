/*
 * niepce - ui/metadatapanecontroller.cpp
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

#include <glibmm/i18n.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>

#include "utils/debug.h"
#include "framework/metadatawidget.h"
#include "metadatapanecontroller.h"


namespace ui {

	
	Gtk::Widget * MetaDataPaneController::buildWidget()
	{
		m_metadataw = Gtk::manage(new framework::MetaDataWidget(_("Exif")));
		m_metapane.pack_end(*m_metadataw, Gtk::PACK_EXPAND_WIDGET, 0);
		m_widget = &m_metapane;

		// TODO test, remove
#if 0
		Gtk::Label *label = Gtk::manage(new Gtk::Label("Data"));
		w->add_data("foo", "Foo:", label);
		label->set_justify(Gtk::JUSTIFY_LEFT);
		label->property_xalign() = 0;
		Gtk::Entry *entry = Gtk::manage(new Gtk::Entry());
		entry->set_text("this is a text");
		w->add_data("bar", "Bar:", entry);
#endif

		return &m_metapane;
	}


	void MetaDataPaneController::display(const utils::XmpMeta & meta)
	{
		DBG_OUT("displaying metadata");
		m_metadataw->set_data_source(meta);		
	}

}
