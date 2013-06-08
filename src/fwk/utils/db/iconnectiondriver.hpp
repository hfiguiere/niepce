/* -*- Mode: C++; indent-tabs-mode:nil; c-basic-offset: 4  -*- */

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
#ifndef __NEMIVER_I_CONNECTION_DRIVER_H__
#define __NEMIVER_I_CONNECTION_DRIVER_H__

#include <stdint.h>

#include <memory>
#include <functional>

namespace fwk {
class Buffer ;
//class UString ;
}

namespace db {

enum ColumnType {
    COLUMN_TYPE_INT=1,
    COLUMN_TYPE_BIG_INT=1<<1,
    COLUMN_TYPE_DECIMAL=1<<2,
    COLUMN_TYPE_DOUBLE=1<<3,
    COLUMN_TYPE_DATETIME=1<<3,
    COLUMN_TYPE_STRING=1<<4,
    COLUMN_TYPE_BLOB=1<<5,
    COLUMN_TYPE_UNKNOWN=1<<30// should be last
};

class SQLStatement;

class IConnectionDriver {

public:
    typedef std::shared_ptr<IConnectionDriver> Ptr;

    typedef std::function<void (void)> f0_t;

    virtual ~IConnectionDriver ()
    {}

    virtual void close () = 0;

    virtual const char* get_last_error () const = 0;

    virtual bool start_transaction () = 0 ;

    virtual bool commit_transaction () = 0 ;

    virtual bool rollback_transaction () = 0 ;

    virtual bool execute_statement (const SQLStatement &a_statement) = 0;
    
    virtual bool create_function0(const std::string & name, const f0_t & f) = 0;

    virtual bool should_have_data () const = 0;

    virtual bool read_next_row () = 0;

    virtual int64_t last_row_id() = 0;

    virtual unsigned int get_number_of_columns () const = 0;

    virtual bool get_column_type (uint32_t a_offset,
                                  enum ColumnType &) const = 0;

    virtual bool get_column_name (uint32_t a_offset,
                                  fwk::Buffer &a_name) const = 0;

    virtual bool get_column_content (uint32_t a_offset,
                                     fwk::Buffer &a_column_content) const = 0;

    virtual bool get_column_content (uint32_t a_offset,
                                     int32_t &a_column_content) const = 0;

    virtual bool get_column_content (uint32_t a_offset,
                                     int64_t &a_column_content) const = 0;

    virtual bool get_column_content (uint32_t a_offset,
                                     double& a_column_content) const = 0;

    virtual bool get_column_content (uint32_t a_offset,
                                     std::string& a_column_content) const = 0;

};//end IConnectionDriver


}//end common

#endif //__NEMIVER_I_CONNECTION_DRIVER_H__

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
