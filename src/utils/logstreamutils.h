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
#ifndef __NMV_LOG_STREAM_UTILS_H__
#define __NMV_LOG_STREAM_UTILS_H__

#include <cassert>
#include <cstdlib>
#include <iostream>

#include "utils/debug.h"

#define NMV_DEFAULT_DOMAIN __FILE__


#ifndef PRETTY_FUNCTION_NAME_
#ifdef DEBUG
#define PRETTY_FUNCTION_NAME_ __ASSERT_FUNCTION
#else
// non portable construct, who the hell care.
#define PRETTY_FUNCTION_NAME_ __PRETTY_FUNCTION__
#endif
#endif

#ifndef HERE
#define HERE PRETTY_FUNCTION_NAME_ << ":" <<__FILE__<< ":" << __LINE__ << ":"
#endif


#ifndef LOG_STREAM
#define LOG_STREAM std::cerr
#endif

#ifndef LOG_MARKER_INFO
#define LOG_MARKER_INFO "|I|"
#endif

#ifndef LOG_MARKER_ERROR
#define LOG_MARKER_ERROR "|E|"
#endif

#ifndef LOG_MARKER_EXCEPTION
#define LOG_MARKER_EXCEPTION "|X|"
#endif

#ifndef LOG_LEVEL_NORMAL___
#define LOG_LEVEL_NORMAL___ " "
#endif

#ifndef LOG_LEVEL_VERBOSE___
#define LOG_LEVEL_VERBOSE___ " "
#endif

#ifndef LOG
#define LOG(message) \
LOG_STREAM << LOG_LEVEL_NORMAL___ << LOG_MARKER_INFO << HERE << message << nemiver::common::endl
#endif

#ifndef MESSAGE
#define MESSAGE(message) \
LOG_STREAM << LOG_LEVEL_NORMAL___ << LOG_MARKER_INFO << message << nemiver::common::endl
#endif

#ifndef LOG_D
#define LOG_D(message, domain) \
LOG_STREAM << domain << " - " << message << std::endl;
#endif

#ifndef LOG_DD
#define LOG_DD(message) LOG_D(message, NMV_DEFAULT_DOMAIN)
#endif

#ifndef LOG_ERROR
#define LOG_ERROR(message) ERR_OUT("Error %s", std::string(message).c_str())
#endif

#ifndef LOG_EXCEPTION
#define LOG_EXCEPTION(message) ERR_OUT("Exception %s", std::string(message).c_str())
#endif

#ifndef LOG_ERROR_D
#define LOG_ERROR_D(message, domain) \
LOG_STREAM.push_domain (domain) ; LOG_ERROR (message) ; LOG_STREAM.pop_domain() ;
#endif

#ifndef LOG_ERROR_DD
#define LOG_ERROR_DD(message) LOG_ERROR_D (message, NMV_DEFAULT_DOMAIN)
#endif

#ifndef LOG_VERBOSE
#define LOG_VERBOSE(message) \
LOG_STREAM << LOG_LEVEL_VERBOSE___ << LOG_MARKER_INFO << HERE << message << std::endl
#endif

#ifndef LOG_VERBOSE_D
#define LOG_VERBOSE_D(message) \
LOG_STREAM.push_domain (domain) ; LOG_VERBOSE(message) ;LOG_STREAM.pop_domain();
#endif

#ifndef LOG_SCOPE_VERBOSE
#define LOG_SCOPE_VERBOSE(scopename) \
nemiver::common::ScopeLogger scope_logger (scopename, nemiver::common::LogStream::LOG_LEVEL_VERBOSE) ;
#endif

#ifndef LOG_SCOPE
#define LOG_SCOPE(scopename) \
nemiver::common::ScopeLogger scope_logger (scopename, nemiver::common::LogStream::LOG_LEVEL_NORMAL) ;
#endif

#ifndef LOG_SCOPE_D
#define LOG_SCOPE_D(scopename, domain) \
nemiver::common::ScopeLogger scope_logger \
        (scopename, nemiver::common::LogStream::LOG_LEVEL_VERBOSE, domain) ;
#endif

#ifndef LOG_SCOPE_NORMAL
#define LOG_SCOPE_NORMAL(scopename) \
nemiver::common::ScopeLogger scope_logger (scopename, nemiver::common::LogStream::LOG_LEVEL_NORMAL) ;
#endif

#ifndef LOG_SCOPE_NORMAL_D
#define LOG_SCOPE_NORMAL_D(scopename, domain) \
std::cerr << scopename << " - " domain;
#endif

#ifndef LOG_FUNCTION_SCOPE
#define LOG_FUNCTION_SCOPE LOG_SCOPE(PRETTY_FUNCTION_NAME_)
#endif

#ifndef LOG_FUNCTION_SCOPE_D
#define LOG_FUNCTION_SCOPE_D(domain) LOG_SCOPE_D(PRETTY_FUNCTION_NAME_, domain)
#endif

#ifndef LOG_FUNCTION_SCOPE_NORMAL
#define LOG_FUNCTION_SCOPE_NORMAL LOG_SCOPE_NORMAL(PRETTY_FUNCTION_NAME_)
#endif

#ifndef LOG_FUNCTION_SCOPE_VERBOSE
#define LOG_FUNCTION_SCOPE_VERBOSE LOG_SCOPE_VERBOSE(PRETTY_FUNCTION_NAME_)
#endif

#ifndef LOG_FUNCTION_SCOPE_NORMAL_D
#define LOG_FUNCTION_SCOPE_NORMAL_D(domain) \
    LOG_SCOPE_NORMAL_D(PRETTY_FUNCTION_NAME_, domain)
#endif

#ifndef LOG_FUNCTION_SCOPE_NORMAL_DD
#define LOG_FUNCTION_SCOPE_NORMAL_DD LOG_FUNCTION_SCOPE_NORMAL_D(NMV_DEFAULT_DOMAIN)
#endif

#ifndef LOG_REF_COUNT
#define LOG_REF_COUNT(a_object_ptr, a_name) \
LOG_D ("object '" \
       << a_name \
       << "' refcount: " \
       << (int) a_object_ptr->get_refcount (), \
       "refcount-domain") ;
#endif //LOG_REF_COUNT

#endif // NMV_LOG_STREAM_UTILS_H__

