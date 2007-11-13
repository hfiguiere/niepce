/*
 * niepce - ui/workspacecontroller.cpp
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

#include <glibmm/i18n.h>

#include <gtkmm/icontheme.h>
#include <gtkmm/box.h>

#include "workspacecontroller.h"


namespace ui {

	WorkspaceController::WorkspaceController()
		: framework::Controller()
	{
		Glib::RefPtr< Gtk::IconTheme > icon_theme(Gtk::IconTheme::get_default());
		m_icons[ICON_FOLDER] = icon_theme->load_icon(
			Glib::ustring("folder"), 16, Gtk::ICON_LOOKUP_USE_BUILTIN);
		m_icons[ICON_PROJECT] = icon_theme->load_icon(
			Glib::ustring("applications-accessories"), 16, 
			Gtk::ICON_LOOKUP_USE_BUILTIN);
	}


	void WorkspaceController::add_item(const Glib::RefPtr<Gtk::TreeStore> & treestore, 
									   const Glib::RefPtr<Gdk::Pixbuf> & icon,
									   const Glib::ustring & label, int id) const
	{
		Gtk::TreeModel::iterator iter;
		Gtk::TreeModel::Row row;
		iter = treestore->append();
		row = *iter;
		row[m_librarycolumns.m_icon] = icon;
		row[m_librarycolumns.m_label] = label; 
		row[m_librarycolumns.m_id] = id;
	}


	Gtk::Widget * WorkspaceController::buildWidget()
	{
		Glib::RefPtr<Gtk::TreeStore> treestore = Gtk::TreeStore::create(m_librarycolumns);
		m_librarytree.set_model(treestore);

		add_item(treestore, m_icons[ICON_FOLDER], 
				 Glib::ustring(_("Pictures")), 0);
		add_item(treestore, m_icons[ICON_PROJECT], 
				 Glib::ustring(_("Projects")), 0);

		m_librarytree.set_headers_visible(false);
		m_librarytree.append_column("", m_librarycolumns.m_icon);
		m_librarytree.append_column("", m_librarycolumns.m_label);

		// TODO make it a mnemonic
		m_label.set_text_with_mnemonic(Glib::ustring(_("_Workspace")));
		m_label.set_mnemonic_widget(m_librarytree);
		m_vbox.pack_start(m_label, Gtk::PACK_SHRINK);
		m_vbox.pack_start(m_librarytree);
		return &m_vbox;
	}
	

}
