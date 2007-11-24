/*
 * niepce - db/thumbnailcache.h
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


#ifndef _DB_THUMBNAILCACHE_H__
#define _DB_THUMBNAILCACHE_H__

#include <boost/filesystem/path.hpp>


#include "utils/worker.h"
#include "framework/notificationcenter.h"
#include "db/libfile.h"

namespace db {


	class ThumbnailTask
	{
	public:
		typedef boost::shared_ptr< ThumbnailTask > Ptr;
		
		ThumbnailTask(const LibFile::Ptr & f, int w, int h)
			: m_file(f), m_width(w), m_height(h)
			{ }
		
		const LibFile::Ptr & file()
			{ return m_file; }
		
	private:
		const LibFile::Ptr m_file;
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

		void request(const LibFile::ListPtr & fl);
		void requestForFile(const LibFile::Ptr & f);

	protected:
		virtual void execute(const  ThumbnailTask::Ptr & task);
	private:
		boost::filesystem::path         m_cacheDir;
		framework::NotificationCenter * m_notif_center;
	};

}

#endif
