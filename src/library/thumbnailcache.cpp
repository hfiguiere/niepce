/*
 * niepce - library/thumbnailcache.cpp
 *
 * Copyright (C) 2007-2008 Hubert Figuiere
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

#include <gdkmm/pixbuf.h>
#include <libopenraw-gnome/gdkpixbuf.h>

#include "niepce/notifications.h"
#include "utils/debug.h"
#include "framework/mimetype.h"
#include "thumbnailcache.h"
#include "thumbnailnotification.h"

using db::LibFile;

namespace library {

	ThumbnailCache::ThumbnailCache(const boost::filesystem::path & dir,
								   framework::NotificationCenter * nc)
		: m_cacheDir(dir),
		  m_notif_center(nc)
	{
	}

	ThumbnailCache::~ThumbnailCache()
	{
	}

	void ThumbnailCache::request(const LibFile::ListPtr & fl)
	{
		std::for_each(fl->begin(), fl->end(),
					 boost::bind(&ThumbnailCache::requestForFile, this, 
								 _1));
	}

	void ThumbnailCache::requestForFile(const LibFile::Ptr & f)
	{
		ThumbnailTask::Ptr task(new ThumbnailTask(f, 160, 160));
		schedule( task );
	}


	void ThumbnailCache::execute(const  ThumbnailTask::Ptr & task)
	{
		const char *filename = task->file()->path().string().c_str();
		DBG_OUT("creating thumbnail for %s",filename);

		framework::MimeType mime_type(filename);


		DBG_OUT("MIME type %s", mime_type.string().c_str());

		if(mime_type.isUnknown()) {
			DBG_OUT("unknown file type", filename);
			return;
		}
		if(!mime_type.isImage()) {
			DBG_OUT("not an image type");
			return;
		}
		
		Glib::RefPtr<Gdk::Pixbuf> pix;
		if(!mime_type.isDigicamRaw()) {
			DBG_OUT("not a raw type, trying GdkPixbuf loaders");
			pix = Gdk::Pixbuf::create_from_file(filename, task->width(), task->height(), true);
		}	
		else {	
			GdkPixbuf *pixbuf = or_gdkpixbuf_extract_rotated_thumbnail(filename, 
																	   std::min(task->width(),
																				task->height()));
			if( pixbuf )
			{
				 pix = Glib::wrap( pixbuf, true ); // take ownership
			}
		}
		if(pix)
		{
			if(m_notif_center) {
				// pass the notification
				framework::Notification::Ptr n(new framework::Notification(niepce::NOTIFICATION_THUMBNAIL));
				ThumbnailNotification tn;
				tn.id = task->file()->id();
				tn.width = pix->get_width();
				tn.height = pix->get_height();
				tn.pixmap = pix;
				n->setData(boost::any(tn));
				DBG_OUT("notify thumbnail for id=%d", tn.id);
				m_notif_center->post(n);
			}
		}
		else 
		{
			DBG_OUT("couldn't get the thumbnail for %s", filename);
		}
	}

}
