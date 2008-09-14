/*
 * niepce - darkroom/toolboxcontroller.h
 *
 * Copyright (C) 2008 Hubert Figuiere
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


#ifndef _DARKROOM_TOOLBOXCONTROLLER_H__
#define _DARKROOM_TOOLBOXCONTROLLER_H__

#include "framework/controller.h"


namespace framework {
class Dock;
class DockItem;
}

namespace darkroom {

class ToolboxController
    : public framework::Controller
{
public:
    typedef boost::shared_ptr<ToolboxController> Ptr;
    ToolboxController(framework::Dock &);
    virtual Gtk::Widget * buildWidget();
private:
    framework::DockItem *m_dockitem;
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
