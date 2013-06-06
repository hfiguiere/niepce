/*
 * niepce - fwk/base/geometry.h
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


#ifndef __FWK_BASE_GEOMETRY_H__
#define __FWK_BASE_GEOMETRY_H__

#include <exception>
#include <array>
#include <string>

#include <boost/lexical_cast.hpp>

namespace fwk {

class Rect
{
public:
    Rect();
    Rect(int x, int y, int w, int h);
    /** build a Rect from a string
     * @param s string whose format is "x y w h" as decimal ints.
     * @throw a std::bad_cast exception if there is not 4 element
     * or if one of them is not an int.
     */
    Rect(const std::string & s)
        throw(std::bad_cast);

    int x() const
        { return _r[X]; }
    int y() const
        { return _r[Y]; }
    int w() const
        { return _r[W]; }
    int h() const
        { return _r[H]; }
    /** convert to a string in the same format as
     * accepted by the %Rect(const std::string & s) constructor.
     */
    std::string to_string() const;

    Rect & scale(double _s);

    /** fit into destination. */
    Rect fit_into(const Rect & dest) const;
    /** fill into desitnation */
    Rect fill_into(const Rect & dest) const;

    bool operator==(const Rect & r) const
        { return _r == r._r; }
private:
    /** the indices */
    enum {
        X = 0,
        Y,
        W,
        H
    };
    std::array<int, 4> _r;
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
