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

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "debug.h"

namespace utils {

	static void _print(const char *prefix, const char *fmt, 
										 const char* func,	va_list marker);


	void dbg_print(const char *fmt, const char* func, ...)
	{
#ifdef DEBUG
#define DEBUG_MSG "DEBUG: "
		va_list marker;
		
		va_start(marker, func);
		// TODO make this atomic
		_print(DEBUG_MSG, fmt, func, marker);

		va_end(marker);
		
#undef DEBUG_MSG
#endif
	}



	void err_print(const char *fmt, const char* func, ...)
	{
#define ERROR_MSG "ERROR: "
		va_list marker;
		
		va_start(marker, func);
		// TODO make this atomic
		_print(ERROR_MSG, fmt, func, marker);

		va_end(marker);
		
#undef ERROR_MSG
	}


	static void _print(const char *prefix, const char *fmt, 
							const char* func,	va_list marker)
	{
		fwrite(prefix, 1, strlen(prefix), stderr);

		if(func) {
			fwrite(func, 1, strlen(func), stderr);
		}

		vfprintf(stderr, fmt, marker);
		fprintf(stderr, "\n");
	}

}
