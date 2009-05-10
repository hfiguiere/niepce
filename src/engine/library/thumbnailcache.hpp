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

#include <tr1/memory>

#include "fwk/utils/worker.hpp"
#include "fwk/toolkit/notificationcenter.hpp"
#include "engine/db/libfile.hpp"

namespace library {


	class ThumbnailTask
	{
	public:
		typedef std::tr1::shared_ptr< ThumbnailTask > Ptr;
		
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
		: private fwk::Worker< ThumbnailTask::Ptr >
	{
	public:
		ThumbnailCache(const std::string & dir,
					   const fwk::NotificationCenter::Ptr & nc);
		~ThumbnailCache();

		void request(const db::LibFile::ListPtr & fl);
		void requestForFile(const db::LibFile::Ptr & f);

	protected:
		virtual void execute(const  ThumbnailTask::Ptr & task);
	private:
    std::string                                 m_cacheDir;
		std::tr1::weak_ptr<fwk::NotificationCenter> m_notif_center;
	};

}

#endif
