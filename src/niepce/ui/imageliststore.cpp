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

#include <mutex>

#include <gtkmm/icontheme.h>

#include "imageliststore.hpp"
#include "engine/db/properties.hpp"
#include "fwk/base/debug.hpp"
#include "fwk/toolkit/application.hpp"
#include "fwk/toolkit/gdkutils.hpp"
#include "niepce/notifications.hpp"
#include "niepcewindow.hpp"

namespace ui {

Glib::RefPtr<Gdk::Pixbuf> ImageListStore::get_loading_icon()
{
    Glib::RefPtr<Gdk::Pixbuf> icon;
    if(!icon) {
        static std::mutex m;
        m.lock();
        if (!icon) {
            auto icon_theme(fwk::Application::app()->getIconTheme());
            try {
                icon = icon_theme->load_icon(
                    Glib::ustring("image-loading"), 32,
                    Gtk::ICON_LOOKUP_USE_BUILTIN);
            }
            catch(const Gtk::IconThemeError & e)
            {
                ERR_OUT("Exception %s.", e.what().c_str());
            }
        }
        m.unlock();
    }
    return icon;
}

Glib::RefPtr<ImageListStore> ImageListStore::create()
{
    static const Columns columns;
    return Glib::RefPtr<ImageListStore>(new ImageListStore(columns));
}

ImageListStore::ImageListStore(const Columns& _columns)
    : Gtk::ListStore(_columns),
      m_columns(_columns)
{
}


Gtk::TreeIter ImageListStore::get_iter_from_id(eng::library_id_t id)
{
    auto iter = m_idmap.find( id );
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
    case eng::Library::NotifyType::FOLDER_CONTENT_QUERIED:
    case eng::Library::NotifyType::KEYWORD_CONTENT_QUERIED:
    {
        eng::LibFile::ListPtr l 
            = boost::any_cast<eng::LibFile::ListPtr>(ln.param);
        DBG_OUT("received folder content file # %lu", l->size());
        // clear the map before the list.
        m_idmap.clear();
        clear();
        eng::LibFile::List::const_iterator iter = l->begin();
        for_each(l->begin(), l->end(),
                 [this](const eng::LibFile::Ptr & f) {
                     Gtk::TreeModel::iterator riter = append();
                     Gtk::TreeRow row = *riter;
                     Glib::RefPtr<Gdk::Pixbuf> icon = get_loading_icon();
                     row[m_columns.m_pix] = icon;
                     row[m_columns.m_libfile] = f;
                     row[m_columns.m_strip_thumb]
                         = fwk::gdkpixbuf_scale_to_fit(icon, 100);
                     m_idmap[f->id()] = riter;
                 });
        // at that point clear the cache because the icon view is populated.
        getLibraryClient()->thumbnailCache().request(l);
        break;
    }
    case eng::Library::NotifyType::METADATA_CHANGED:
    {
        eng::metadata_desc_t m = boost::any_cast<eng::metadata_desc_t>(ln.param);
        fwk::PropertyIndex prop = m.meta;
        DBG_OUT("metadata changed %s", eng::_propertyName(prop));
        // only interested in a few props
        if(is_property_interesting(prop)) {
            std::map<eng::library_id_t, Gtk::TreeIter>::const_iterator iter = m_idmap.find(m.id);
            if(iter != m_idmap.end()) {
                Gtk::TreeRow row = *(iter->second);
                //
                eng::LibFile::Ptr file = row[m_columns.m_libfile];
                file->setProperty(prop, boost::get<int>(m.value));
                row[m_columns.m_libfile] = file;
            }
        }
        break;
    }
    case eng::Library::NotifyType::XMP_NEEDS_UPDATE:
    {
        fwk::Configuration & cfg = fwk::Application::app()->config();
        int write_xmp = false;
        Glib::ustring xmp_pref;
        try {
            xmp_pref = cfg.getValue("write_xmp_automatically", "0");
            write_xmp = std::stoi(xmp_pref);
        }
        catch(const std::exception & e)
        {
            ERR_OUT("couldn't cast %s: %s", xmp_pref.c_str(),
                    e.what());
        }
        getLibraryClient()->processXmpUpdateQueue(write_xmp);
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
        auto pixbuf = tn.pixmap.pixbuf();
        Gtk::TreeRow row = *(iter->second);
        row[m_columns.m_pix] = pixbuf;
        row[m_columns.m_strip_thumb] = fwk::gdkpixbuf_scale_to_fit(pixbuf, 100);
    }
    else {
        DBG_OUT("row %Ld not found", (long long)tn.id);
    }
}

libraryclient::LibraryClientPtr ImageListStore::getLibraryClient()
{
    ModuleShell::Ptr shell = std::dynamic_pointer_cast<ModuleShell>(m_controller.lock());
    DBG_ASSERT(static_cast<bool>(shell), "parent not a ModuleShell");
    return shell->getLibraryClient();
}


bool ImageListStore::is_property_interesting(fwk::PropertyIndex idx)
{
    return (idx == eng::NpXmpRatingProp) || (idx == eng::NpXmpLabelProp)
        || (idx == eng::NpTiffOrientationProp) || (idx == eng::NpNiepceFlagProp);
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
