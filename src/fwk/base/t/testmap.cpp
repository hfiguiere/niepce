/*
 * niepce - base/t/testmap.cpp
 *
 * Copyright (C) 2013-2018 Hubert Figuiere
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
/** @brief unit test for fractions */

#include <gtest/gtest.h>

#include <stdlib.h>
#include <map>
#include "fwk/base/map.hpp"

TEST(testMap, testMapSanity)
{

  std::map<std::string, int> n { { "one", 1 }, { "two", 2 } };

  ASSERT_EQ(n.size(), 2U);

  std::vector<std::string> keys;
  fwk::map_get_keys(n, keys);
  ASSERT_EQ(n.size(), keys.size());
  ASSERT_EQ(keys[0], "one");

  std::vector<int> values;
  fwk::map_get_values(n, values);
  ASSERT_EQ(n.size(), values.size());
  ASSERT_EQ(values[0], 1);
}

