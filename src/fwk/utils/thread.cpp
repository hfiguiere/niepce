/*
 * niepce - utils/thread.cpp
 *
 * Copyright (C) 2007-2008 Hubert Figuiere
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


#include "thread.h"

namespace utils {


	Thread::Thread()
		: m_terminated(true),
		  m_thrd(NULL)
	{
	}


	Thread::~Thread()
	{
		terminate();
	}



#if 0
	void Thread::schedule(const Op::Ptr & _op)
	{
		OpQueue::mutex_t::scoped_lock lock(m_ops.mutex(), true);
		bool was_empty = m_ops.isEmpty();
		m_ops.add(_op);
		if(was_empty) {
			start();
		}
	}
#endif

	void Thread::start()
	{
		m_thrd = Glib::Thread::create(sigc::mem_fun(*this, &Thread::main), true);
// TODO add this thread to a manager for task management.
//		thrd->join();
	}

}
