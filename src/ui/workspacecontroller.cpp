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

#include <boost/any.hpp>
#include <boost/bind.hpp>

#include <glibmm/i18n.h>

#include <gtkmm/icontheme.h>
#include <gtkmm/box.h>

#include "utils/debug.h"
#include "niepce/notifications.h"
#include "db/library.h" // FIXME uh oh. this shouldn't be
#include "libraryclient/libraryclient.h"
#include "framework/application.h"
#include "niepcewindow.h"
#include "workspacecontroller.h"


using framework::Application;

namespace ui {

	WorkspaceController::WorkspaceController()
		: framework::Controller()
	{
		Glib::RefPtr< Gtk::IconTheme > icon_theme(Application::app()->getIconTheme());
		m_icons[ICON_FOLDER] = icon_theme->load_icon(
			Glib::ustring("folder"), 16, Gtk::ICON_LOOKUP_USE_BUILTIN);
		m_icons[ICON_PROJECT] = icon_theme->load_icon(
			Glib::ustring("applications-accessories"), 16, 
			Gtk::ICON_LOOKUP_USE_BUILTIN);
		m_icons[ICON_ROLL] = icon_theme->load_icon(
			Glib::ustring("emblem-photos"), 16,
			Gtk::ICON_LOOKUP_USE_BUILTIN);
		// FIXME use an icon that make more sense.
		m_icons[ICON_KEYWORD] = icon_theme->load_icon(
			Glib::ustring("application-certificate"), 16, 
			Gtk::ICON_LOOKUP_USE_BUILTIN);
	}

	libraryclient::LibraryClient::Ptr WorkspaceController::getLibraryClient()
	{
		return	boost::dynamic_pointer_cast<NiepceWindow>(m_parent.lock())->getLibraryClient();
	}


	void WorkspaceController::on_lib_notification(const framework::Notification::Ptr &n)
	{
		DBG_OUT("notification for workspace");
		if(n->type() == niepce::NOTIFICATION_LIB) {
			db::LibNotification ln = boost::any_cast<db::LibNotification>(n->data());
			switch(ln.type) {
			case db::Library::NOTIFY_ADDED_FOLDERS:
			{
				db::LibFolder::ListPtr l 
					= boost::any_cast<db::LibFolder::ListPtr>(ln.param);
				DBG_OUT("received added folders # %d", l->size());
				for_each(l->begin(), l->end(), 
						 boost::bind(&WorkspaceController::add_folder_item, 
									 this, _1));
				break;
			}
			case db::Library::NOTIFY_ADDED_KEYWORD:
			{
				db::Keyword::Ptr k
					= boost::any_cast<db::Keyword::Ptr>(ln.param);
				DBG_ASSERT(k, "keyword must not be NULL");
				add_keyword_item(k);
				break;
			}
			case db::Library::NOTIFY_ADDED_KEYWORDS:
			{
				db::Keyword::ListPtr l
					= boost::any_cast<db::Keyword::ListPtr>(ln.param);
				DBG_ASSERT(l, "keyword list must not be NULL");
				for_each(l->begin(), l->end(), 
						 boost::bind(&WorkspaceController::add_keyword_item, 
									 this, _1));
				break;
			}
			default:
				break;
			}
		}
	}


	void WorkspaceController::on_libtree_selection()
	{
		Glib::RefPtr<Gtk::TreeSelection> selection = m_librarytree.get_selection();
		Gtk::TreeModel::iterator selected = selection->get_selected();
		if((*selected)[m_librarycolumns.m_type] == FOLDER_ITEM)
		{
			int id = (*selected)[m_librarycolumns.m_id];
			getLibraryClient()->queryFolderContent(id);
		}
		else if((*selected)[m_librarycolumns.m_type] == KEYWORD_ITEM)
		{
			int id = (*selected)[m_librarycolumns.m_id];
			getLibraryClient()->queryKeywordContent(id);			
		}
		else 
		{
			DBG_OUT("selected something not a folder");
		}
	}

	void WorkspaceController::add_keyword_item(const db::Keyword::Ptr & k)
	{
		add_item(m_treestore, m_keywordsNode->children(), 
				 m_icons[ICON_KEYWORD], k->keyword(), k->id(), KEYWORD_ITEM);
	}

	void WorkspaceController::add_folder_item(const db::LibFolder::Ptr & f)
	{
		add_item(m_treestore, m_folderNode->children(), m_icons[ICON_ROLL], 
				 f->name(), f->id(), FOLDER_ITEM);
	}

	Gtk::TreeModel::iterator
	WorkspaceController::add_item(const Glib::RefPtr<Gtk::TreeStore> &treestore,
								  const Gtk::TreeNodeChildren & childrens,
								  const Glib::RefPtr<Gdk::Pixbuf> & icon,
								  const Glib::ustring & label, int id,
								  int type) const
	{
		Gtk::TreeModel::iterator iter;
		Gtk::TreeModel::Row row;
		iter = treestore->append(childrens);
		row = *iter;
		row[m_librarycolumns.m_icon] = icon;
		row[m_librarycolumns.m_label] = label; 
		row[m_librarycolumns.m_id] = id;
		row[m_librarycolumns.m_type] = type;
		return iter;
	}


	Gtk::Widget * WorkspaceController::buildWidget()
	{
		m_treestore = Gtk::TreeStore::create(m_librarycolumns);
		m_librarytree.set_model(m_treestore);

		m_folderNode = add_item(m_treestore, m_treestore->children(),
								m_icons[ICON_FOLDER], 
								Glib::ustring(_("Pictures")), 0,
								FOLDERS_ITEM);
		m_projectNode = add_item(m_treestore, m_treestore->children(),
								 m_icons[ICON_PROJECT], 
								 Glib::ustring(_("Projects")), 0,
								 PROJECTS_ITEM);
		m_keywordsNode = add_item(m_treestore, m_treestore->children(),
								  m_icons[ICON_KEYWORD],
								  Glib::ustring(_("Keywords")), 0,
								  KEYWORDS_ITEM);

		m_librarytree.set_headers_visible(false);
		m_librarytree.append_column("", m_librarycolumns.m_icon);
		m_librarytree.append_column("", m_librarycolumns.m_label);

		// TODO make it a mnemonic
		m_label.set_text_with_mnemonic(Glib::ustring(_("_Workspace")));
		m_label.set_mnemonic_widget(m_librarytree);
		m_vbox.pack_start(m_label, Gtk::PACK_SHRINK);
		m_vbox.pack_start(m_librarytree);

		m_librarytree.get_selection()->signal_changed().connect (
			sigc::mem_fun(*this, 
						  &WorkspaceController::on_libtree_selection));

		return &m_vbox;
	}
	
	void WorkspaceController::on_ready()
	{
		getLibraryClient()->getAllFolders();
		getLibraryClient()->getAllKeywords();
	}

}
