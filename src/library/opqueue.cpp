/*
 * niepce - library/opqueue.cpp
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



#include "opqueue.h"

namespace library {

	OpQueue::OpQueue()
		: m_queue(),
		  m_mutex()
	{
	}


	OpQueue::~OpQueue()
	{
	}

	void
	OpQueue::add(const Op::Ptr &op)
	{
		mutex_t::scoped_lock(m_mutex, true);
		m_queue.push_back(op);
	}


	Op::Ptr OpQueue::pop()
	{
		Op::Ptr elem;
		mutex_t::scoped_lock(m_mutex, true);		
		elem = m_queue.front();
		m_queue.pop_front();
		return elem;
	}


	bool OpQueue::isEmpty() const
	{
		mutex_t::scoped_lock(m_mutex, true);
		return m_queue.empty();
	}

}
