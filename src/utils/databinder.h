/*
 * niepce - utils/databinder.h
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




#ifndef _UTILS_DATABINDER_H_
#define _UTILS_DATABINDER_H_

#include <vector>

#include <boost/utility.hpp>


namespace utils {

/** @brief the base class for all the data binders */
class DataBinderBase
	: public boost::noncopyable
{
public:
	virtual ~DataBinderBase() 
		{}
};

/** @brief a pool of data binders */
class DataBinderPool
	: private std::vector< DataBinderBase* >
{
public:
	~DataBinderPool();
	/** add a data binder to the pool. the pool will own the pointer */
	void add_binder(DataBinderBase *);
};


}


#endif
