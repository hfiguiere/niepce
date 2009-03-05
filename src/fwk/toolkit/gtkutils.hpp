/*
 * niepce - fwk/toolkit/gtkutils.hpp
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


#ifndef __FWK_GTKUTILS_H__
#define __FWK_GTKUTILS_H__

#include <string>
#include <gtkmm/treeview.h>


namespace fwk {

class TextTreeviewModel
    : public Gtk::TreeModelColumnRecord
{
public:
    TextTreeviewModel()
        { add(m_col1); }

    /** "inject" the model to the TreeView */
    void inject(Gtk::TreeView & treeview);

    Gtk::TreeModelColumn<std::string> m_col1;
};

}


#endif
/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
