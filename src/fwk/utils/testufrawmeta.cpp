/*
 * niepce - utils/testufrawmeta.cpp
 *
 * Copyright (C) 2007-2018 Hubert Figuière
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
/** @brief unit test for xmp */

#include <boost/test/minimal.hpp>

#include <stdlib.h>
#include <string>
#include <vector>

#include <exempi/xmpconsts.h>

#include "init.hpp"
#include "fwk/base/debug.hpp"
#include "exempi.hpp"
#include "ufrawmeta.hpp"

// stub to avoid linking the Rust library.
extern "C"
void niepce_init()
{
}

int test_main( int, char *[] )             // note the name!
{
  fwk::utils::init();

  std::string dir;
  const char * pdir = getenv("srcdir");
  if(pdir == NULL) {
    dir = ".";
  }
  else {
    dir = pdir;
  }
  fwk::ExempiManagerPtr exemmpi_manager_new();

  xmp::ScopedPtr<XmpPtr> xmp(xmp_new_empty());

  fwk::UfrawMeta ufraw(dir + "/test2.ufraw");

  BOOST_CHECK(ufraw.ufraw_to_xmp(xmp));

  xmp::ScopedPtr<XmpStringPtr> property(xmp_string_new());
  BOOST_CHECK(property != NULL);
  BOOST_CHECK(xmp_get_property(xmp, NS_CAMERA_RAW_SETTINGS,
                               "WhiteBalance", property, NULL));
  BOOST_CHECK(strcmp("Auto", xmp_string_cstr(property)) == 0);

  int32_t t = 0;
  BOOST_CHECK(xmp_get_property_int32(xmp, NS_CAMERA_RAW_SETTINGS,
                                     "Temperature", &t, NULL));
  BOOST_CHECK(t == 3684);
  double exposure = 0;
  BOOST_CHECK(xmp_get_property_float(xmp, NS_CAMERA_RAW_SETTINGS,
                                     "Exposure", &exposure, NULL));
  BOOST_CHECK(exposure == 1.0);

  bool hasCrop = false;
  BOOST_CHECK(xmp_get_property_bool(xmp, NS_CAMERA_RAW_SETTINGS,
                                    "HasCrop", &hasCrop, NULL));
  BOOST_CHECK(hasCrop);


  BOOST_CHECK(xmp_get_property_int32(xmp, NS_CAMERA_RAW_SETTINGS,
                                     "CropUnits", &t, NULL));
  BOOST_CHECK(t == 0);


  double aDim = 0;
  BOOST_CHECK(xmp_get_property_float(xmp, NS_CAMERA_RAW_SETTINGS,
                                     "CropLeft", &aDim, NULL));
  BOOST_CHECK(aDim == 0);
  BOOST_CHECK(xmp_get_property_float(xmp, NS_CAMERA_RAW_SETTINGS,
                                     "CropTop", &aDim, NULL));
  BOOST_CHECK(aDim == 201);
  BOOST_CHECK(xmp_get_property_float(xmp, NS_CAMERA_RAW_SETTINGS,
                                     "CropBottom", &aDim, NULL));
  BOOST_CHECK(aDim == 3522);
  BOOST_CHECK(xmp_get_property_float(xmp, NS_CAMERA_RAW_SETTINGS,
                                     "CropRight", &aDim, NULL));
  BOOST_CHECK(aDim == 2213);

  BOOST_CHECK(xmp_get_property_float(xmp, NS_CAMERA_RAW_SETTINGS,
                                     "CropWidth", &aDim, NULL));
  BOOST_CHECK(aDim == 2213);
  BOOST_CHECK(xmp_get_property_float(xmp, NS_CAMERA_RAW_SETTINGS,
                                     "CropHeight", &aDim, NULL));
  BOOST_CHECK(aDim == 3321);

  bool imported = false;
  BOOST_CHECK(xmp_get_property_bool(xmp, xmp::UFRAW_INTEROP_NAMESPACE,
                                    "ImportedFromUFraw", &imported, NULL));
  BOOST_CHECK(imported);

  return 0;
}

