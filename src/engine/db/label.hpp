/*
 * niepce - engine/db/label.hpp
 *
 * Copyright (C) 2009 Hubert Figuiere
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


#ifndef __NIEPCE_DB_LABEL_HPP__
#define __NIEPCE_DB_LABEL_HPP__

#include <string>
#include <vector>

#include <tr1/memory>

#include "fwk/base/colour.hpp"
#include "engine/db/librarytypes.hpp"

namespace eng {

/** represent a label assigned to a library object
 * There shouldn't be much instances of this.
 */
class Label
{
public:
    typedef std::tr1::shared_ptr<Label> Ptr;
    typedef std::vector<Ptr> List;
    typedef std::tr1::shared_ptr<List> ListPtr;

    Label(library_id_t _id, const std::string & _label, const std::string & _colourstring)
        : m_id(_id), m_label(_label)
        , m_colour(_colourstring)
        {
        }

    library_id_t id() const
        { return m_id; }
    const std::string & label()
        { return m_label; }
    void set_label(const std::string & l)
        { m_label = l; }
    const fwk::RgbColour & colour() const
        { return m_colour; }
    void set_colour(const fwk::RgbColour & c)
        { m_colour = c; }

private:
    library_id_t          m_id;
    std::string      m_label;
    fwk::RgbColour    m_colour;
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
