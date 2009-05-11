/*
 * niepce - ui/workspacecontroller.h
 *
 * Copyright (C) 2007-2008 Hubert Figuiere
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



#ifndef __UI_WORKSPACECONTROLLER_H__
#define __UI_WORKSPACECONTROLLER_H__

#include <tr1/array>

#include <glibmm/ustring.h>

#include <gtkmm/treeview.h>
#include <gtkmm/label.h>
#include <gtkmm/treestore.h>

#include "engine/db/libfolder.hpp"
#include "fwk/toolkit/controller.hpp"
#include "fwk/toolkit/notification.hpp"

namespace Gtk {
}


namespace ui {

	class WorkspaceController
		: public fwk::Controller
	{
	public:
		typedef std::tr1::shared_ptr<WorkspaceController> Ptr;

		enum {
			FOLDERS_ITEM,
			PROJECTS_ITEM,
			KEYWORDS_ITEM,
			FOLDER_ITEM,
			PROJECT_ITEM,
			KEYWORD_ITEM
		};

		WorkspaceController();
		class WorkspaceTreeColumns 
			: public Gtk::TreeModelColumnRecord
		{
		public:
			
			WorkspaceTreeColumns()
				{ 
					add(m_icon);
					add(m_id);
					add(m_label);  
					add(m_type);
					add(m_count);
				}
			Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf> > m_icon;
			Gtk::TreeModelColumn<int> m_id;
			Gtk::TreeModelColumn<Glib::ustring> m_label;
			Gtk::TreeModelColumn<int> m_type;
			Gtk::TreeModelColumn<Glib::ustring> m_count;
		};

		virtual void on_ready();

		void on_lib_notification(const eng::LibNotification &);
		void on_count_notification(int);
		void on_libtree_selection();

	protected:
		virtual Gtk::Widget * buildWidget();

	private:
		libraryclient::LibraryClient::Ptr getLibraryClient();

		/** add a folder item to the treeview */
		void add_folder_item(const eng::LibFolder::Ptr & f);
		/** add a keyword item to the treeview */
		void add_keyword_item(const eng::Keyword::Ptr & k);
		/** add a tree item in the treeview 
		 * @param treestore the treestore to add to
		 * @param childrens the children subtree to add to
		 * @param icon the icon for the item
		 * @param label the item label
		 * @param id the item id (in the database)
		 * @paran type the type of node
		 */
		Gtk::TreeModel::iterator add_item(const Glib::RefPtr<Gtk::TreeStore> & treestore, 
										  const Gtk::TreeNodeChildren & childrens,
										  const Glib::RefPtr<Gdk::Pixbuf> & icon,
										  const Glib::ustring & label, 
										  int id, int type) const;
		enum {
			ICON_FOLDER = 0,
			ICON_PROJECT,
			ICON_ROLL,
			ICON_KEYWORD,
			_ICON_SIZE
		};
		std::tr1::array< Glib::RefPtr<Gdk::Pixbuf>, _ICON_SIZE > m_icons;
		WorkspaceTreeColumns           m_librarycolumns;
		Gtk::VBox                      m_vbox;
		Gtk::Label                     m_label;
		Gtk::TreeView                  m_librarytree;
		Gtk::TreeModel::iterator       m_folderNode;  /**< the folder node */
		Gtk::TreeModel::iterator       m_projectNode; /**< the project node */
		Gtk::TreeModel::iterator       m_keywordsNode; /**< the keywords node */
		Glib::RefPtr<Gtk::TreeStore>   m_treestore;   /**< the treestore */
		std::map<int, Gtk::TreeIter>   m_folderidmap;
		std::map<int, Gtk::TreeIter>   m_projectidmap;
		std::map<int, Gtk::TreeIter>   m_keywordsidmap;
	};


}

#endif
