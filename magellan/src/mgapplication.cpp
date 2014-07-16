/*
 * niepce - mgapplication.cpp
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


#include "config.h"

#include "mgapplication.hpp"
#include "mgwindow.hpp"


namespace mg {

  MgApplication::MgApplication(int & argc, char** & argv)
    : Application(argc, argv, "net.figuiere.magellan", PACKAGE)
  {
  }


  fwk::Application::Ptr MgApplication::create(int & argc, char** & argv)
  {
    if (!m_application) {
      m_application = fwk::Application::Ptr(new MgApplication(argc, argv));
    }
    return m_application;
  }


  fwk::AppFrame::Ptr MgApplication::makeMainFrame()
  {
    m_main_frame = fwk::AppFrame::Ptr(new MgWindow);
    return m_main_frame;
  }

}
