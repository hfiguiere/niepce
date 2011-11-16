/*
 * niepce - ui/imoduleshell.hpp
 *
 * Copyright (C) 2011 Hubert Figuiere
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


#ifndef _UI_IMODULESHELL_HPP__
#define _UI_IMODULESHELL_HPP__

#include "niepce/ui/selectioncontroller.hpp"

namespace ui {

class IModuleShell
{
public:
    virtual ~IModuleShell() {}
    virtual const ui::SelectionController::Ptr & get_selection_controller() const = 0;
    virtual libraryclient::LibraryClient::Ptr getLibraryClient() const = 0;
};

}

#endif
/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:80
  End:
*/
