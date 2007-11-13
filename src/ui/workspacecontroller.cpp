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

#include <gtkmm/treeview.h>
#include <gtkmm/icontheme.h>
#include <gtkmm/treestore.h>

#include "workspacecontroller.h"


namespace ui {

	Gtk::Widget * WorkspaceController::buildWidget()
	{
		Glib::RefPtr<Gtk::TreeStore> treestore = Gtk::TreeStore::create(m_librarycolumns);
		m_librarytree = Gtk::manage(new Gtk::TreeView());
		m_librarytree->set_model(treestore);
		Gtk::TreeModel::iterator iter = treestore->append();
		Gtk::TreeModel::Row row = *iter;
		Glib::RefPtr< Gtk::IconTheme > icon_theme(Gtk::IconTheme::get_default());
		row[m_librarycolumns.m_icon] = icon_theme->load_icon(
			Glib::ustring("folder"), 16, Gtk::ICON_LOOKUP_USE_BUILTIN);
		row[m_librarycolumns.m_label] = _("Pictures");
		row[m_librarycolumns.m_id] = 0;
		iter = treestore->append();
		row = *iter;
		row[m_librarycolumns.m_icon] = icon_theme->load_icon(
			Glib::ustring("applications-accessories"), 16, 
			Gtk::ICON_LOOKUP_USE_BUILTIN);
		row[m_librarycolumns.m_label] = _("Projects");
		row[m_librarycolumns.m_id] = 0;

		m_librarytree->set_headers_visible(true);
		m_librarytree->append_column("", m_librarycolumns.m_icon);
		m_librarytree->append_column(_("Workspace"), m_librarycolumns.m_label);
		return m_librarytree;
	}
	

}
