/*
 * niepce - fwk/toolkit/dockable.hpp
 *
 * Copyright (C) 2008, 2011 Hubert Figuiere
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


#ifndef __FRAMEWORK_DOCKABLE_H__
#define __FRAMEWORK_DOCKABLE_H__

#include <glibmm/ustring.h>
#include <gtkmm/box.h>
#include "fwk/toolkit/uicontroller.hpp"
#include "fwk/toolkit/widgets/dock.hpp"

namespace fwk {


/** A dockable item controller
 */
class Dockable
    : public UiController
{
public:
    Dockable(const Glib::ustring& name, 
             const Glib::ustring& long_name, 
             const Glib::ustring& icon_name);

    /** return the vbox controlled (construct it if needed) */
    Gtk::VBox * build_vbox();

    const Glib::ustring & get_name() const
        {
            return m_name;
        }
private:
    Gtk::VBox * m_vbox;
    Glib::ustring m_name;
    Glib::ustring m_long_name;
    Glib::ustring m_icon_name;
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

