/*
 * niepce - library/test_worker.cpp
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


#include "fwk/utils/init.hpp"
#include "fwk/utils/fsutils.hpp"

#define BOOST_AUTO_TEST_MAIN
#include "locallibraryserver.hpp"

#include <boost/test/minimal.hpp>


using namespace eng;
using namespace libraryclient;

//BOOST_AUTO_TEST_CASE(worker_test)
int test_main(int, char *[])
{
  fwk::utils::init();

	char templ[] = "/tmp/niepce-tmpXXXXXX";
	char *ptempl =  mkdtemp(templ);
	BOOST_CHECK(ptempl);
	{
		fwk::DirectoryDisposer d(ptempl);
		LocalLibraryServer w(std::string("") + ptempl, fwk::NotificationCenter::Ptr());

		BOOST_CHECK(w._tasks().empty());

		Op::Ptr p(new Op(0, [](const Library::Ptr &){}));
		w.schedule(p);
	}
    return 0;
}

