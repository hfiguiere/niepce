/*
 * niepce - ui/imageliststore.cpp
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

#include <gtkmm/icontheme.h>

#include "imageliststore.hpp"
#include "fwk/base/debug.hpp"
#include "fwk/toolkit/application.hpp"
#include "fwk/toolkit/gdkutils.hpp"
#include "niepce/notifications.hpp"
#include "niepcewindow.hpp"

namespace ui {

Glib::RefPtr<ImageListStore> ImageListStore::create()
{
    static const Columns columns;
    Glib::RefPtr<ImageListStore> p(new ImageListStore(columns));
    return p;
}

ImageListStore::ImageListStore(const Columns& _columns)
    : Gtk::ListStore(_columns),
      m_columns(_columns)
{
}


Gtk::TreeIter ImageListStore::get_iter_from_id(eng::library_id_t id)
{
    std::map<eng::library_id_t, Gtk::TreeIter>::iterator iter
        = m_idmap.find( id );
    if(iter != m_idmap.end()) {
        return iter->second;
    }
    return Gtk::TreeIter();
}

Gtk::TreePath ImageListStore::get_path_from_id(eng::library_id_t id)
{
    Gtk::TreeIter iter = get_iter_from_id(id);
    if(iter) {
        return get_path(iter);
    }
    return Gtk::TreePath();
}


void ImageListStore::on_lib_notification(const eng::LibNotification &ln)
{
    switch(ln.type) {
    case eng::Library::NOTIFY_FOLDER_CONTENT_QUERIED:
    case eng::Library::NOTIFY_KEYWORD_CONTENT_QUERIED:
    {
        eng::LibFile::ListPtr l 
            = boost::any_cast<eng::LibFile::ListPtr>(ln.param);
        DBG_OUT("received folder content file # %lu", l->size());
        Glib::RefPtr< Gtk::IconTheme > icon_theme(fwk::Application::app()->getIconTheme());
        clear();
        m_idmap.clear();
        eng::LibFile::List::const_iterator iter = l->begin();
        for( ; iter != l->end(); iter++ )
        {
            Gtk::TreeModel::iterator riter = append();
            Gtk::TreeRow row = *riter;
            // locate it in local cache...
            // this would avoid exception handling.
            Glib::RefPtr<Gdk::Pixbuf> icon;
            try {
                icon = icon_theme->load_icon(
                    Glib::ustring("image-loading"), 32,
                    Gtk::ICON_LOOKUP_USE_BUILTIN);
            }
            catch(const Gtk::IconThemeError & e)
            {
                ERR_OUT("Exception %s.", e.what().c_str());
            }
            row[m_columns.m_pix] = icon;
            row[m_columns.m_libfile] = *iter;
            row[m_columns.m_strip_thumb] = fwk::gdkpixbuf_scale_to_fit(icon, 100);
            m_idmap[(*iter)->id()] = riter;
        }
        // at that point clear the cache because the icon view is populated.
        getLibraryClient()->thumbnailCache().request(l);
        break;
    }
    case eng::Library::NOTIFY_METADATA_CHANGED:
    {
		eng::metadata_desc_t m = boost::any_cast<eng::metadata_desc_t>(ln.param);
        DBG_OUT("metadata changed");
        Gtk::TreeRow row;
        std::map<eng::library_id_t, Gtk::TreeIter>::const_iterator iter = m_idmap.find(m.id);
        if(iter != m_idmap.end()) {
            row = *(iter->second);
            //
            eng::LibFile::Ptr file = row[m_columns.m_libfile];
            file->setMetaData(m.meta, m.value);
            row[m_columns.m_libfile] = file;
        }
        break;
    }
    case eng::Library::NOTIFY_XMP_NEEDS_UPDATE:
    {
        getLibraryClient()->processXmpUpdateQueue();
        break;
    }
    default:
        break;
    }
}

void ImageListStore::on_tnail_notification(const eng::ThumbnailNotification &tn)
{
    std::map<eng::library_id_t, Gtk::TreeIter>::iterator iter
        = m_idmap.find( tn.id );
    if(iter != m_idmap.end()) {
        // found the icon view item
        Gtk::TreeRow row = *(iter->second);
        row[m_columns.m_pix] = tn.pixmap;
        row[m_columns.m_strip_thumb] = fwk::gdkpixbuf_scale_to_fit(tn.pixmap, 100);
    }
    else {
        DBG_OUT("row %Ld not found", tn.id);
    }
}

libraryclient::LibraryClient::Ptr ImageListStore::getLibraryClient()
{
    ModuleShell::Ptr shell = std::tr1::dynamic_pointer_cast<ModuleShell>(m_controller.lock());
    DBG_ASSERT(shell, "parent not a ModuleShell");
    return	shell->getLibraryClient();
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
