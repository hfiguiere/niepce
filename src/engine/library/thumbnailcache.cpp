/*
 * niepce - library/thumbnailcache.cpp
 *
 * Copyright (C) 2007-2008,2011 Hubert Figuiere
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

#include <functional>
#include <boost/bind.hpp>
#include <boost/any.hpp>
#include <boost/lexical_cast.hpp>

#include <gdkmm/pixbuf.h>
#include <libopenraw-gnome/gdkpixbuf.h>

#include "niepce/notifications.hpp"
#include "fwk/base/debug.hpp"
#include "fwk/utils/pathutils.hpp"
#include "fwk/toolkit/mimetype.hpp"
#include "fwk/toolkit/gdkutils.hpp"
#include "fwk/toolkit/movieutils.hpp"
#include "thumbnailcache.hpp"
#include "thumbnailnotification.hpp"

namespace eng {

ThumbnailCache::ThumbnailCache(const std::string & dir,
                               const fwk::NotificationCenter::Ptr & nc)
    : m_cacheDir(dir),
      m_notif_center(nc)
{
}

ThumbnailCache::~ThumbnailCache()
{
}

void ThumbnailCache::request(const LibFile::ListPtr & fl)
{
    clear();
    std::for_each(fl->begin(), fl->end(),
                  boost::bind(&ThumbnailCache::requestForFile, this, 
                              _1));
}

void ThumbnailCache::requestForFile(const LibFile::Ptr & f)
{
    ThumbnailTask::Ptr task(new ThumbnailTask(f, 160, 160));
    schedule( task );
}

namespace {

// TODO see about 1. moving this out 2. abstracting the type away from Gdk::Pixbuf
// Gdk does not belong to eng.
Glib::RefPtr<Gdk::Pixbuf> getThumbnail(const LibFile::Ptr & f, int w, int h, const std::string & cached)
{
    if(ThumbnailCache::is_thumbnail_cached(f->path(), cached)) {
        DBG_OUT("cached!");
        return Gdk::Pixbuf::create_from_file(cached);
    }
    const std::string & filename = f->path();
    DBG_OUT("creating thumbnail for %s",filename.c_str());

    fwk::MimeType mime_type(filename);
    if(!fwk::ensure_path_for_file(cached)) {
        ERR_OUT("coudln't create directories for %s", cached.c_str());
    }

    DBG_OUT("MIME type %s", mime_type.string().c_str());
	
    Glib::RefPtr<Gdk::Pixbuf> pix;

    if(mime_type.isUnknown()) {
        DBG_OUT("unknown file type %s", filename.c_str());
    }
    // TODO: what about videos?
    else if(mime_type.isMovie()) {
        if(fwk::thumbnail_movie(filename, w, h, cached)) {
            pix = Gdk::Pixbuf::create_from_file(cached, w, h, true);
        }
    }
    else if(!mime_type.isImage()) {
        DBG_OUT("not an image type");
    }
    else if(!mime_type.isDigicamRaw()) {
        DBG_OUT("not a raw type, trying GdkPixbuf loaders");
        try {
            pix = Gdk::Pixbuf::create_from_file(filename, w, h, true);
            if(pix) {
                pix = fwk::gdkpixbuf_exif_rotate(pix, f->orientation());
            }
        }
        catch(const Glib::Error & e) {
            ERR_OUT("exception %s", e.what().c_str());
        }
    }	
    else {	
        GdkPixbuf *pixbuf = or_gdkpixbuf_extract_rotated_thumbnail(filename.c_str(), 
                                                                   std::min(w, h));
        if(pixbuf) {
            pix = Glib::wrap(pixbuf, true); // take ownership
        }
        if((w < pix->get_width()) || (h < pix->get_height())) {
            pix = fwk::gdkpixbuf_scale_to_fit(pix, std::min(w,h));
        }
    }
    if(pix)
    {
        pix->save(cached, "png");
    }
    else {
        DBG_OUT("couldn't get the thumbnail for %s", filename.c_str());
    }
    return pix;
}

}

void ThumbnailCache::execute(const  ThumbnailTask::Ptr & task)
{
    int w, h;
    w = task->width();
    h = task->height();

    Glib::RefPtr<Gdk::Pixbuf> pix;

    std::string dest = path_for_thumbnail(task->file()->path(), std::max(w,h));
    DBG_OUT("cached thumbnail %s", dest.c_str());

    pix = getThumbnail(task->file(), w, h, dest);

    if(pix) {
        fwk::NotificationCenter::Ptr nc(m_notif_center);
        if(nc) {
            // pass the notification
            fwk::Notification::Ptr n(new fwk::Notification(niepce::NOTIFICATION_THUMBNAIL));
            ThumbnailNotification tn;
            tn.id = task->file()->id();
            tn.width = pix->get_width();
            tn.height = pix->get_height();
            tn.pixmap = pix;
            n->setData(boost::any(tn));
            DBG_OUT("notify thumbnail for id=%Ld", tn.id);
            nc->post(n);
        }
    }
}

std::string ThumbnailCache::path_for_thumbnail(const std::string & filename, int size) const
{
    // todo compute a hash
    return dir_for_thumbnail(size) + fwk::path_basename(filename) + ".png";
}

std::string ThumbnailCache::dir_for_thumbnail(int size) const
{
    std::string subdir = size ? boost::lexical_cast<std::string>(size) : "full";
    return m_cacheDir + "/" + subdir + "/";
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
