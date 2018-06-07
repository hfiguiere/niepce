/*
 * niepce - fwk/base/t/testoption.cpp
 *
 * Copyright (C) 2017-2018 Hubert Figuiere
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
/** @brief unit test for option */

#include <gtest/gtest.h>

#include <stdlib.h>

#include <string>

#include "fwk/base/option.hpp"

TEST(testOption, testOptionSanity)
{
  fwk::Option<std::string> result;

  // Default, option is empty
  ASSERT_TRUE(result.empty());
  bool unwrapped = false;
  try {
    result.unwrap();
    unwrapped = true;
  } catch(std::runtime_error&) {
    ASSERT_TRUE(true);
  } catch(...) {
    ASSERT_TRUE(false);
  }
  ASSERT_FALSE(unwrapped);
  ASSERT_TRUE(result.empty());

  // Option with value
  result = fwk::Option<std::string>("hello world");
  ASSERT_FALSE(result.empty());
  ASSERT_EQ(result.unwrap(), "hello world");
  ASSERT_TRUE(result.empty());
  // try unwrapping again
  unwrapped = false;
  try {
    result.unwrap();
    unwrapped = true;
  } catch(std::runtime_error&) {
    ASSERT_TRUE(true);
  } catch(...) {
    ASSERT_FALSE(false);
  }
  ASSERT_FALSE(unwrapped);
  ASSERT_TRUE(result.empty());
}

