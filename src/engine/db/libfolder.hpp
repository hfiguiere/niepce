/*
 * niepce - eng/db/libfolder.hpp
 *
 * Copyright (C) 2007, 2011 Hubert Figuiere
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



#ifndef __NIEPCE_DB_LIBFOLDER_H__
#define __NIEPCE_DB_LIBFOLDER_H__

#include <string>
#include <list>
#include <tr1/memory>

#include "engine/db/librarytypes.hpp"
#include "fwk/utils/db/iconnectiondriver.hpp"

namespace eng {

class LibFolder
{
public:
    typedef std::tr1::shared_ptr< LibFolder > Ptr;
    typedef std::list< Ptr > List;
    typedef std::tr1::shared_ptr< List > ListPtr;
    typedef enum {
        VIRTUAL_NONE = 0,
        VIRTUAL_TRASH = 1,

        _VIRTUAL_LAST
    } VirtualType;

    LibFolder(library_id_t _id, std::string _name)
        : m_id(_id), m_name(_name)
        , m_locked(false)
        , m_virtual(VIRTUAL_NONE)
        {
        }
    library_id_t id() const
        { return m_id; }
    const std::string & name() const
        { return m_name; }

    bool is_locked() const
        { return m_locked; }
    void set_is_locked(bool _locked)
        { m_locked = _locked; }
    bool is_expanded() const
        { return m_expanded; }
    void set_expanded(bool _exp)
        { m_expanded = _exp; }
    VirtualType virtual_type() const
        { return m_virtual; }
    void set_virtual_type(VirtualType _virtual)
        { m_virtual = _virtual; }

    /** database persistance */
    static const char * read_db_columns();
    static Ptr read_from(const db::IConnectionDriver::Ptr & db);
private:
    library_id_t         m_id;
    std::string m_name;
    bool m_locked;
    bool m_expanded;
    VirtualType m_virtual;
};

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

#endif
