/*
 * niepce - src/fwk/utils/t/testpathutils.cpp
 *
 * Copyright (C) 2009-2018 Hubert Figuière
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

#include <gtest/gtest.h>

#include <string>

#include "pathutils.hpp"

TEST(testPathUtils, testPathUtilsSanity)
{
  ASSERT_EQ(fwk::path_basename("/foo/bar/baz.txt"), "baz.txt");

  ASSERT_EQ(fwk::path_stem("/foo/bar/baz.txt"), "/foo/bar/baz");

  ASSERT_EQ(fwk::path_dirname("/foo/bar/"), "/foo/bar");
  ASSERT_EQ(fwk::path_dirname("/foo/bar/baz.txt"), "/foo/bar");
  ASSERT_EQ(fwk::path_dirname("baz.txt"), ".");

  ASSERT_EQ(fwk::path_extension("/foo/bar/baz.txt"), ".txt");
  ASSERT_EQ(fwk::path_extension("img_2114.xmp"), ".xmp");

  ASSERT_EQ(fwk::path_replace_extension("/foo/bar/baz.txt", ".xmp"), "/foo/bar/baz.xmp");
}

