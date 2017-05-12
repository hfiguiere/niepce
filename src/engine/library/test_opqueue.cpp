/*
 * niepce - library/test_opqueue.cpp
 *
 * Copyright (C) 2007-2017 Hubert Figuière
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


#include "op.hpp"
#include "opqueue.hpp"

#include <boost/test/minimal.hpp>

using namespace eng;

//BOOST_AUTO_TEST_CASE(opqueue_test)
int test_main(int, char *[])
{
	OpQueue q;

	Op::Ptr p(new Op(1, [](const Library::Ptr &){}));

	BOOST_CHECK(q.empty());

	tid_t old_id = p->id();
	Op* old_ptr = p.get();
	q.add(p);
	BOOST_CHECK(!q.empty());
	BOOST_CHECK(p == nullptr);

	Op::Ptr p2(q.pop());
	BOOST_CHECK(p2.get() == old_ptr);
	BOOST_CHECK(p2->id() == old_id);
	BOOST_CHECK(q.empty());
	return 0;
}

