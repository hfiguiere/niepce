/*
 * niepce - engine/db/test_filebundle.cpp
 *
 * Copyright (C) 2009 Hubert Figuiere
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


#include "filebundle.hpp"


#include "fwk/utils/init.hpp"
#include "fwk/utils/files.hpp"

#include <boost/test/minimal.hpp>


int test_main(int, char *[])
{
    fwk::utils::init();

    fwk::FileList::Ptr thelist(new fwk::FileList());
    
    thelist->push_back("/foo/bar/img_0001.cr2");
    thelist->push_back("/foo/bar/img_0001.jpg");
    thelist->push_back("/foo/bar/img_0001.xmp");

    thelist->push_back("/foo/bar/dcs_0001.jpg");
    thelist->push_back("/foo/bar/dcs_0001.nef");
    thelist->push_back("/foo/bar/dcs_0001.xmp");

//    thelist->push_back("/tmp/some/file");

    db::FileBundle::ListPtr bundles_list = 
        db::FileBundle::filter_bundles(thelist);

    BOOST_CHECK(bundles_list->size() == 2);
    db::FileBundle::List::const_iterator iter = bundles_list->begin();
    db::FileBundle::Ptr b = *iter;
    BOOST_CHECK(b->main_file() == "/foo/bar/dcs_0001.nef");
    BOOST_CHECK(b->jpeg() == "/foo/bar/dcs_0001.jpg");
    BOOST_CHECK(b->sidecar() == "/foo/bar/dcs_0001.xmp");

    ++iter;
    b = *iter;
    BOOST_CHECK(b->main_file() == "/foo/bar/img_0001.cr2");
    BOOST_CHECK(b->jpeg() == "/foo/bar/img_0001.jpg");
    BOOST_CHECK(b->sidecar() == "/foo/bar/img_0001.xmp");

    return 0;
}

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/

