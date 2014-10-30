/*
 * niepce - ui/moduleshellwidget.hpp
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

#ifndef __MODULE_SHELL_WIDGET_H__
#define __MODULE_SHELL_WIDGET_H__

#include <vector>
#include <utility>

#include <gtkmm/box.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/menubutton.h>
#include <gtkmm/stack.h>
#include <gtkmm/stackswitcher.h>

namespace Gtk {
class ToggleButton;
}

namespace ui {


class ModuleShellWidget
    : public Gtk::Box
{
public:
    ModuleShellWidget();

    void appendPage(Gtk::Widget & w,
                     const Glib::ustring & name,
                     const Glib::ustring & label);
    void activatePage(const std::string &);

    Gtk::MenuButton & getMenuButton()
        { return m_menubutton; }
//    Gtk::Stack* getStack() const
//        { return m_stack; }

    sigc::signal<void, const std::string &> signal_activated;
    sigc::signal<void, const std::string &> signal_deactivated;

private:
    Gtk::Box                m_mainbox;
    Gtk::ButtonBox          m_mainbar;
    Gtk::MenuButton         m_menubutton;
    Gtk::Stack              m_stack;
    Gtk::StackSwitcher      m_switcher;
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
