/*
 * niepce - fwk/utils/mtqueue.h
 *
 * Copyright (C) 2007-2013 Hubert Figuiere
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



#ifndef __FWK_UTILS_MTQUEUE_H__
#define __FWK_UTILS_MTQUEUE_H__

#include <deque>
#include <glibmm/threads.h>

namespace fwk {

	/** This class implement a simple deque protected 
	 * by mutexes.
	 *
	 * It is not STL compliant.
	 */
	template < class T >
	class MtQueue
	{
	public:
	  typedef Glib::Threads::RecMutex mutex_t;
		typedef T              value_type;

		MtQueue();
		virtual ~MtQueue();

		void add(const T &);
		void add(T & ); // for unique_ptr<>
		T pop();
		bool empty() const;
		void clear();
		mutex_t & mutex() const
			{ return m_mutex; }
	private:
		std::deque<T> m_queue;
		mutable mutex_t     m_mutex;
	};


	template < class T >
	MtQueue<T>::MtQueue()
		: m_queue(),
		  m_mutex()
	{
	}

	template < class T >
	MtQueue<T>::~MtQueue()
	{
		mutex_t::Lock lock(m_mutex);		
	}

	template < class T > void
	MtQueue<T>::add(const T &op)
	{
		mutex_t::Lock lock(m_mutex);
		m_queue.push_back(op);
	}

	// for unique_ptr<>
	template < class T > void
	MtQueue<T>::add(T &op)
	{
		mutex_t::Lock lock(m_mutex);
		m_queue.push_back(std::move(op));
	}

	template < class T >
	T MtQueue<T>::pop()
	{
		T elem;
		mutex_t::Lock lock(m_mutex);
		elem = std::move(m_queue.front());
		m_queue.pop_front();
		return elem;
	}

	template < class T >
	bool MtQueue<T>::empty() const
	{
		mutex_t::Lock lock(m_mutex);
		return m_queue.empty();
	}

	template < class T >
	void MtQueue<T>::clear() 
	{
		mutex_t::Lock lock(m_mutex);
		m_queue.clear();
	}
}

#endif
