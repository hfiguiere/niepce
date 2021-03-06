/*
 * niepce - fwk/toolkit/gtkutils.cpp
 *
 * Copyright (C) 2009-2014 Hubert Figuiere
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


#include "fwk/toolkit/application.hpp"
#include "gtkutils.hpp"

namespace fwk {

Glib::RefPtr<Gio::SimpleAction>
add_action(Gio::ActionMap* group,
           const char* name,
           const Gio::ActionMap::ActivateSlot& slot,
           const char* context,
           const char* accel)
{
    auto an_action = Gio::SimpleAction::create(name);
    group->add_action(an_action);
    an_action->signal_activate().connect(sigc::hide(slot));

    if (context && accel) {
        Glib::ustring detail = Glib::ustring::compose("%1.%2", context, name);
        Application::app()->gtkApp()->set_accel_for_action(detail, accel);
    }
    return an_action;
}

Glib::RefPtr<Gio::SimpleAction>
add_menu_action(Gio::ActionMap* group,
                const char* name,
                const Gio::ActionMap::ActivateSlot& slot,
                const Glib::RefPtr<Gio::Menu> & menu,
                const char* label, const char* context, const char* accel)
{
    auto an_action = Gio::SimpleAction::create(name);
    group->add_action(an_action);
    an_action->signal_activate().connect(sigc::hide(slot));
    if (label && context) {
        Glib::ustring detail = Glib::ustring::compose("%1.%2", context, name);
        menu->append(label, detail);
        if(accel) {
            Application::app()->gtkApp()->set_accel_for_action(detail, accel);
        }
    }
    return an_action;
}


Glib::RefPtr<Gtk::ListStore>
ModelRecord::inject(Gtk::TreeView & treeview)
{
    auto model = Gtk::ListStore::create(*this);
    treeview.set_model(model);
    treeview.append_column("text", m_col1);
    return model;
}

Glib::RefPtr<Gtk::ListStore> 
ModelRecord::inject(Gtk::ComboBox & combo)
{
    auto model = Gtk::ListStore::create(*this);
    combo.set_model(model);
    combo.clear();
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
