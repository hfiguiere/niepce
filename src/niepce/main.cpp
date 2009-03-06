/*
 * niepce - main/main.cpp
 *
 * Copyright (C) 2007 Hubert Figuiere
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


#include <boost/bind.hpp>
#include <boost/filesystem/path.hpp>

#include "fwk/utils/exempi.h"
#include "xmp.h"
#include "ui/niepceapplication.hpp"

namespace bfs = boost::filesystem;
using utils::ExempiManager;


int main(int argc, char ** argv)
{
	// we need to init boost::filesystem to use native path checks.
	// note: this is not the case in the tests.
	bfs::path::default_name_check(&bfs::native);

	ExempiManager ex_manager(niepce::xmp_namespaces);
	
	return fwk::Application::main(
		boost::bind(&ui::NiepceApplication::create),
		argc, argv);
}


