/*
 * niepce - engine/db/test_library.cpp
 *
 * Copyright (C) 2007-2013 Hubert Figuiere
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  
 * 02110-1301, USA
 */


#include "fwk/base/debug.hpp"
#include "fwk/utils/init.hpp"
#include "fwk/utils/db/sqlstatement.hpp"
#include "fwk/utils/db/iconnectiondriver.hpp"
#include "library.hpp"
#include "libfile.hpp"
#include "libfolder.hpp"

#include <boost/test/minimal.hpp>


//BOOST_AUTO_TEST_CASE(library_test)
int test_main(int, char *[])
{
    fwk::utils::init();
    eng::Library lib("./", fwk::NotificationCenter::Ptr());

    BOOST_CHECK(lib.ok());

    BOOST_CHECK(lib.checkDatabaseVersion() == DB_SCHEMA_VERSION);

    db::IConnectionDriver::Ptr db(lib.dbDriver());

    eng::LibFolder::Ptr folder_added(lib.addFolder("foo"));
    BOOST_CHECK(folder_added);
    BOOST_CHECK(folder_added->id() > 0);
    eng::LibFolder::Ptr f(lib.getFolder("foo"));
    BOOST_CHECK(f);
    BOOST_CHECK(f->id() == folder_added->id());
    lib.addFolder("bar");
    BOOST_CHECK(lib.getFolder("bar"));

    eng::LibFolder::ListPtr l( new eng::LibFolder::List );
    lib.getAllFolders( l );
    // now we have the Trash folder created at startup
    BOOST_CHECK( l->size() == 3 );

    int file_id = lib.addFile(folder_added->id(), "foo/myfile", eng::Library::Managed::NO);
    BOOST_CHECK(file_id > 0);

    BOOST_CHECK(lib.moveFileToFolder(file_id, 100) == false);
    BOOST_CHECK(lib.moveFileToFolder(file_id, folder_added->id()));

    int count = lib.countFolder(folder_added->id());
    BOOST_CHECK(count == 1);

    const eng::LibFile::ListPtr fl(new eng::LibFile::List);
    lib.getFolderContent(folder_added->id(), fl);
    BOOST_CHECK(fl->size() == (size_t)count);
    BOOST_CHECK(fl->front()->id() == file_id);

    int kwid1 = lib.makeKeyword("foo");
    BOOST_CHECK(kwid1 > 0);
    int kwid2 = lib.makeKeyword("bar");
    BOOST_CHECK(kwid2 > 0);

    // duplicate keyword
    int kwid3 = lib.makeKeyword("foo");
    // should return kwid1 because it already exists.
    BOOST_CHECK(kwid3 == kwid1);

    BOOST_CHECK(lib.assignKeyword(kwid1, file_id));
    BOOST_CHECK(lib.assignKeyword(kwid2, file_id));

    eng::LibFile::ListPtr fl2(new eng::LibFile::List);
    lib.getKeywordContent(kwid1, fl2);
    BOOST_CHECK(fl2->size() == 1);
    BOOST_CHECK(fl2->front()->id() == file_id);

    eng::KeywordListPtr kl(new eng::KeywordList);
    lib.getAllKeywords(kl);
    BOOST_CHECK(kl->size() == 2);

    BOOST_CHECK(unlink(lib.dbName().c_str()) != -1);
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
