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
#ifndef __NEMIVER_I_CONNECTION_MANAGER_DRIVER_H__
#define __NEMIVER_I_CONNECTION_MANAGER_DRIVER_H__

#include <string>

#include "iconnectiondriver.hpp"

namespace db {

class IConnectionManagerDriver ;

class DBDesc {
    std::string m_type ;
    std::string m_host ;
    unsigned long m_port ;
    std::string m_name ;

public:
    DBDesc ()
    {}
    DBDesc (const std::string &a_host,
            const unsigned long &a_port,
            const std::string &a_db_name) 
        : m_host(a_host),
          m_port(a_port),
          m_name(a_db_name)
        {
        }

    const std::string host () const
    {
        return m_host;
    }
    unsigned long port () const
    {
        return m_port;
    }
    const std::string name () const
    {
        return m_name;
    }
    const std::string type () const
    {
        return m_type;
    }

    void set_host (const std::string &a_host)
    {
        m_host = a_host;
    }
    void set_port (const unsigned long &a_port)
    {
        m_port = a_port;
    }
    void set_name (const std::string &a_name)
    {
        m_name = a_name;
    }
    void set_type (const std::string &a_type)
    {
        m_type = a_type;
    }
};//end class DBDesc

class  IConnectionManagerDriver {
public:
    typedef std::shared_ptr<IConnectionManagerDriver> Ptr;
    IConnectionManagerDriver()
    {
    }

    virtual ~IConnectionManagerDriver () {};
    virtual IConnectionDriver::Ptr connect_to_db(const DBDesc &a_db_desc,
                                                 const std::string &a_user,
                                                 const std::string &a_pass) = 0;
};//end class IConnectionManagerDriver

}//end namespace common

#endif //__NEMIVER_I_CONNECTION_MANAGER_DRIVER_H__

