/*
 * niepce - fwk/base/date.hpp
 *
 * Copyright (C) 2012-2013 Hubert Figuiere
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

#ifndef __FWK_BASE_DATE_HPP
#define __FWK_BASE_DATE_HPP

#include <string>

#include <exempi/xmp.h>

namespace fwk {

class Timezone;

XmpDateTime& make_xmp_date_time(time_t t, XmpDateTime& xmp_dt);

/**
 * Class to deal with ISO8601 string dates as used by XMP.
 * Bonus: with a timezone.
 */
class Date
{
public:
    Date(const XmpDateTime& dt, const Timezone* tz = nullptr);
    Date(const time_t dt, const Timezone* tz = nullptr);

    std::string to_string() const;
    const XmpDateTime& xmp_date() const
        { return m_datetime; }

private:
    XmpDateTime m_datetime;
    // weak pointer.
    const Timezone* m_tz;
};

time_t make_time_value(const Date &);

}

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
#endif
