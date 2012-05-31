/*
 * niepce - fwk/utils/thread.cpp
 *
 * Copyright (C) 2007-2009 Hubert Figuiere
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


#include "thread.hpp"

namespace fwk {


Thread::Thread()
    : m_terminated(true),
      m_thrd(NULL)
{
}


Thread::~Thread()
{
    terminate();
}


void Thread::start()
{
    m_thrd = Glib::Threads::Thread::create(sigc::mem_fun(*this, &Thread::main));
// TODO add this thread to a manager for task management.
//		thrd->join();
}

}
/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:80
  End:
*/
