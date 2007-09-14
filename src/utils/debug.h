/*
 * niepce - utils/debug.h
 *
 * Copyright (C) 2007 Hubert Figuiere
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

#ifndef __UTILS_DEBUG_H__
#define __UTILS_DEBUG_H__

#ifdef __GNUC__
// we have too mark this as a system header because otherwise GCC barfs 
// on variadic macros.
#pragma GCC system_header
#endif

namespace utils {

#ifdef DEBUG
#define DBG_OUT(x, ...) \
	utils::dbg_print(x,  __PRETTY_FUNCTION__, ## __VA_ARGS__)
#else
#define DBG_OUT(x, ...) 	
#endif

#define ERR_OUT(x, ...) \
	utils::err_print(x,  __PRETTY_FUNCTION__, ## __VA_ARGS__)


	/** print debug messages. printf format.
	 * NOOP if NDEBUG is defined.
	 * Call with the DBG_OUT macro
	 * @param fmt the formt string, printf style
	 * @param func the func name
	 */
	void dbg_print(const char* fmt, const char* func, ...);

	/** print error message. printf format.
	 * Call with the ERR_OUT macro.
	 * @param fmt the formt string, printf style
	 * @param func the func name
	 */
	void err_print(const char *fmt, const char* func, ...);

}


#endif