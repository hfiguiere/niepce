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

namespace library {


	Worker::Worker(const std::string & moniker)
		: m_thread(boost::bind(&Worker::main, this, moniker))
	{
	}


	Worker::~Worker()
	{
	}

	/** this is the main loop of the libray worker */
	void Worker::main(const std::string & moniker)
	{
		bool terminated = false;

		m_library = db::Library::Ptr(new db::Library(moniker));

		do {

			while(m_ops.isEmpty()) {
				m_thread.yield();
			}
			
			Op::Ptr op = m_ops.pop();

			execute(op);

		} while(!terminated);

		m_library.reset();
	}


	void Worker::execute(const Op::Ptr & _op)
	{
	}
}
