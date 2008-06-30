/*
 * niepce - library/test_opqueue.cpp
 *
 * Copyright (C) 2007-2008 Hubert Figuiere
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


#include "op.h"
#include "opqueue.h"

#include <boost/bind.hpp>
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>

using namespace library;

void foo(const db::Library::Ptr &)
{
}


BOOST_AUTO_TEST_CASE(opqueue_test)
{
	OpQueue q;

	Op::Ptr p(new Op(1, boost::bind(&foo, db::Library::Ptr())));

	BOOST_CHECK(q.empty());

	q.add(p);
	BOOST_CHECK(!q.empty());

	Op::Ptr p2(q.pop());
	BOOST_CHECK(p2 == p);
	BOOST_CHECK(p2->id() == p->id());
	BOOST_CHECK(q.empty());	
}

