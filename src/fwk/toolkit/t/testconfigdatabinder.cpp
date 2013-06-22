/*
 * niepce - fwk/toolkit/testdatabinder.cpp
 *
 * Copyright (C) 2013 Hubert Figuiere
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

#include <boost/test/minimal.hpp>

#include <glibmm/init.h>
#include <glibmm/object.h>
#include <glibmm/property.h>
#include <giomm/file.h>

#include "fwk/toolkit/configdatabinder.hpp"

class PropertyFixture
	: Glib::Object
{
public:
	PropertyFixture()
		: Glib::ObjectBase(typeid(PropertyFixture))
		, Glib::Object()
		, str_property(*this, "string")
		, int_property(*this, "int")
		, bool_property(*this, "bool")
		{
		}

	Glib::Property<Glib::ustring> str_property;
	Glib::Property<int> int_property;
	Glib::Property<bool> bool_property;
};

int test_main( int, char *[] )             // note the name!
{
	Glib::init();

	PropertyFixture fixture;

	// get tmp file
	Glib::ustring cfg_file("/tmp/tmp-cfg.ini");
	Glib::RefPtr<Gio::File> file = Gio::File::create_for_path(cfg_file);

	{
		fwk::Configuration cfg(cfg_file);

		{
			// the binder only write to the preference when
			// it is destroyed.
			fwk::ConfigDataBinder<int>
				binder_int(fixture.int_property.get_proxy(),
					   cfg, "int");

			fixture.int_property = 1;
		}
		BOOST_CHECK(file->query_exists());

		BOOST_CHECK(cfg.hasKey("int"));
		Glib::ustring val = cfg.getValue("int", "0");
		BOOST_CHECK(val == "1");

		{
			fwk::ConfigDataBinder<Glib::ustring>
				binder_str(fixture.str_property.get_proxy(),
					   cfg, "string");
			fixture.str_property = "foo";
		}
		BOOST_CHECK(file->query_exists());

		BOOST_CHECK(cfg.hasKey("string"));
		val = cfg.getValue("string", "");
		BOOST_CHECK(val == "foo");

		{
			fwk::ConfigDataBinder<bool>
				binder_bool(fixture.bool_property.get_proxy(),
					   cfg, "bool");
			fixture.bool_property = true;
		}
		BOOST_CHECK(file->query_exists());

		BOOST_CHECK(cfg.hasKey("bool"));
		val = cfg.getValue("bool", "");
		BOOST_CHECK(val == "1");
	}


	file->remove();

	BOOST_CHECK(!file->query_exists());

	return 0;
}
