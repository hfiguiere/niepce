/*
 * niepce - utils/testfiles.cpp
 *
 * Copyright (C) 2007-2018 Hubert Figuiere
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
/** @brief unit test for files */

#include <gtest/gtest.h>

#include <stdlib.h>

#include "files.hpp"
#include "init.hpp"

using fwk::FileList;

// stub to avoid linking the Rust library.
extern "C" {
void niepce_init()
{
}
bool file_is_media(GFileInfo *)
{
  return false;
}
}

TEST(testFiles, testFilesSanity)
{
  fwk::utils::init();

  system("mkdir -p AAtest/sub");
  system("touch AAtest/1");
  system("touch AAtest/2");
  system("touch AAtest/3");

  FileList::Ptr files;

  files = FileList::getFilesFromDirectory("foo", fwk::filter_none);
  ASSERT_FALSE(files);

  files = FileList::getFilesFromDirectory("AAtest", fwk::filter_none);
  ASSERT_TRUE(files);
  ASSERT_EQ(files->size(), 3U);

  system("rm -fr AAtest");
}

