/*
 * niepce - engine/library/thumbnailnotification.hpp
 *
 * Copyright (C) 2007-2017 Hubert Figuiere
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


#ifndef _LIBRARY_THUMBNAILNOTIFICATION_H__
#define _LIBRARY_THUMBNAILNOTIFICATION_H__

#include "fwk/toolkit/thumbnail.hpp"
#include "engine/db/librarytypes.hpp"

namespace eng {

struct ThumbnailNotification
{
  eng::library_id_t  id;
  int  width;
  int  height;
  fwk::Thumbnail pixmap;
};

}

#endif
