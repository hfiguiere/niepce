/*
 * niepce - library/test_worker.cpp
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



#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>

#include "worker.h"


using namespace library;

BOOST_AUTO_UNIT_TEST(worker_test)
{
	char templ[] = "/tmp/niepce-tmpXXXXXX";
	char *ptempl =  mkdtemp(templ);
	BOOST_CHECK(ptempl);
	{
		Worker w(std::string("") + ptempl);
		
		BOOST_CHECK(w._ops().isEmpty());
		
		Op::Ptr p(new Op(OP_NONE));
		w.schedule(p);
	}

	
}

