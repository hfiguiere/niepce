/*
 * niepce - ui/librarymainviewcontroller.cpp
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
#include <glibmm/ustring.h>

#include <gtkmm/icontheme.h>

#include <gtkimageview/gtkimageview.h>
#include <gtkimageview/gtkimagescrollwin.h>

#include "utils/debug.h"
#include "niepce/notifications.h"
#include "db/library.h"
#include "framework/application.h"
#include "librarymainviewcontroller.h"

namespace ui {

	void LibraryMainViewController::on_lib_notification(const framework::Notification::Ptr &n)
	{
		DBG_OUT("notification");
		if(n->type() == niepce::NOTIFICATION_LIB) {
			db::LibNotification ln = boost::any_cast<db::LibNotification>(n->data());
			switch(ln.type) {
			case db::Library::NOTIFY_FOLDER_CONTENT_QUERIED:
			{
				db::LibFile::ListPtr l 
					= boost::any_cast<db::LibFile::ListPtr>(ln.param);
				DBG_OUT("received folder content file # %d", l->size());
				Glib::RefPtr< Gtk::IconTheme > icon_theme(framework::Application::app()->getIconTheme());
				m_model->clear();
				db::LibFile::List::const_iterator iter = l->begin();
				for( ; iter != l->end(); iter++ )
				{
					Gtk::TreeModel::iterator riter = m_model->append();
					Gtk::TreeRow row = *riter;
					row[m_columns.m_pix] = icon_theme->load_icon(
						Glib::ustring("image-loading"), 32,
						Gtk::ICON_LOOKUP_USE_BUILTIN);
					row[m_columns.m_name] = Glib::ustring((*iter)->name());
					row[m_columns.m_libfile] = *iter;
				}
				break;
			}
			default:
				break;
			}
		}
	}


	Gtk::Widget * LibraryMainViewController::buildWidget()
	{
		m_model = Gtk::ListStore::create(m_columns);
		m_librarylistview.set_pixbuf_column(m_columns.m_pix);
		m_librarylistview.set_markup_column(m_columns.m_name);
		m_librarylistview.set_model(m_model);
		m_mainview.append_page(m_librarylistview, _("Library"));
		
		GtkWidget *iv = gtk_image_view_new();
		GtkWidget *ivs = gtk_image_scroll_win_new(GTK_IMAGE_VIEW(iv));
		m_imageview = Gtk::manage(Glib::wrap(ivs));
		m_mainview.append_page(*m_imageview, _("Darkroom"));
		return &m_mainview;
	}

	void LibraryMainViewController::on_ready()
	{
	}

}
