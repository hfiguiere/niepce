/*
 * niepce - fwk/base/date.rs
 *
 * Copyright (C) 2017 Hubert Figuière
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

use exempi;
use libc;
use std::ptr;
use std::time::{
    SystemTime,
    UNIX_EPOCH
};

// i64 since rusql doesn't like u64.
// Like a UNIX time_t (also i64)
pub type Time = i64;

pub enum Timezone {}

/**
 * Class to deal with ISO8601 string dates as used by XMP.
 * Bonus: with a timezone.
 */
pub struct Date {
    datetime: exempi::DateTime,
    tz: Option<Timezone>,
}


impl Date {

    pub fn new(dt: exempi::DateTime, tz: Option<Timezone>) -> Date {
        Date { datetime: dt, tz: tz }
    }

    pub fn new_from_time(t: Time, tz: Option<Timezone>) -> Date {
        let mut dt = exempi::DateTime::new();
        make_xmp_date_time(t, &mut dt);
        Date { datetime: dt, tz: tz }
    }
    pub fn xmp_date(&self) -> &exempi::DateTime {
        &self.datetime
    }

    pub fn time_value(&self) -> Time {
        let value = self.xmp_date();
        let mut dt = libc::tm {
            tm_sec: value.c.second,
            tm_min: value.c.minute,
            tm_hour: value.c.hour,
            tm_mday: value.c.day,
            tm_mon: value.c.month,
            tm_year: value.c.year - 1900,
            tm_wday: 0, // ignored by mktime()
            tm_yday: 0, // ignored by mktime()
            tm_isdst: -1,
            // this field is supposed to be a glibc extension. oh joy.
            tm_gmtoff: value.c.tz_sign as i64 * ((value.c.tz_hour as i64 * 3600i64) +
                                                 (value.c.tz_minute as i64 * 60i64)),
            tm_zone: ptr::null_mut(),
        };
        let date = unsafe { libc::mktime(&mut dt) };
        //DBG_ASSERT(date != -1, "date is -1");

        date
    }

    pub fn now() -> Time {
        let time: i64 = if let Ok(t) = SystemTime::now().duration_since(UNIX_EPOCH) {
            t.as_secs()
        } else {
            0
        } as i64;
        time
    }

}

impl ToString for Date {
    fn to_string(&self) -> String {
        format!("{0:04}:{1:02}:{2:02} {3:02}:{4:02}:{5:02} {6}{7:02}{8:02}",
                self.datetime.c.year, self.datetime.c.month,
                self.datetime.c.day, self.datetime.c.hour,
                self.datetime.c.minute, self.datetime.c.second,
                match self.datetime.c.tz_sign {
                    exempi::XmpTzSign::West => '-',
                    _ => '+'
                },
                self.datetime.c.tz_hour, self.datetime.c.tz_minute)
    }
}

/**
 * Fill the XmpDateTime %xmp_dt from a %t
 * @return false if gmtime_r failed.
 */
pub fn make_xmp_date_time(t: Time, xmp_dt: &mut exempi::DateTime) -> bool {
    let pgmt = unsafe { libc::gmtime(&t) };
    if pgmt.is_null() {
        return false;
    }
    unsafe {
        xmp_dt.c.year = (*pgmt).tm_year + 1900;
        xmp_dt.c.month = (*pgmt).tm_mon + 1;
        xmp_dt.c.day = (*pgmt).tm_mday;
        xmp_dt.c.hour = (*pgmt).tm_hour;
        xmp_dt.c.minute = (*pgmt).tm_min;
        xmp_dt.c.second = (*pgmt).tm_sec;
    }
    xmp_dt.c.tz_sign = exempi::XmpTzSign::UTC;
    xmp_dt.c.tz_hour = 0;
    xmp_dt.c.tz_minute = 0;
    xmp_dt.c.nano_second = 0;

    return true;
}

#[cfg(test)]
mod test {
    use super::Date;

    #[test]
    fn test_date() {
        let d = Date::new_from_time(0, None);
        let xmp_dt = d.xmp_date();

        assert_eq!(xmp_dt.c.year, 1970);
        assert_eq!(xmp_dt.c.month, 1);
        assert_eq!(xmp_dt.c.day, 1);

        assert_eq!(d.to_string(), "1970:01:01 00:00:00 +0000");
    }

}
