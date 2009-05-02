/* -*- Mode: C++; indent-tabs-mode:nil; c-basic-offset: 4-*- */

/*Copyright (c) 2005-2006 Dodji Seketeli
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit
 * persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS",
 * WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */
#ifndef __NEMIVER_EXCEPTION_H__
#define __NEMIVER_EXCEPTION_H__

#include <stdexcept>
#include <string>
#include <iostream>

#include "logstreamutils.hpp"

namespace utils {

class Exception: public std::runtime_error
{
public:
    Exception (const char* a_reason) ;
    Exception (const std::string &a_reason) ;
    Exception (const Exception &a_other) ;
    Exception (const std::exception &) ;
    Exception& operator= (const Exception &a_other) ;
    virtual ~Exception () throw ();
    const char* what () const throw ();
};//class Exception

#define THROW_IF_FAIL(a_cond) \
if (!(a_cond)) { \
LOG_EXCEPTION (std::string("condition (") + #a_cond + ") failed; raising exception" ) ;\
throw utils::Exception \
    (std::string ("Assertion failed: ") + #a_cond)  ;\
}

#define THROW_IF_FAIL2(a_cond, a_reason) \
if (!(a_cond)) { \
LOG_EXCEPTION (std::string("condition (") + #a_cond + ") failed; raising exception " + a_reason);\
throw utils::Exception (a_reason)  ;\
}

#define THROW_IF_FAIL3(a_cond, type, a_reason) \
if (!(a_cond)) { \
LOG_EXCEPTION (std::string("condition (") + #a_cond + ") failed; raising exception " + #type + \
":  " + a_reason ) ; throw type (a_reason)  ;\
}

#define ABORT_IF_FAIL(a_cond, a_reason) \
if (!(a_cond)) { \
LOG_EXCEPTION ("condition (" << #a_cond << ") failed; raising exception " << a_reason <<"\n"); abort();\
}

#define THROW(a_reason) \
LOG_EXCEPTION (std::string("raised exception: ") + std::string(a_reason)); \
throw utils::Exception (std::string (a_reason))  ;

#define THROW_EMPTY \
LOG_EXCEPTION ("raised empty exception " << endl) ; \
throw ;

#define THROW_EXCEPTION(type, message) \
LOG_EXCEPTION ("raised " << #type << ": "<< message<< "\n") ; \
throw type (message) ;

#define TRACE_EXCEPTION(exception) \
LOG_EXCEPTION ("catched exception: " << exception.what () << "\n")

#define RETHROW_EXCEPTION(exception) \
LOG_EXCEPTION ("catched and rethrowing exception: " << exception.what() << "\n")

#define RETURN_VAL_IF_FAIL(expression, value) \
if (!(expression)) { \
ERR_OUT("assertion %s  failed. Returning %s", #expression, #value); \
return value ; \
}

#define RETURN_IF_FAIL(expression) \
if (!(expression)) { \
ERR_OUT ("assertion %s failed. Returning.", #expression); \
return ; \
}

#ifndef NEMIVER_TRY
#define NEMIVER_TRY try {
#endif

#ifndef NEMIVER_CATCH_NOX
#define NEMIVER_CATCH_NOX \
} catch (Glib::Exception &e) { \
    LOG_ERROR (e.what ()) ; \
} catch (std::exception &e) { \
    LOG_ERROR (e.what ()) ; \
} catch (...) { \
    LOG_ERROR ("An unknown error occured") ; \
}
#endif

#ifndef NEMIVER_CATCH_AND_RETURN_NOX
#define NEMIVER_CATCH_AND_RETURN_NOX(a_value) \
} catch (Glib::Exception &e) { \
    LOG_ERROR (e.what ()) ; \
    return a_value ; \
} catch (std::exception &e) { \
    LOG_ERROR (e.what ()) ; \
    return a_value ; \
} catch (...) { \
    LOG_ERROR ("An unknown error occured") ; \
    return a_value ; \
}
#endif

}//end namespace common

#endif //__NEMIVER_EXCEPTION_H__
