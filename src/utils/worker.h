/*
 * niepce - utils/worker.h
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


#ifndef __UTILS_WORKER_H__
#define __UTILS_WORKER_H__


#include <string>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include "utils/thread.h"
#include "utils/mtqueue.h"

namespace utils {

	/** worker thread for the library */
	template <class T>
	class Worker
		: public Thread
	{
	public:
		Worker();
		typedef MtQueue<T> queue_t;

#ifdef BOOST_AUTO_TEST_MAIN
		queue_t & _tasks() 
			{ return m_tasks; }
#endif
		void schedule(const T & );
		void clear();
	protected:
		virtual void main();

		queue_t      m_tasks;
	private:
		virtual void execute(const T & _op) = 0;
	};

	template <class T>
	Worker<T>::Worker()
		: Thread()
	{
	}

	/** this is the main loop of the libray worker */
	template <class T>
	void Worker<T>::main()
	{
		bool terminated = false;
		
		do 
		{
			{
				// make sure we terminate the thread before we unlock
				// the task queue.
				typename queue_t::mutex_t::scoped_lock(m_tasks.mutex(), true);
				if(m_tasks.empty()) {
					break;
				}
			}
			
			T op = m_tasks.pop();
			execute(op);
		} while(!terminated);
	}

	template <class T>
	void Worker<T>::schedule(const T & _op)
	{
		typename queue_t::mutex_t::scoped_lock(m_tasks.mutex(), true);
		bool was_empty = m_tasks.empty();
		m_tasks.add(_op);
		if(was_empty) {
			start();
		}
	}

	template <class T>
	void Worker<T>::clear()
	{
		m_tasks.clear();
	}

}

#endif
