/*
 * niepce - niepce/ui/filmstripcontroller.cpp
 *
 * Copyright (C) 2008-2009 Hubert Figuiere
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

#include "niepce/notifications.hpp"
#include "engine/db/library.hpp"
#include "engine/library/thumbnailnotification.hpp"
#include "fwk/base/debug.hpp"

#include "eog-thumb-nav.hpp"
#include "thumbstripview.hpp"
#include "filmstripcontroller.hpp"

namespace ui {

FilmStripController::FilmStripController(const Glib::RefPtr<ImageListStore> & store)
    : m_store(store)
{
}

Gtk::Widget * FilmStripController::buildWidget(const Glib::RefPtr<Gtk::UIManager> &)
{
    if(m_widget) {
        return m_widget;
    }
    DBG_ASSERT(m_store, "m_store NULL");
    m_thumbview = manage(new ThumbStripView(m_store));
    GtkWidget *thn = eog_thumb_nav_new(m_thumbview, 
                                       EOG_THUMB_NAV_MODE_ONE_ROW, true);
    m_thumbview->set_selection_mode(Gtk::SELECTION_SINGLE);
    m_widget = Glib::wrap(thn);
    m_widget->set_size_request(-1, 145);
    return m_widget;
}

Gtk::IconView * FilmStripController::image_list() 
{ 
    return m_thumbview;
}

int FilmStripController::get_selected()
{
    int id = 0;
    DBG_OUT("get selected in filmstrip");
    Gtk::IconView::ArrayHandle_TreePaths paths = m_thumbview->get_selected_items();
    if(!paths.empty()) {
        Gtk::TreePath path(*(paths.begin()));
        DBG_OUT("found path %s", path.to_string().c_str());
        Gtk::TreeRow row = *(m_store->get_iter(path));
        if(row) {
            DBG_OUT("found row");
            eng::LibFile::Ptr libfile = row[m_store->columns().m_libfile];
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
