/*
 * niepce - fwk/utils/worker.h
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


#ifndef __FWK_UTILS_WORKER_H__
#define __FWK_UTILS_WORKER_H__


#include <string>

#include "fwk/utils/thread.hpp"
#include "fwk/utils/mtqueue.hpp"

namespace fwk {

/** worker thread for the library */
template <class T>
class Worker
    : public Thread
{
public:
    Worker();
    virtual ~Worker();
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
    Glib::Mutex m_q_mutex;
    Glib::Cond m_wait_cond;
};

template <class T>
Worker<T>::Worker()
    : Thread()
{
    start();
}

template <class T>
Worker<T>::~Worker()
{
    m_tasks.clear();
    {
        Glib::Mutex::Lock lock(m_q_mutex);
        m_terminated = true;
        m_wait_cond.broadcast();
    }
    thread()->join();
}

/** this is the main loop of the libray worker */
template <class T>
void Worker<T>::main()
{
    m_terminated = false;
    
    do {
        T op;
        {
            // make sure we terminate the thread before we unlock
            // the task queue.
            Glib::Mutex::Lock lock(m_q_mutex);
            if(!m_tasks.empty()) {
                op = m_tasks.pop();
            }
        }
        // depending on T this might blow
        if(op) {
            execute(op);
        }

        Glib::Mutex::Lock lock(m_q_mutex);
        if(m_tasks.empty()) {
            m_wait_cond.wait(m_q_mutex);
        }
    } while(!m_terminated);
}

template <class T>
void Worker<T>::schedule(const T & _op)
{
    Glib::Mutex::Lock lock(m_q_mutex);
    m_tasks.add(_op);
    m_wait_cond.broadcast();
}

template <class T>
void Worker<T>::clear()
{
    Glib::Mutex::Lock lock(m_q_mutex);
    m_tasks.clear();
}

}

#endif
/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:80
  End:
*/
