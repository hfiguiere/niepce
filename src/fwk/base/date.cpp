/*
 * niepce - fwk/base/date.hpp
 *
 * Copyright (C) 2012 Hubert Figuiere
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

#include <stdio.h>
#include <time.h>

#include "date.hpp"
#include "debug.hpp"
#include "rust.hpp"

// rust glue
extern "C" {
void fwk_date_delete(fwk::Date*);
char* fwk_date_to_string(const fwk::Date*);
}

namespace fwk {

DatePtr date_wrap(fwk::Date* date)
{
    return DatePtr(date, fwk_date_delete);
}

std::string date_to_string(const Date* d)
{
    DBG_ASSERT(d, "d is nullptr");
    if (!d) {
        return "";
    }
    char* p = fwk_date_to_string(d);
    std::string s(p);
    rust_cstring_delete(p);
    return s;
}

#if 0
time_t make_time_value(const Date & d)
{
    time_t date = 0;

    const XmpDateTime & value = d.xmp_date();
    struct tm dt;
    dt.tm_sec = value.second;
    dt.tm_min = value.minute;
    dt.tm_hour = value.hour;
    dt.tm_mday = value.day;
    dt.tm_mon = value.month;
    dt.tm_year = value.year - 1900;
    dt.tm_isdst = -1;
    // this field is supposed to be a glibc extension. oh joy.
    dt.tm_gmtoff = value.tzSign * ((value.tzHour * 3600) +
                                   (value.tzMinute * 60));
    date = mktime(&dt);
    DBG_ASSERT(date != -1, "date is -1");

    return date;
}
#endif

bool make_xmp_date_time(time_t t, XmpDateTime& xmp_dt)
{
    struct tm gmt;
    struct tm* pgmt = gmtime_r(&t, &gmt);
    DBG_ASSERT(pgmt == &gmt, "gmtime failed");

    if (!pgmt) {
        return false;
    }

    xmp_dt.year = pgmt->tm_year + 1900;
    xmp_dt.month = pgmt->tm_mon + 1;
    xmp_dt.day = pgmt->tm_mday;
    xmp_dt.hour = pgmt->tm_hour;
    xmp_dt.minute = pgmt->tm_min;
    xmp_dt.second = pgmt->tm_sec;
    xmp_dt.tzSign = 0;
    xmp_dt.tzHour = 0;
    xmp_dt.tzMinute = 0;
    xmp_dt.nanoSecond = 0;

    return true;
}

#if 0
Date::Date(const XmpDateTime& dt, const Timezone* tz)
    : m_datetime(dt)
    , m_tz(tz)
{
}

Date::Date(const time_t dt, const Timezone* tz)
    : m_tz(tz)
{
    make_xmp_date_time(dt, m_datetime);
}

std::string Date::to_string() const
{
    char buffer[256];

    snprintf(buffer, 256, "%.4u:%.2u:%.2d %.2u:%.2u:%.2u %c%.2u%.2u",
             m_datetime.year, m_datetime.month,
             m_datetime.day, m_datetime.hour,
             m_datetime.minute, m_datetime.second,
             m_datetime.tzSign >= 0 ? '+' : '-',
             m_datetime.tzHour, m_datetime.tzMinute);

    return buffer;
}
#endif
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
