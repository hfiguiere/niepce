/*
 * niepce - db/storage.h
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


#ifndef __NIEPCE_LIBRARY_STORAGE_H__
#define __NIEPCE_LIBRARY_STORAGE_H__

#include <tr1/memory>

#include "engine/db/keyword.h"

namespace db {

	/** @brief the interface for a storage */
	class Storage
	{
	public:
		typedef std::tr1::shared_ptr<Storage> Ptr;

		virtual ~Storage();

		virtual bool fetchKeywordsForFile(int file, Keyword::IdList &keywords) = 0;
	};

}

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/

#endif
