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

#include "fwk/utils/worker.hpp"
#include "engine/library/op.hpp"
#include "engine/db/library.hpp"
namespace libraryclient {

	class LocalLibraryServer
		: public fwk::Worker< eng::Op::Ptr >
	{
	public:
		/** create the local server for the library whose dir is specified */
		LocalLibraryServer(const std::string & dir, 
						   const fwk::NotificationCenter::Ptr & nc)
			: fwk::Worker< eng::Op::Ptr >()
			, m_library(eng::Library::Ptr(new eng::Library(dir, nc)))
			{
			}
		bool ok() const
			{
				return m_library && m_library->ok();
			}
	protected:
		virtual void execute(const eng::Op::Ptr & _op);

	private:
		eng::Library::Ptr m_library;
	};

}


#endif
