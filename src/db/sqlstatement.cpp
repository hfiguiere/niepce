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

#include "utils/exception.h"
#include "utils/logstreamutils.h"
#include "sqlstatement.h"

#include <string>


namespace db {

struct SQLStatementPriv
{
    std::string sql_string ;
};

const std::string&
SQLStatement::to_string () const
{
    THROW_IF_FAIL (m_priv) ;
    return m_priv->sql_string ;
}

std::string
SQLStatement::escape_string (const std::string &a_sql_string)
{
    std::string out_string ;

    for(std::string::const_iterator iter = a_sql_string.begin();
        iter != a_sql_string.end(); ++iter) {
        char c = *iter;
        out_string.append (1,c);
        if (c == '\'') {
            out_string.append (1,c);
        }
    }
    return out_string ;
}

bool 
SQLStatement::bind(int idx, const std::string & text)
{
    m_bindings.push_back(binder_t(COLUMN_TYPE_STRING, idx, 
                                  boost::any(text)));
    return true;
}


bool 
SQLStatement::bind(int idx, const utils::Buffer & blob)
{
    m_bindings.push_back(binder_t(COLUMN_TYPE_BLOB, idx, 
                                      boost::any(&blob)));
    return true;
}



SQLStatement::SQLStatement (const std::string &a_sql_string)
{
    m_priv = new SQLStatementPriv ;
    m_priv->sql_string = a_sql_string ;
}

SQLStatement::SQLStatement (const boost::format & _format)
{
    m_priv = new SQLStatementPriv ;
    m_priv->sql_string = str(_format);
}


SQLStatement::SQLStatement (const SQLStatement &a_statement)
{
    m_priv = new SQLStatementPriv ;
    m_priv->sql_string = a_statement.m_priv->sql_string ;
}

SQLStatement&
SQLStatement::operator= (const SQLStatement &a_statement)
{
    if (this == &a_statement) {
        return *this ;
    }
    m_priv->sql_string = a_statement.m_priv->sql_string ;
    return *this ;
}

SQLStatement::~SQLStatement ()
{
    if (!m_priv)
        return ;
    delete m_priv ;
}

SQLStatement::operator const char* () const
{
    return to_string().c_str() ;
}

std::ostream&
operator<< (std::ostream &a_os, const SQLStatement &a_s)
{
    a_os << a_s.to_string() ;
    return a_os;
}

}//end db


