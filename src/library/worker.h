/*
 * niepce - library/worker.h
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


#ifndef __LIBRARY_WORKER_H__
#define __LIBRARY_WORKER_H__


#include <string>
#include <boost/thread.hpp>

#include "db/library.h"
#include "library/op.h"
#include "library/opqueue.h"

namespace utils {
	class Moniker;
}

namespace library {

	/** worker thread for the library */
	class Worker
	{
	public:
		Worker(const std::string & moniker);
		virtual ~Worker();

		void main(const std::string & moniker);
	private:
		void execute(const Op::Ptr & _op);

		boost::thread    m_thread;
		OpQueue          m_ops;
		db::Library::Ptr m_library;
	};

}


#endif
