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
#include "niepce/notifications.hpp"
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

Glib::RefPtr<ImageListStore> ImageListStore::create()
{
    static const Columns columns;
    return Glib::RefPtr<ImageListStore>(new ImageListStore(columns));
}

ImageListStore::ImageListStore(const Columns& _columns)
    : Gtk::ListStore(_columns)
    , m_columns(_columns)
    , m_current_folder(0)
    , m_current_keyword(0)
{
}


Gtk::TreeIter ImageListStore::get_iter_from_id(eng::library_id_t id) const
{
    auto iter = m_idmap.find( id );
    if(iter != m_idmap.end()) {
        return iter->second;
    }
    return Gtk::TreeIter();
}

Gtk::TreePath ImageListStore::get_path_from_id(eng::library_id_t id) const
{
    Gtk::TreeIter iter = get_iter_from_id(id);
    if(iter) {
        return get_path(iter);
    }
    return Gtk::TreePath();
}

eng::library_id_t ImageListStore::get_libfile_id_at_path(const Gtk::TreePath& path)
{
    auto iter = get_iter(path);
    eng::LibFilePtr libfile = (*iter)[columns().m_libfile];
    if (libfile) {
        return engine_db_libfile_id(libfile.get());
    }
    return 0;
}

eng::LibFilePtr ImageListStore::get_file(eng::library_id_t id) const
{
    auto iter = get_iter_from_id(id);
    if (iter) {
        return (*iter)[m_columns.m_libfile];
    }
    return eng::LibFilePtr();
}

void ImageListStore::add_libfile(const eng::LibFilePtr & f)
{
    Gtk::TreeModel::iterator riter = append();
    Gtk::TreeRow row = *riter;
    Glib::RefPtr<Gdk::Pixbuf> icon = get_loading_icon();
    row[m_columns.m_pix] = icon;
    row[m_columns.m_libfile] = f;
    row[m_columns.m_strip_thumb]
        = fwk::gdkpixbuf_scale_to_fit(icon, 100);
    row[m_columns.m_file_status] = eng::FileStatus::Ok;
    m_idmap[engine_db_libfile_id(f.get())] = riter;
}

void ImageListStore::clear_content()
{
    // clear the map before the list.
    m_idmap.clear();
    clear();
}

void ImageListStore::on_lib_notification(const eng::LibNotification &ln)
{
    auto type = engine_library_notification_type(&ln);
    switch (type) {
    case eng::NotificationType::FOLDER_CONTENT_QUERIED:
    case eng::NotificationType::KEYWORD_CONTENT_QUERIED:
    {
        auto content = engine_library_notification_get_content(&ln);
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
        getLibraryClient()->thumbnailCache().request(*l);
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
                iter = erase(iter);
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
            row[m_columns.m_file_status] = status;
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
            if(iter != m_idmap.end()) {
                Gtk::TreeRow row = *(iter->second);
                //
                eng::LibFilePtr file = row[m_columns.m_libfile];
                engine_db_libfile_set_property(
                    file.get(), prop, fwk_property_value_get_integer(
                        ffi::metadatachange_get_value(m)));
                row[m_columns.m_libfile] = file;
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
