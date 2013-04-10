/*
 * niepce - cwapplication.cpp
 *
 * Copyright (C) 2009, 2013 Hubert Figuiere
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


#include "config.h"

#include "cwapplication.hpp"
#include "cwwindow.hpp"


namespace cw {

  CwApplication::CwApplication(int & argc, char** & argv)
	  : Application(argc, argv, "net.figuiere.camerawire", PACKAGE)
  {
  }


  fwk::Application::Ptr CwApplication::create(int & argc, char** & argv)
  {
    if (!m_application) {
      m_application = fwk::Application::Ptr(new CwApplication(argc, argv));
    }
    return m_application;
  }


  fwk::Frame::Ptr CwApplication::makeMainFrame()
  {
    return fwk::Frame::Ptr(new CwWindow);
  }

}
