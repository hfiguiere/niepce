/*
 * niepce - fwk/base/debug.cpp
 *
 * Copyright (C) 2007-2012 Hubert Figuiere
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
#include <string.h>
#include <stdarg.h>

#include <pthread.h>

#if defined(NDEBUG)
#define _SAVENDEBUG NDEBUG
#undef NDEBUG
#endif
// we make sure assert is always defined.
#include <assert.h>
// but we need to save the state.
#if defined(_SAVENDEBUG)
#define NDEBUG _SAVENDEBUG
#endif

#include "debug.hpp"

extern "C" void dbg_assert_raised()
{
    // set a breakpoint here.
}

namespace fwk {

	static void _vprint(const char *prefix, const char *fmt, 
										 const char* func,	va_list marker);
static void _v2print(const char *prefix, const char* filen, int linen,
                     const char *fmt, const char* func, va_list marker);
	static void _print(const char *prefix, const char *fmt, 
					   const char* func, ...);

	void dbg_print(const char *fmt, const char* func, ...)
	{
#ifdef DEBUG
#define DEBUG_MSG "DEBUG: "
		va_list marker;
		
		va_start(marker, func);
		// TODO make this atomic
		_vprint(DEBUG_MSG, fmt, func, marker);

		va_end(marker);
		
#undef DEBUG_MSG
#endif
	}


	/** assert 
	 * 
	 */
	void dbg_assert(bool condvalue, const char* cond, 
                        const char* func, const char* filen,
                        int linen, const char* reason)
	{
            if(!condvalue) {
                _print("ASSERT: ", "[%s] %s:%d %s", func, cond, filen, linen, reason);
                dbg_assert_raised();
            }
	}


void err_print(const char *fmt, const char* func, const char* filen,
               int linen, ...)
{
#define ERROR_MSG "ERROR %s:%d: "
    va_list marker;

    va_start(marker, linen);
    // TODO make this atomic
    _v2print(ERROR_MSG, filen, linen, fmt, func, marker);

    va_end(marker);

#undef ERROR_MSG
}


	static void _print(const char *prefix, const char *fmt, 
							const char* func, ...)
	{
		va_list marker;
		
		va_start(marker, func);

		_vprint(prefix, fmt, func, marker);

		va_end(marker);
	}

	static void _vprint(const char *prefix, const char *fmt, 
							const char* func,	va_list marker)
	{
		char buf[128];
		snprintf(buf, 128, "(0x%lx) ", (unsigned long)pthread_self());
		fwrite(buf, 1, strlen(buf), stderr);
		fwrite(prefix, 1, strlen(prefix), stderr);

		if(func) {
			fwrite(func, 1, strlen(func), stderr);
			fwrite(" - ", 1, 3, stderr);
		}

		vfprintf(stderr, fmt, marker);
		fprintf(stderr, "\n");
	}

static void _v2print(const char* prefix, const char* filen, int linen,
                     const char* fmt, const char* func, va_list marker)
{
    char buf[128];
    snprintf(buf, 128, "(0x%lx) ", (unsigned long)pthread_self());
    fwrite(buf, 1, strlen(buf), stderr);

    fprintf(stderr, prefix, filen, linen);

    if(func) {
        fwrite(func, 1, strlen(func), stderr);
        fwrite(" - ", 1, 3, stderr);
    }

    vfprintf(stderr, fmt, marker);
    fprintf(stderr, "\n");
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
