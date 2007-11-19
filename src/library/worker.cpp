/*
 * niepce - library/worker.cpp
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


#include <boost/bind.hpp>

#include "worker.h"
#include "commands.h"

namespace library {


	Worker::Worker(const std::string & dir, framework::NotificationCenter * nc)
		: m_library(db::Library::Ptr(new db::Library(dir, nc)))
	{
	}


	Worker::~Worker()
	{
	}



	void Worker::schedule(const Op::Ptr & _op)
	{
		OpQueue::mutex_t::scoped_lock(m_ops.mutex(), true);
		bool was_empty = m_ops.isEmpty();
		m_ops.add(_op);
		if(was_empty) {
			start();
		}
	}

	void Worker::start()
	{
		boost::thread * thrd = m_threads.create_thread(
			boost::bind(&Worker::main, this));
		thrd->join();
	}

	/** this is the main loop of the libray worker */
	void Worker::main()
	{
		bool terminated = false;

		do {
			{
				OpQueue::mutex_t::scoped_lock(m_ops.mutex(), true);
				while(m_ops.isEmpty()) {
					return;
				}
			}
			
			Op::Ptr op = m_ops.pop();

			execute(op);

		} while(!terminated);
	}


	void Worker::execute(const Op::Ptr & _op)
	{
		Commands::dispatch(m_library, _op);
	}
}
