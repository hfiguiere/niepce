/*
 * niepce - ui/imageliststore.h
 *
 * Copyright (C) 2008-2020 Hubert Figuiere
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

#pragma once

#include <gdkmm/pixbuf.h>
#include <gtkmm/liststore.h>

#include "fwk/base/propertybag.hpp"
#include "fwk/toolkit/controller.hpp"
#include "engine/db/libfile.hpp"
#include "engine/library/thumbnailnotification.hpp"
#include "libraryclient/libraryclient.hpp"

namespace ui {

class ImageListStore;

typedef std::shared_ptr<ImageListStore> ImageListStorePtr;

/** @brief the general list store. Wraps the list store from Rust. */
class ImageListStore
{
public:
    class Columns
        : public Gtk::TreeModelColumnRecord
    {
    public:
        Columns()
            {
                add(m_pix);
                add(m_libfile_do_not_use);
                add(m_strip_thumb);
                add(m_file_status);
            }
        Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf> > m_pix;
        Gtk::TreeModelColumn<eng::LibFilePtr> m_libfile_do_not_use;
        Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf> > m_strip_thumb;
        Gtk::TreeModelColumn<gint> m_file_status;
    };

    ImageListStore(ffi::ImageListStore*);
    ~ImageListStore();
    Glib::RefPtr<Gtk::ListStore> gobjmm() const
        { return m_store_wrap; }
    Gtk::TreePath get_path_from_id(eng::library_id_t id) const;
    Gtk::TreeIter get_iter_from_id(eng::library_id_t id) const;
    eng::library_id_t get_libfile_id_at_path(const Gtk::TreePath& path) const;
    eng::LibFilePtr get_file(eng::library_id_t id) const;
    size_t get_count() const
        { return m_store_wrap->children().size(); }

    static ImageListStorePtr create();

    void set_parent_controller(const fwk::Controller::WeakPtr & ctrl)
        { m_controller = ctrl; }

    // Should be called when the content will change
    void clear_content();
    void on_lib_notification(const eng::LibNotification &n);
    void on_tnail_notification(const eng::ThumbnailNotification &n);

private:
    /// Add the LibFile to the model
    void add_libfile(const eng::LibFilePtr & f);

    static Glib::RefPtr<Gdk::Pixbuf> get_loading_icon();
    libraryclient::LibraryClientPtr getLibraryClient();
    static bool is_property_interesting(fwk::PropertyIndex idx);

    Columns m_columns;
    ffi::ImageListStore* m_store;
    Glib::RefPtr<Gtk::ListStore> m_store_wrap;
    eng::library_id_t m_current_folder;
    eng::library_id_t m_current_keyword;
    std::map<eng::library_id_t, Gtk::TreeIter> m_idmap;
    fwk::Controller::WeakPtr m_controller;
};

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

