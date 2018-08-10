/*
 * niepce - fwk/toolkit/gtkutils.hpp
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


#ifndef __FWK_GTKUTILS_H__
#define __FWK_GTKUTILS_H__

#include <string>

#include <giomm/simpleactiongroup.h>
#include <giomm/menu.h>
#include <gtkmm/treeview.h>
#include <gtkmm/combobox.h>
#include <gtkmm/liststore.h>


namespace fwk {

Glib::RefPtr<Gio::SimpleAction>
add_action(const Glib::RefPtr<Gio::ActionMap> & group,
           const char* name,
           const Gio::ActionMap::ActivateSlot& slot,
           const char* context = nullptr,
           const char* accel = nullptr);

/** Helper to add an action with a menu.
 * @param group the action group
 * @param name the action name
 * @param slot the action slot
 * @param menu the menu append the action to.
 * @param label the label of the menu
 */
Glib::RefPtr<Gio::SimpleAction>
add_menu_action(const Glib::RefPtr<Gio::ActionMap> & group,
                const char* name,
                const Gio::ActionMap::ActivateSlot& slot,
                const Glib::RefPtr<Gio::Menu> & menu,
                const char* label, const char* context = nullptr,
                const char* accel = nullptr);


/** a simple model record with one text column.
 * This class is "abstract".
 */
class ModelRecord
    : public Gtk::TreeModelColumnRecord
{
public:
    virtual ~ModelRecord()
        {
        }
    /** "inject" the model to the TreeView */
    virtual Glib::RefPtr<Gtk::ListStore> inject(Gtk::TreeView & treeview);
    /** "inject" the model to the ComboBox */
    virtual Glib::RefPtr<Gtk::ListStore> inject(Gtk::ComboBox & combox);

    Gtk::TreeModelColumn<std::string> m_col1;
protected:
    ModelRecord()
        {}
};


class TextModelRecord
    : public ModelRecord
{
public:
    TextModelRecord()
        { add(m_col1); }

};

class TextPairModelRecord
    : public ModelRecord
{
public:
    TextPairModelRecord()
        {
            add(m_col1);
            add(m_col2);
        }

    Gtk::TreeModelColumn<std::string> m_col2;
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
