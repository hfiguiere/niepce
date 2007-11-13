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


#include "framework/controller.h"

namespace Gtk {
	class TreeView;
}



namespace ui {

	class WorkspaceController
		: public framework::Controller
	{
	public:
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
		LibraryTreeColumns             m_librarycolumns;
		Gtk::TreeView*                 m_librarytree;
	};


}

#endif
