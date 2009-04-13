/*
 * niepce - ui/filmstripcontroller.cpp
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


#include <gtkmm/iconview.h>

#include "niepce/notifications.h"
#include "engine/db/library.hpp"
#include "engine/library/thumbnailnotification.h"
#include "fwk/utils/debug.h"

#include "eog-thumb-nav.h"
#include "eog-thumb-view.h"
#include "filmstripcontroller.hpp"

namespace ui {

FilmStripController::FilmStripController(const Glib::RefPtr<ImageListStore> & store)
    : m_store(store)
{
}

Gtk::Widget * FilmStripController::buildWidget()
{
    DBG_ASSERT(m_store, "m_store NULL");
	m_thumbview = Glib::wrap(GTK_ICON_VIEW(eog_thumb_view_new(m_store)));
	GtkWidget *thn = eog_thumb_nav_new(GTK_WIDGET(m_thumbview->gobj()), 
									   EOG_THUMB_NAV_MODE_ONE_ROW, true);
	gtk_icon_view_set_selection_mode(GTK_ICON_VIEW(m_thumbview->gobj()),
									 GTK_SELECTION_SINGLE);
	m_widget = Glib::wrap(thn);
	return m_widget;
}

Gtk::IconView * FilmStripController::image_list() 
{ 
	return m_thumbview; 
}

int FilmStripController::get_selected()
{
	int id = 0;
	Gtk::IconView::ArrayHandle_TreePaths paths = m_thumbview->get_selected_items();
	if(!paths.empty()) {
		Gtk::TreePath path(*(paths.begin()));
		DBG_OUT("found path %s", path.to_string().c_str());
		Gtk::TreeRow row = *(m_store->get_iter(path));
		if(row) {
			DBG_OUT("found row");
            db::LibFile::Ptr libfile = row[m_store->columns().m_libfile];
            if(libfile) {
                id = libfile->id();
            }
		}
	}
	return id;
}

void FilmStripController::select_image(int id)
{
	DBG_OUT("filmstrip select %d", id);
    Gtk::TreePath path = m_store->get_path_from_id(id);
    m_thumbview->select_path(path);
}


#if 0
void FilmStripController::on_lib_notification(const framework::Notification::Ptr &n)
{
	DBG_ASSERT(n->type() == niepce::NOTIFICATION_LIB, "wrong notification type");
	if(n->type() == niepce::NOTIFICATION_LIB) {
		db::LibNotification ln = boost::any_cast<db::LibNotification>(n->data());
		switch(ln.type) {
		case db::Library::NOTIFY_FOLDER_CONTENT_QUERIED:
		case db::Library::NOTIFY_KEYWORD_CONTENT_QUERIED:
		{
			db::LibFile::ListPtr l 
				= boost::any_cast<db::LibFile::ListPtr>(ln.param);
			DBG_OUT("received folder content file # %d", l->size());
			
			Glib::RefPtr<EogListStore> store(new EogListStore( *l ));
			eog_thumb_view_set_model((EogThumbView*)(m_thumbview->gobj()), 
									 store);
			break;
		}
		default:
			break;
		}
	}
}


void FilmStripController::on_tnail_notification(const framework::Notification::Ptr &n)
{
	DBG_ASSERT(n->type() == niepce::NOTIFICATION_THUMBNAIL, "wrong notification type");
	if(n->type() == niepce::NOTIFICATION_THUMBNAIL)	{
		Glib::RefPtr<EogListStore> store 
			= eog_thumb_view_get_model((EogThumbView*)(m_thumbview->gobj()));
		library::ThumbnailNotification tn 
			= boost::any_cast<library::ThumbnailNotification>(n->data());
		Gtk::TreeRow row;
		bool found = store->find_by_id(tn.id, row);
		if(found) {
			// FIXME parametrize
			row[store->m_columns.m_thumbnail] = framework::gdkpixbuf_scale_to_fit(tn.pixmap, 100);
		}
		else {
			DBG_OUT("row %d not found", tn.id);
		}
	}
}
#endif

}

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
