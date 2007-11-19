/*
 * niepce - library/opqueue.h
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



#ifndef __NIEPCE_LIBRARY_OPQUEUE_H__
#define __NIEPCE_LIBRARY_OPQUEUE_H__


#include "utils/mtqueue.h"
#include "op.h"

namespace library {

	typedef utils::MtQueue< Op::Ptr > OpQueue;

}

#endif
