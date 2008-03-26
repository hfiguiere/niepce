/*
 * niepce - ui/librarymainviewcontroller.cpp
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


#include <glibmm/i18n.h>
#include <glibmm/ustring.h>

#include <gtkmm/icontheme.h>

#include <gtkimageview/gtkimageview.h>
#include <gtkimageview/gtkimagescrollwin.h>

#include "utils/debug.h"
#include "niepce/notifications.h"
#include "db/library.h"
#include "library/thumbnailnotification.h"
#include "framework/application.h"
#include "librarymainviewcontroller.h"
#include "niepcewindow.h"
#include "metadatapanecontroller.h"

namespace ui {

	void LibraryMainViewController::on_lib_notification(const framework::Notification::Ptr &n)
	{
		DBG_ASSERT(n->type() == niepce::NOTIFICATION_LIB, 
				   "wrong notification type");
		if(n->type() == niepce::NOTIFICATION_LIB) {
			db::LibNotification ln = boost::any_cast<db::LibNotification>(n->data());
			switch(ln.type) {
			case db::Library::NOTIFY_FOLDER_CONTENT_QUERIED:
			case db::Library::NOTIFY_KEYWORD_CONTENT_QUERIED:
			{
				db::LibFile::ListPtr l 
					= boost::any_cast<db::LibFile::ListPtr>(ln.param);
				DBG_OUT("received folder content file # %d", l->size());
				Glib::RefPtr< Gtk::IconTheme > icon_theme(framework::Application::app()->getIconTheme());
				m_model->clear();
				m_idmap.clear();
				db::LibFile::List::const_iterator iter = l->begin();
				for( ; iter != l->end(); iter++ )
				{
					Gtk::TreeModel::iterator riter = m_model->append();
					Gtk::TreeRow row = *riter;
					// locate it in local cache...
					row[m_columns.m_pix] = icon_theme->load_icon(
						Glib::ustring("image-loading"), 32,
						Gtk::ICON_LOOKUP_USE_BUILTIN);
					row[m_columns.m_name] = Glib::ustring((*iter)->name());
					row[m_columns.m_libfile] = *iter;
					m_idmap[(*iter)->id()] = riter;
				}
				// at that point clear the cache because the icon view is populated.
				getLibraryClient()->thumbnailCache().request(l);
				break;
			}
			default:
				break;
			}
		}
	}

	void LibraryMainViewController::on_tnail_notification(const framework::Notification::Ptr &n)
	{
		DBG_ASSERT(n->type() == niepce::NOTIFICATION_THUMBNAIL, 
				   "wrong notification type");
		if(n->type() == niepce::NOTIFICATION_THUMBNAIL) {
			library::ThumbnailNotification tn 
				= boost::any_cast<library::ThumbnailNotification>(n->data());
			std::map<int, Gtk::TreeIter>::iterator iter
				= m_idmap.find( tn.id );
			if(iter != m_idmap.end()) {
				// found the icon view item
				Gtk::TreeRow row = *(iter->second);
				row[m_columns.m_pix] = tn.pixmap;
			}
			else {
				DBG_OUT("row %d not found", tn.id);
			}
		}
	}


	Gtk::Widget * LibraryMainViewController::buildWidget()
	{
		m_model = Gtk::ListStore::create(m_columns);
		m_librarylistview.set_pixbuf_column(m_columns.m_pix);
		m_librarylistview.set_markup_column(m_columns.m_name);
		m_librarylistview.set_model(m_model);
		m_librarylistview.set_selection_mode(Gtk::SELECTION_SINGLE);
		m_scrollview.add(m_librarylistview);
		m_scrollview.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
		m_lib_splitview.pack1(m_scrollview);
		m_lib_splitview.pack2(m_lib_metapanescroll);
		m_metapanecontroller = MetaDataPaneController::Ptr(new MetaDataPaneController());
		m_lib_metapanescroll.add(*m_metapanecontroller->widget());
		m_lib_metapanescroll.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);

		m_databinders.add_binder(new framework::ConfigDataBinder<int>(
									 m_lib_splitview.property_position(),
									 framework::Application::app()->config(),
									 "meta_pane_splitter"));
		
		m_mainview.append_page(m_lib_splitview, _("Library"));

//		m_librarylistview.signal_selection_changed()
//			.connect(sigc::mem_fun(*m_metapanecontroller,
//								   &MetaDataPaneController::on_selection_changed));
		
		// TODO DarkroomModuleController
		GtkWidget *iv = gtk_image_view_new();
		GtkWidget *ivs = gtk_image_scroll_win_new(GTK_IMAGE_VIEW(iv));
		m_imageview = Gtk::manage(Glib::wrap(ivs));
		m_dr_splitview.pack1(*m_imageview, Gtk::EXPAND);
		m_mainview.append_page(m_dr_splitview, _("Darkroom"));

		// TODO PrintModuleController
		// m_mainview.append_page(, _("Print"));
		return &m_mainview;
	}


	void LibraryMainViewController::on_ready()
	{
	}


	void LibraryMainViewController::on_selected(int id)
	{
		DBG_OUT("selected callback %d", id);
		std::map<int, Gtk::TreeIter>::iterator iter
			= m_idmap.find( id );
		if(iter != m_idmap.end()) {
			db::LibFile::Ptr libfile = (*iter->second)[m_columns.m_libfile];

			// FIXME get the XMP a different way as it is in the DB
			// 
			utils::XmpMeta meta(libfile->path(), false);
			if(meta.isOk()) {
				m_metapanecontroller->display(meta);
			}
		}		
	}


	libraryclient::LibraryClient::Ptr LibraryMainViewController::getLibraryClient()
	{
		return	boost::dynamic_pointer_cast<NiepceWindow>(m_parent.lock())->getLibraryClient();
	}


	Gtk::IconView * LibraryMainViewController::image_list()
	{ 
		return & m_librarylistview; 
	}

	int LibraryMainViewController::get_selected()
	{
		int id = 0;
		Glib::RefPtr<Gtk::TreeSelection> selection;

		Gtk::IconView::ArrayHandle_TreePaths paths = m_librarylistview.get_selected_items();
		if(!paths.empty()) {
			Gtk::TreePath path(*(paths.begin()));
			DBG_OUT("found path %s", path.to_string().c_str());
			Gtk::TreeRow row = *(m_model->get_iter(path));
			if(row) {
				DBG_OUT("found row");
				db::LibFile::Ptr libfile = row[m_columns.m_libfile];
				if(libfile) {
					id = libfile->id();
				}
			}
		}
		DBG_OUT("get_selected %d", id);
		return id;
	}

	void LibraryMainViewController::select_image(int id)
	{
		DBG_OUT("library select %d", id);
		std::map<int, Gtk::TreeIter>::iterator iter
			= m_idmap.find( id );
		if(iter != m_idmap.end()) {
			// found the icon view item
			Gtk::TreePath path = m_model->get_path(iter->second);
			m_librarylistview.select_path(path);
		}
	}

}
