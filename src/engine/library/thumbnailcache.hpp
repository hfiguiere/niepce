/*
 * niepce - library/thumbnailcache.h
 *
 * Copyright (C) 2007-2013 Hubert Figuiere
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


#ifndef _LIBRARY_THUMBNAILCACHE_H__
#define _LIBRARY_THUMBNAILCACHE_H__

#include <memory>

#include "fwk/utils/worker.hpp"
#include "fwk/toolkit/notificationcenter.hpp"
#include "engine/db/libfile.hpp"

namespace eng {


class ThumbnailTask
{
public:
    typedef std::unique_ptr<ThumbnailTask> Ptr;

    ThumbnailTask(const LibFile::Ptr & f, int w, int h)
        : m_file(f), m_width(w), m_height(h)
        { }

    const LibFile::Ptr & file()
        { return m_file; }
    int width() const
        { return m_width; }
    int height() const
        { return m_height; }
private:
    const LibFile::Ptr m_file;
    int m_width;
    int m_height;
};


class ThumbnailCache
    : private fwk::Worker<ThumbnailTask>
{
public:
    ThumbnailCache(const std::string & dir,
                   const fwk::NotificationCenter::Ptr & nc);
    ~ThumbnailCache();

    void request(const LibFile::ListPtr & fl);

    static bool is_thumbnail_cached(const std::string & file, const std::string & thumb);

protected:
    virtual void execute(const ptr_t & task) override;
private:
    std::string                                 m_cacheDir;
    std::weak_ptr<fwk::NotificationCenter> m_notif_center;

    std::string path_for_thumbnail(const std::string & filename, library_id_t id, int size) const;
    std::string dir_for_thumbnail(int size) const;
};

}

#endif
/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:80
  End:
*/
