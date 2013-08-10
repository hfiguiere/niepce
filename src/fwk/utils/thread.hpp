/*
 * niepce - fwk/utils/thread.hpp
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


#ifndef __FWK_UTILS_THREAD_H__
#define __FWK_UTILS_THREAD_H__


#include <string>
#include <thread>

namespace fwk {

/** thread */
class Thread
{
public:
    Thread();
    virtual ~Thread();

    void terminate()
        { m_terminated = true; }
protected:
    void start();
    virtual void main() = 0;
    volatile bool m_terminated;
    std::thread * thread() const
        { return m_thrd; }
private:
    std::thread *       m_thrd;
};

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
