/*
 * niepce - fwk/toolkit/widgets/notabtextview.cpp
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



#include "fwk/base/debug.hpp"

#include "notabtextview.hpp"


namespace fwk {

bool NoTabTextView::on_key_press_event(GdkEventKey *event)
{
    if(event->keyval == GDK_KEY_Tab) {
        get_parent()->child_focus(Gtk::DIR_TAB_FORWARD);
        return true;
    }
    return Gtk::TextView::on_key_press_event(event);
}


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
