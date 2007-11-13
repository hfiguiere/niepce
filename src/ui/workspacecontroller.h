/*
 * niepce - ui/workspacecontroller.h
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



#ifndef __UI_WORKSPACECONTROLLER_H__
#define __UI_WORKSPACECONTROLLER_H__

#include <boost/array.hpp>

#include <glibmm/ustring.h>

#include <gtkmm/treeview.h>
#include <gtkmm/label.h>
#include <gtkmm/treestore.h>

#include "framework/controller.h"

namespace Gtk {
}



namespace ui {

	class WorkspaceController
		: public framework::Controller
	{
	public:
		WorkspaceController();
		class LibraryTreeColumns 
			: public Gtk::TreeModelColumnRecord
		{
		public:
			
			LibraryTreeColumns()
				{ 
					add(m_icon);
					add(m_id);
					add(m_label);  
				}
			Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf> > m_icon;
			Gtk::TreeModelColumn<int> m_id;
			Gtk::TreeModelColumn<Glib::ustring> m_label;
		};

	protected:
		virtual Gtk::Widget * buildWidget();

	private:
		void add_item(const Glib::RefPtr<Gtk::TreeStore> & treestore, 
					  const Glib::RefPtr<Gdk::Pixbuf> & icon,
					  const Glib::ustring & label, int id) const;
		enum {
			ICON_FOLDER = 0,
			ICON_PROJECT,
			_ICON_SIZE
		};
		boost::array< Glib::RefPtr<Gdk::Pixbuf>, _ICON_SIZE > m_icons;
		LibraryTreeColumns             m_librarycolumns;
		Gtk::VBox                      m_vbox;
		Gtk::Label                     m_label;
		Gtk::TreeView                  m_librarytree;
	};


}

#endif
