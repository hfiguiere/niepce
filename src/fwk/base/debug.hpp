/*
 * niepce - fwk/base/debug.h
 *
 * Copyright (C) 2007-2009 Hubert Figuiere
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

#pragma once

#ifndef __FWK_BASE_DEBUG_H__
#define __FWK_BASE_DEBUG_H__

#ifndef DEBUG
#include <assert.h>
#endif

#ifdef __GNUC__
// we have too mark this as a system header because otherwise GCC barfs
// on variadic macros.
#pragma GCC system_header
#endif

namespace fwk {

#ifdef DEBUG

#define DBG_OUT(x, ...) \
	fwk::dbg_print(x,  __FUNCTION__, ## __VA_ARGS__)
#define DBG_ASSERT(cond, reason)	\
	fwk::dbg_assert(cond, #cond, __FUNCTION__, __FILE__, __LINE__, reason)
#define DBG_DTOR \
    DBG_OUT("dtor %p", this)

#else

#define DBG_OUT(x, ...)
#define DBG_ASSERT(cond, reason)	\
	assert(cond)
#define DBG_DTOR

#endif

#define ERR_OUT(x, ...) \
    fwk::err_print(x,  __FUNCTION__, __FILE__, __LINE__, ## __VA_ARGS__)


/** print debug messages. printf format.
 * NOOP if DEBUG is not defined.
 * Call with the DBG_OUT macro
 * @param fmt the formt string, printf style
 * @param func the func name
 */
void dbg_print(const char* fmt, const char* func, ...)
    __attribute__ ((format (printf, 1, 3)));

/** assert
 * @param condvalue the value of the assert, true, assert
 * @param cond the text of the condition
 * @param func the function it happens in __FUNCTION__
 * @param filen the file name __FILE__
 * @param linen the line number __LINE__
 * @param reason the reason of the assert
 */
void dbg_assert(bool condvalue, const char* cond, const char* func,
                const char* filen,
                int linen, const char* reason);

/** print error message. printf format.
 * Call with the ERR_OUT macro.
 * @param fmt the formt string, printf style
 * @param func the func name
 */
void err_print(const char *fmt, const char* func, const char* filen,
               int linen, ...)
    __attribute__ ((format (printf, 1, 5)));

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
