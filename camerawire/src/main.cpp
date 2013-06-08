/*
 * niepce - camerawire/main.cpp
 *
 * Copyright (C) 2007-2009, 2013 Hubert Figuiere
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


#include "config.h"

#include <glibmm/i18n.h>

#include "fwk/utils/init.hpp"
#include "cwapplication.hpp"

int main(int argc, char ** argv)
{
  bindtextdomain(CW_GETTEXT_PACKAGE, CW_LOCALEDIR);
  bind_textdomain_codeset(CW_GETTEXT_PACKAGE, "UTF-8");
  textdomain(CW_GETTEXT_PACKAGE);

  fwk::utils::init();

  fwk::Application::Ptr app = cw::CwApplication::create(argc, argv);
  return fwk::Application::main(app, argc, argv);
}


