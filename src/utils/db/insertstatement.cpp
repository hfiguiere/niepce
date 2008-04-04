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

#include "utils/logstreamutils.h"
#include "utils/exception.h"
#include "insertstatement.h"

namespace db {

struct InsertStatementPriv {
    std::string table_name ;
    ColumnList columns ;
    std::string string_repr ;
};//end InsertStatementPriv

InsertStatement::InsertStatement (const std::string &a_table_name,
                                  ColumnList &a_columns)
{
    m_priv = new InsertStatementPriv ;
    m_priv->table_name = a_table_name ;
    m_priv->columns = a_columns ;
}

InsertStatement::~InsertStatement ()
{
    if (m_priv) {
        delete m_priv ;
        m_priv = NULL ;
    }
}

void
InsertStatement::set (const std::string &a_table_name, ColumnList &a_columns)
{
    m_priv->table_name = a_table_name ;
    m_priv->columns = a_columns ;
    m_priv->string_repr = "" ;
}

const std::string&
InsertStatement::to_string () const
{
    std::string str ;

//    LOG_FUNCTION_SCOPE_NORMAL_DD ;

    if (m_priv->string_repr == "") {
        RETURN_VAL_IF_FAIL (m_priv->table_name != "", m_priv->string_repr) ;
        RETURN_VAL_IF_FAIL (m_priv->columns.size () != 0,
                            m_priv->string_repr) ;
        RETURN_VAL_IF_FAIL (m_priv->columns.size ()
                            == m_priv->columns.size (),
                            m_priv->string_repr) ;
        str = "INSERT INTO " + m_priv->table_name + "( " ;
        std::string col_names, col_values ;
        for (ColumnList::iterator it = m_priv->columns.begin () ;
                it != m_priv->columns.end () ;
                ++it) {
            if (col_names.size ()) {
                col_names += ", " ;
                col_values += ", " ;
            }
            col_names += it->get_name () ;
            if (it->get_auto_increment ()) {
                col_values += "null" ;//for mysql and sqlite
                //TODO: find an elegant way to consider
                //the case of other databases.
            } else {
                col_values += "\'" + it->get_value () + "\'" ;
            }
        }
        str += col_names + ") VALUES (" + col_values + ")";
        m_priv->string_repr = str ;
    }
    return m_priv->string_repr ;
}

const ColumnList&
InsertStatement::get_columns () const
{
    return m_priv->columns ;
}

const std::string&
InsertStatement::get_table_name () const
{
    return m_priv->table_name ;
}


}//end namespace common

