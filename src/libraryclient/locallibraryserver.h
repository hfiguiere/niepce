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

#include "library/worker.h"

namespace libraryclient {

	/** @todo get rid of this class altogether */
	class LocalLibraryServer
		: public library::Worker
	{
	public:
		LocalLibraryServer(const std::string & dir)
			: library::Worker(dir)
			{
			}

		
	};

}


#endif
