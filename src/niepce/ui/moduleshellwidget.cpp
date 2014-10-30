/*
 * niepce - ui/moduleshellwidget.cpp
 *
 * Copyright (C) 2007-2014 Hubert Figuiere
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

#include <gtkmm/togglebutton.h>
#include <gtkmm/stackswitcher.h>

#include "fwk/base/debug.hpp"
#include "ui/moduleshellwidget.hpp"

namespace ui {

ModuleShellWidget::ModuleShellWidget()
    : Gtk::Box(Gtk::ORIENTATION_VERTICAL)
    , m_mainbox(Gtk::ORIENTATION_HORIZONTAL)
    , m_mainbar(Gtk::ORIENTATION_HORIZONTAL)
{
    set_spacing(4);
    m_mainbar.set_layout(Gtk::BUTTONBOX_START);
    m_mainbar.set_spacing(4);
    m_menubutton.set_direction(Gtk::ARROW_NONE);
    m_mainbox.pack_end(m_menubutton, Gtk::PACK_SHRINK);
    m_mainbox.pack_start(m_mainbar, Gtk::PACK_EXPAND_WIDGET);
    pack_start(m_mainbox, Gtk::PACK_SHRINK);

    m_mainbox.pack_start(m_switcher);
    pack_start(m_stack);

    m_switcher.set_stack(m_stack);
}



void
ModuleShellWidget::appendPage(Gtk::Widget & w, const Glib::ustring & name,
                              const Glib::ustring & label)
{
    m_stack.add(w, name, label);
}

void ModuleShellWidget::activatePage(const std::string & name)
{
    Glib::ustring current_name
        = m_stack.get_visible_child_name();
    if(current_name != name) {
        signal_deactivated(current_name);
        m_stack.set_visible_child(name);
        signal_activated(name);
    }
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
