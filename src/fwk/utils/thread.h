/*
 * niepce - utils/thread.h
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


#ifndef __UTILS_THREAD_H__
#define __UTILS_THREAD_H__


#include <string>

#include <glibmm/thread.h>


namespace utils {

	/** thread */
	class Thread
	{
	public:
		/** create the worker for the library whose dir is specified */
		Thread();
		virtual ~Thread();

		void terminate()
			{ m_terminated = true; }
	protected:
		void start();
		virtual void main() = 0;
		volatile bool m_terminated;
	private:
    Glib::Thread *       m_thrd;
	};

}


#endif
