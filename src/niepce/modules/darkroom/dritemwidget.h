/*
 * niepce - darkroom/dritem.h
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



#ifndef _DARKROOM_DRITEMWIDGET_H_
#define _DARKROOM_DRITEMWIDGET_H_

#include <gtkmm/box.h>

#include "fwk/toolkit/widgets/toolboxitemwidget.h"

namespace darkroom {

/** Generic Darkroom item for the toolbox. */
class DrItemWidget
    : public framework::ToolboxItemWidget
{
public:
    DrItemWidget(const Glib::ustring & title);

    void add_widget(const Glib::ustring & label, Gtk::Widget &);
private:
    Gtk::VBox   m_box;
};


}

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:80
  End:
*/


#endif
