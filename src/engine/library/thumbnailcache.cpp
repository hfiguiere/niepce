/*
 * niepce - library/thumbnailcache.cpp
 *
 * Copyright (C) 2007-2017 Hubert Figuière
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

#include <string.h>

#include <boost/any.hpp>
#include <boost/format.hpp>

#include <glibmm/miscutils.h>

#include "niepce/notifications.hpp"
#include "niepce/notificationcenter.hpp"
#include "fwk/base/debug.hpp"
#include "fwk/utils/pathutils.hpp"
#include "fwk/toolkit/thumbnail.hpp"
#include "fwk/toolkit/notificationcenter.hpp"
#include "thumbnailcache.hpp"
#include "thumbnailnotification.hpp"

namespace eng {

ThumbnailCache::ThumbnailCache(const std::string & dir, uint64_t notif_id)
    : m_cacheDir(dir),
      m_notif_id(notif_id)
{
}

ThumbnailCache::~ThumbnailCache()
{
}

void ThumbnailCache::request(const LibFileList& fl)
{
    clear();
    std::for_each(fl.begin(), fl.end(),
                  [this] (const LibFilePtr& f) {
                      ThumbnailTask::Ptr task(new ThumbnailTask(f, 160, 160));
                      this->schedule(task);
                  });
}

namespace {

fwk::Thumbnail getThumbnail(const LibFilePtr & f, int w, int h, const std::string & cached)
{
    std::string filename = engine_db_libfile_path(f.get());

    if (ThumbnailCache::is_thumbnail_cached(filename, cached)) {
        DBG_OUT("thumbnail for %s is cached!", filename.c_str());
        return Gdk::Pixbuf::create_from_file(cached);
    }

    DBG_OUT("creating thumbnail for %s", filename.c_str());

    if(!fwk::ensure_path_for_file(cached)) {
        ERR_OUT("coudln't create directories for %s", cached.c_str());
    }

    auto thumbnail = fwk::Thumbnail::thumbnail_file(filename, w, h,
                                                    engine_db_libfile_orientation(f.get()));
    if (thumbnail.ok()) {
        thumbnail.save(cached, "png");
    } else {
        DBG_OUT("couldn't get the thumbnail for %s", filename.c_str());
    }
    return thumbnail;
}

}

void ThumbnailCache::execute(const ptr_t & task)
{
    int w, h;
    w = task->width();
    h = task->height();

    auto libfile = task->file();
    std::string path = engine_db_libfile_path(libfile.get());
    auto id = engine_db_libfile_id(libfile.get());
    std::string dest = path_for_thumbnail(path, id, std::max(w,h));
    DBG_OUT("cached thumbnail %s", dest.c_str());

    fwk::Thumbnail pix = getThumbnail(libfile, w, h, dest);
    if (!fwk::path_exists(path)) {
        DBG_OUT("file doesn't exist");
        auto wnc = fwk::NotificationCenter::get_nc(m_notif_id);
        auto nc = wnc.lock();
        if (nc) {
            auto lnc = std::static_pointer_cast<niepce::NotificationCenter>(nc);
            ffi::engine_library_notify_filestatus_changed(lnc->get_channel().get(), id,
                                                          FileStatus::Missing);
        }
    }

    if(!pix.ok()) {
        return;
    }
    auto wnc = fwk::NotificationCenter::get_nc(m_notif_id);
    auto nc = wnc.lock();
    if (nc) {
        // pass the notification
        fwk::Notification::Ptr n(new fwk::Notification(niepce::NOTIFICATION_THUMBNAIL));
        ThumbnailNotification tn{ engine_db_libfile_id(task->file().get()),
                pix.get_width(), pix.get_height(), pix };
        n->setData(boost::any(tn));
        DBG_OUT("notify thumbnail for id=%Ld", (long long)tn.id);
        nc->post(std::move(n));
    }
}

std::string ThumbnailCache::path_for_thumbnail(const std::string & filename, library_id_t id, int size) const
{
    // todo compute a better hash
    std::string thumb_name = str(boost::format("%1%-%2%.png") % id % fwk::path_basename(filename));
    return Glib::build_filename(dir_for_thumbnail(size), thumb_name);
}

std::string ThumbnailCache::dir_for_thumbnail(int size) const
{
    std::string subdir = size ? std::to_string(size) : "full";
    return Glib::build_filename(m_cacheDir, subdir);
}

bool ThumbnailCache::is_thumbnail_cached(const std::string & /*file*/, const std::string & thumb)
{
    return fwk::path_exists(thumb);
}

}
/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:80
  End:
*/
