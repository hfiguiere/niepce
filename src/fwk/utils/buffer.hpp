/* -*- Mode: C++; indent-tabs-mode:nil; c-basic-offset:4; -*- */

/*
 *This file is part of the Nemiver Project.
 *
 *Nemiver is free software; you can redistribute
 *it and/or modify it under the terms of
 *the GNU General Public License as published by the
 *Free Software Foundation; either version 2,
 *or (at your option) any later version.
 *
 *Nemiver is distributed in the hope that it will
 *be useful, but WITHOUT ANY WARRANTY;
 *without even the implied warranty of
 *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *See the GNU General Public License for more details.
 *
 *You should have received a copy of the
 *GNU General Public License along with Nemiver;
 *see the file COPYING.
 *If not, write to the Free Software Foundation,
 *Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *See COPYRIGHT file copyright information.
 */
#ifndef __NEMIVER_BUFFER_H__
#define __NEMIVER_BUFFER_H__

namespace fwk {

class Buffer {
    char * m_data ;
    unsigned long m_len ;

public:

    Buffer (): m_data (nullptr), m_len (0)
    {}

    Buffer (const char *a_buf, unsigned long a_len)
    {
        m_data = const_cast<char*>(a_buf) ;
        m_len = a_len ;
    }

    Buffer (const Buffer &a_buf) : m_data (a_buf.m_data), m_len (a_buf.m_len)
    {}

    void set
        (const char* a_buf, unsigned long a_len)
    {
        m_data = const_cast<char*> (a_buf);
        m_len = a_len ;
    }

    Buffer& operator= (Buffer &a_buf)
    {
        if (this == &a_buf)
            return *this ;
        m_data = a_buf.m_data ;
        m_len = a_buf.m_len ;
    }

    const char* get_data () const
    {
        return m_data ;
    }

    unsigned long get_len () const
    {
        return m_len;
    }
};//end class Bufer

}//end namespace common

#endif //__NEMIVER_BUFFER_H__

