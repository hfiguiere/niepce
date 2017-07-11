/*
 * niepce - fwk/base/geometry.cpp
 *
 * Copyright (C) 2007-2017 Hubert Figuiere
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


#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include "fwk/base/debug.hpp"
#include "geometry.hpp"

namespace fwk {

Rect::Rect()
{
    _r[X] = 0;
    _r[Y] = 0;
    _r[W] = 0;
    _r[H] = 0;
}

Rect::Rect(int _x, int _y, int _w, int _h)
{
    _r[X] = _x;
    _r[Y] = _y;
    _r[W] = _w;
    _r[H] = _h;
}


Rect::Rect(const std::string & s) noexcept(false)
{
    std::vector< std::string > v;
    v.reserve(4);
    boost::split(v, s, boost::is_any_of(" "));
    if(v.size() < 4) {
        throw std::bad_cast();
    }
    int i = 0;
    for_each(v.cbegin(), v.cend(),
             [&i, this] (const std::string &_s) {
                 try {
                     _r[i] = std::stoi(_s);
                     i++;
                 }
                 catch(...) {
                     // we likely got an invalid_argument.
                     // Doesn't matter, at that point it is a bad_cast
                     throw std::bad_cast();
                 }
             }
        );
}

Rect & Rect::scale(double _s)
{
    _r[W] = _r[W] * _s;
    _r[H] = _r[H] * _s;
    return *this;
}


Rect Rect::fit_into(const Rect & dest) const
{
    Rect result = *this;

    double in_w = w();
    double in_h = h();
    double scale_w = dest.w() / in_w;
    DBG_OUT("w %d in_w %f", dest.w(), in_w);
    double scale_h = dest.h() / in_h;
    DBG_OUT("h %d in_h %f", dest.h(), in_h);
    DBG_OUT("scale w %f h %f", scale_w, scale_h);
    double _scale = std::min(scale_w, scale_h);

    result.scale(_scale);
    if(scale_w <= scale_h) {
        result._r[W] = dest.w();
    }
    if(scale_w >= scale_h) {
        result._r[H] = dest.h();
    }

    return result;
}


Rect Rect::fill_into(const Rect & dest) const
{
    // the smallest dimension
    Rect result = *this;

    double in_w = w();
    double in_h = h();
    double scale_w = dest.w() / in_w;
    DBG_OUT("w %d in_w %f", dest.w(), in_w);
    double scale_h = dest.h() / in_h;
    DBG_OUT("h %d in_h %f", dest.h(), in_h);
    DBG_OUT("scale w %f h %f", scale_w, scale_h);
    double _scale = std::max(scale_w, scale_h);

    result.scale(_scale);

    if(scale_w >= scale_h) {
        result._r[W] = dest.w();
    }
    if(scale_w <= scale_h) {
        result._r[H] = dest.h();
    }

    return result;
}

}

namespace std {

std::string to_string(const fwk::Rect & r)
{
    return str(boost::format("%1% %2% %3% %4%")
               % r._r[fwk::Rect::X] % r._r[fwk::Rect::Y] % r._r[fwk::Rect::W] % r._r[fwk::Rect::H]);
}

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
