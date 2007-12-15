/* -*- Mode: C++; indent-tabs-mode:nil; c-basic-offset:4; -*- */

/*
 *This file is part of the Dodji Common Library Project.
 *
 *Dodji is free software; you can redistribute
 *it and/or modify it under the terms of
 *the GNU General Public License as published by the
 *Free Software Foundation; either version 2,
 *or (at your option) any later version.
 *
 *Dodji is distributed in the hope that it will
 *be useful, but WITHOUT ANY WARRANTY;
 *without even the implied warranty of
 *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *See the GNU General Public License for more details.
 *
 *You should have received a copy of the
 *GNU General Public License along with Dodji;
 *see the file COPYING.
 *If not, write to the Free Software Foundation,
 *Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *See COPYRIGHT file copyright information.
 */
#ifndef __NEMIVER_SQLITE_CNX_DRV_H__
#define __NEMIVER_SQLITE_CNX_DRV_H__

#include <boost/scoped_ptr.hpp>

#include "db/iconnectiondriver.h"
#include "sqlitestatement.h"

struct sqlite3;

namespace db {


namespace sqlite {

class SqliteCnxDrv: public db::IConnectionDriver {
    struct Priv ;
    friend class SqliteCnxMgrDrv ;
    boost::scoped_ptr<Priv> m_priv ;

    friend void boost::checked_delete<SqliteCnxDrv>(SqliteCnxDrv * x);

    //forbid copy
    SqliteCnxDrv (const SqliteCnxDrv &) ;
    SqliteCnxDrv& operator= (const SqliteCnxDrv &) ;

    SqliteCnxDrv (sqlite3 *a_sqlite_handle) ;

    virtual ~SqliteCnxDrv () ;
public:
    sqlite3* sqlite_handle() const;
    const char* get_last_error () const ;

    bool start_transaction () ;

    bool commit_transaction () ;

    bool rollback_transaction () ;

    bool execute_statement (const db::SQLStatement &a_statement) ;

    bool should_have_data () const ;

    bool read_next_row () ;

    virtual int64_t last_row_id();

    unsigned int get_number_of_columns () const ;

    bool get_column_type (uint32_t a_offset,
                          enum db::ColumnType &a_type) const ;

    bool get_column_name (uint32_t a_offset, utils::Buffer &a_name) const ;

    bool get_column_content (uint32_t a_offset,
                             utils::Buffer &a_column_content) const ;

    bool get_column_content (uint32_t a_offset,
                             int32_t &a_column_content) const ;

    bool get_column_content (uint32_t a_offset,
                             int64_t &a_column_content) const ;

    bool get_column_content (uint32_t a_offset,
                             double& a_column_content) const ;

    bool get_column_content (uint32_t a_offset,
                             std::string& a_column_content) const ;

    void close () ;
};//end IConnectionDriver

}//end namespace sqlite
}//end namspace common

#endif //__NEMIVER_SQLITE_CNX_DRV_H__

