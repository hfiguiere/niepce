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

#include "exception.h"

namespace db {

Exception::Exception (const char* a_reason): std::runtime_error (a_reason)
{}

Exception::Exception (const std::string &a_reason): std::runtime_error (a_reason)
{}

Exception::Exception (const Exception &a_other): std::runtime_error (a_other.what ())
{}

Exception::Exception (const std::exception &a_other):
    std::runtime_error (a_other.what ())
{}

Exception&
Exception::operator= (const Exception &a_other)
{
    if (this == &a_other) {
        return *this;
    }
    std::runtime_error::operator= (a_other) ;
    return *this ;
}

Exception::~Exception () throw ()
{}

const char*
Exception::what () const throw ()
{
    return std::runtime_error::what () ;
}

}//end namespace db


