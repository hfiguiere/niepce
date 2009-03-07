/*
 * niepce - fwk/toolkit/gtkutils.cpp
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

#include <gtkmm/liststore.h>

#include "gtkutils.hpp"

namespace fwk {

Glib::RefPtr<Gtk::ListStore> 
TextTreeviewModel::inject(Gtk::TreeView & treeview)
{
    Glib::RefPtr<Gtk::ListStore> model = Gtk::ListStore::create(*this);
    treeview.set_model(model);
    treeview.append_column("text", m_col1);
    return model;
}


Glib::RefPtr<Gtk::ListStore> 
TextPairTreeviewModel::inject(Gtk::ComboBox & combo)
{
    Glib::RefPtr<Gtk::ListStore> model = Gtk::ListStore::create(*this);
    combo.set_model(model);
		static_cast<Gtk::CellLayout&>(combo).clear();
		combo.pack_start(m_col1);
    return model;
}

}
/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
