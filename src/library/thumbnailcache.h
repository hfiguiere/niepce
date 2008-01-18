/*
 * niepce - library/thumbnailcache.h
 *
 * Copyright (C) 2007 Hubert Figuiere
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

#include <boost/filesystem/path.hpp>


#include "utils/worker.h"
#include "framework/notificationcenter.h"
#include "db/libfile.h"

namespace library {


	class ThumbnailTask
	{
	public:
		typedef boost::shared_ptr< ThumbnailTask > Ptr;
		
		ThumbnailTask(const db::LibFile::Ptr & f, int w, int h)
			: m_file(f), m_width(w), m_height(h)
			{ }
		
		const db::LibFile::Ptr & file()
			{ return m_file; }
		int width() const
			{ return m_width; }
		int height() const
			{ return m_height; }
	private:
		const db::LibFile::Ptr m_file;
		int m_width;
		int m_height;
	};


	class ThumbnailCache
		: private utils::Worker< ThumbnailTask::Ptr >
	{
	public:
		ThumbnailCache(const boost::filesystem::path & dir,
			framework::NotificationCenter * nc);
		~ThumbnailCache();

		void request(const db::LibFile::ListPtr & fl);
		void requestForFile(const db::LibFile::Ptr & f);

	protected:
		virtual void execute(const  ThumbnailTask::Ptr & task);
	private:
		boost::filesystem::path         m_cacheDir;
		framework::NotificationCenter * m_notif_center;
	};

}

#endif
