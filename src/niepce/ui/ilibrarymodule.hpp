/*
 * niepce - niepce/ui/niepcelibrarymodule.hpp
 *
 * Copyright (C) 2009 Hubert Figuiere
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


#ifndef __NIEPCE_UI_ILIBRARYMODULE_HPP_
#define __NIEPCE_UI_ILIBRARYMODULE_HPP_

#include <string>
#include <tr1/memory>

#include "fwk/toolkit/controller.hpp"

namespace ui {

/** interface to implement a library module */
class ILibraryModule
  : public fwk::Controller
{
public:
  typedef std::tr1::shared_ptr<ILibraryModule> Ptr;

  /** dispatch action by name to the controller */
  virtual void dispatch_action(const std::string & action_name) = 0;
};


}


#endif
