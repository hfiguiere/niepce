/*
 * niepce - utils/databinder.cpp
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

#include "debug.hpp"
#include "databinder.hpp"

namespace utils {

DataBinderPool::~DataBinderPool()
{
	for(iterator iter = begin(); iter != end(); iter++)
	{
		boost::checked_delete(*iter);
	}
}


void DataBinderPool::add_binder(DataBinderBase *binder)
{
	push_back(binder);
}

void DataBinderPool::destroy(DataBinderPool *pool)
{
	delete pool;
}


}
