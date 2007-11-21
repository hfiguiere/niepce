/*
 * niepce - ui/librarymainviewcontroller.h
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


#ifndef __UI_LIBRARYMAINVIEWCONTROLLER_H__
#define __UI_LIBRARYMAINVIEWCONTROLLER_H__


#include <gtkmm/iconview.h>
#include <gtkmm/liststore.h>

#include "librarymainview.h"
#include "db/libfile.h"
#include "framework/controller.h"
#include "framework/notification.h"

namespace Gtk {
	class Widget;
}


namespace ui {

	class LibraryMainViewController
		: public framework::Controller
	{
	public:
		typedef boost::shared_ptr<LibraryMainViewController> Ptr;
		typedef boost::weak_ptr<LibraryMainViewController> WeakPtr;


		class LibraryListColumns 
			: public Gtk::TreeModelColumnRecord
		{
		public:
			
			LibraryListColumns()
				{ 
					add(m_pix);
					add(m_name);
					add(m_libfile);
				}
			Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf> > m_pix;
			Gtk::TreeModelColumn<Glib::ustring> m_name;
			Gtk::TreeModelColumn<db::LibFile::Ptr> m_libfile;
		};


		void on_lib_notification(const framework::Notification::Ptr &);
		
	protected:
		virtual Gtk::Widget * buildWidget();
		virtual void on_ready();
	private:

		// managed widgets...
		LibraryMainView              m_mainview;
		Gtk::IconView                m_librarylistview;
		LibraryListColumns           m_columns;
		Glib::RefPtr<Gtk::ListStore> m_model;
		Gtk::Widget*                 m_imageview;
	};

}

#endif
