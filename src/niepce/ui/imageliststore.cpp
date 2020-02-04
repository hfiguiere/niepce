/*
 * niepce - ui/imageliststore.cpp
 *
 * Copyright (C) 2008-2020 Hubert Figuière
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
#include "niepcewindow.hpp"

#include "rust_bindings.hpp"

namespace ui {

Glib::RefPtr<Gdk::Pixbuf> ImageListStore::get_loading_icon()
{
    static Glib::RefPtr<Gdk::Pixbuf> icon;
    if (!icon) {
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

ImageListStorePtr ImageListStore::create()
{
    return std::make_shared<ImageListStore>(ffi::npc_image_list_store_new());
}

ImageListStore::ImageListStore(ffi::ImageListStore* store)
    : m_store(store)
    , m_store_wrap(Glib::wrap(GTK_LIST_STORE(ffi::npc_image_list_store_gobj(store)), true))
    , m_current_folder(0)
    , m_current_keyword(0)
{
}

ImageListStore::~ImageListStore()
{
    m_store_wrap.reset();
    ffi::npc_image_list_store_delete(m_store);
}

Gtk::TreeIter ImageListStore::get_iter_from_id(eng::library_id_t id) const
{
    auto iter = m_idmap.find(id);
    if(iter != m_idmap.end()) {
        return iter->second;
    }
    return Gtk::TreeIter();
}

Gtk::TreePath ImageListStore::get_path_from_id(eng::library_id_t id) const
{
    Gtk::TreeIter iter = get_iter_from_id(id);
    if(iter) {
        return m_store_wrap->get_path(iter);
    }
    return Gtk::TreePath();
}

eng::library_id_t ImageListStore::get_libfile_id_at_path(const Gtk::TreePath &path) const
{
    return ffi::npc_image_list_store_get_file_id_at_path(m_store, path.gobj());
}

eng::LibFilePtr ImageListStore::get_file(eng::library_id_t id) const
{
    auto iter = get_iter_from_id(id);
    if (iter) {
        auto f = ffi::npc_image_list_store_get_file(m_store, iter.gobj());
        if (f) {
            return eng::libfile_wrap(f);
        }
    }
    return eng::LibFilePtr();
}

void ImageListStore::add_libfile(const eng::LibFilePtr & f)
{
    Glib::RefPtr<Gdk::Pixbuf> icon = get_loading_icon();
    DBG_ASSERT(static_cast<bool>(f), "f can't be null");
    auto iter = ffi::npc_image_list_store_add_row(m_store,
        icon->gobj(), f.get(), fwk::gdkpixbuf_scale_to_fit(icon, 100)->gobj(),
        eng::FileStatus::Ok);

    m_idmap[engine_db_libfile_id(f.get())] = Gtk::TreeIter(
        GTK_TREE_MODEL(m_store_wrap->gobj()), &iter);
}

void ImageListStore::clear_content()
{
    // clear the map before the list.
    m_idmap.clear();
    m_store_wrap->clear();
}

void ImageListStore::on_lib_notification(const eng::LibNotification &ln)
{
    auto type = engine_library_notification_type(&ln);
    switch (type) {
    case eng::NotificationType::FOLDER_CONTENT_QUERIED:
    case eng::NotificationType::KEYWORD_CONTENT_QUERIED:
    {
        auto content = ffi::engine_library_notification_get_content(&ln);
        if (type == eng::NotificationType::FOLDER_CONTENT_QUERIED) {
            m_current_folder = ffi::engine_queried_content_id(content);
            m_current_keyword = 0;
        } else if (type == eng::NotificationType::KEYWORD_CONTENT_QUERIED) {
            m_current_folder = 0;
            m_current_keyword = ffi::engine_queried_content_id(content);
        }
        clear_content();
        DBG_OUT("received folder content file # %lu", engine_queried_content_size(content));
        eng::LibFileListPtr l(new eng::LibFileList);
        for (uint64_t i = 0; i < engine_queried_content_size(content); i++) {
            auto f = eng::libfile_wrap(engine_queried_content_get(content, i));
            add_libfile(f);
            l->push_back(f);
        }
        // at that point clear the cache because the icon view is populated.
        ffi::engine_library_thumbnail_cache_request(getLibraryClient()->thumbnailCache(), content);
        break;
    }
    case eng::NotificationType::FILE_MOVED:
    {
        DBG_OUT("File moved. Current folder %ld", m_current_folder);
        auto param = engine_library_notification_get_filemoved(&ln);
        if (m_current_folder == 0) {
            return;
        }
        if (param->from == m_current_folder) {
            // remove from list
            DBG_OUT("from this folder");
            auto iter = get_iter_from_id(param->file);
            if (iter) {
                iter = m_store_wrap->erase(iter);
            }
        } else if (param->to == m_current_folder) {
            // XXX add to list. but this isn't likely to happen atm.
        }
        break;
    }
    case eng::NotificationType::FILE_STATUS_CHANGED:
    {
        auto status = engine_library_notification_get_filestatus(&ln);
        auto id = engine_library_notification_get_id(&ln);
        auto iter = m_idmap.find(id);
        if (iter != m_idmap.end()) {
            Gtk::TreeRow row = *(iter->second);
            row[m_columns.m_file_status] = static_cast<gint>(status);
        }
        break;
    }
    case eng::NotificationType::METADATA_CHANGED:
    {
        auto m = engine_library_notification_get_metadatachange(&ln);
        const fwk::PropertyIndex& prop = ffi::metadatachange_get_meta(m);
        DBG_OUT("metadata changed %s", eng::_propertyName(prop));
        // only interested in a few props
        if(is_property_interesting(prop)) {
            std::map<eng::library_id_t, Gtk::TreeIter>::const_iterator iter =
                m_idmap.find(ffi::metadatachange_get_id(m));
            if (iter != m_idmap.end()) {
                ffi::npc_image_list_store_set_property(m_store, iter->second.gobj(), m);
            }
        }
        break;
    }
    case eng::NotificationType::XMP_NEEDS_UPDATE:
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
        ffi::libraryclient_process_xmp_update_queue(getLibraryClient()->client(), write_xmp);
        break;
    }
    case eng::NotificationType::ThumbnailLoaded:
    {
        auto id = engine_library_notification_get_id(&ln);
        auto iter = m_idmap.find(id);
        if(iter != m_idmap.end()) {
            // found the icon view item
            auto pixbuf = engine_library_notification_get_pixbuf(&ln);
            // About the ownership:
            // The Glib::wrap below will claim it.
            ffi::npc_image_list_store_set_tnail(
                m_store, iter->second.gobj(), pixbuf,
                fwk::gdkpixbuf_scale_to_fit(Glib::wrap(pixbuf), 100)->gobj());
        }
        else {
            DBG_OUT("row %Ld not found", (long long)id);
        }
    }
    default:
        break;
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
