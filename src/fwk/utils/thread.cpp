/*
 * niepce - fwk/utils/thread.cpp
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

#include "fwk/base/debug.hpp"
#include "thread.hpp"

namespace fwk {

Thread::Thread()
    : m_terminated(true),
      m_thrd(nullptr)
{
}

Thread::~Thread()
{
    terminate();
    delete m_thrd;
}

void Thread::start()
{
    DBG_ASSERT(!m_thrd, "Thread already start. Will cancel.");
    if(m_thrd) {
        delete m_thrd;
    }
    m_thrd = new std::thread(&Thread::main, this);
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
