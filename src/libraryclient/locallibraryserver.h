/*
 * niepce - libraryclient/locallibraryserver.h
 *
 * Copyright (C) 2007 Hubert Figuiere
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  
 * 02110-1301, USA
 */



#ifndef _LIBRARYCLIENT_LOCALLIBRARYSERVER_H_
#define _LIBRARYCLIENT_LOCALLIBRARYSERVER_H_

#include "fwk/utils/worker.h"
#include "library/op.h"
#include "db/library.h"

namespace libraryclient {

	class LocalLibraryServer
		: public utils::Worker< library::Op::Ptr >
	{
	public:
		/** create the local server for the library whose dir is specified */
		LocalLibraryServer(const std::string & dir, 
						   const framework::NotificationCenter::Ptr & nc)
			: utils::Worker< library::Op::Ptr >()
			, m_library(db::Library::Ptr(new db::Library(dir, nc)))
			{
			}

	protected:
		virtual void execute(const library::Op::Ptr & _op);

	private:
		db::Library::Ptr m_library;
	};

}


#endif
