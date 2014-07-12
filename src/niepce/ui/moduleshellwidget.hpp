/*
 * niepce - ui/moduleshellwidget.hpp
 *
 * Copyright (C) 2007-2009 Hubert Figuiere
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

#include <gtkmm/notebook.h>
#include <gtkmm/box.h>
#include <gtkmm/buttonbox.h>

namespace Gtk {
	class ToggleButton;
}

namespace ui {


class ModuleShellWidget
    : public Gtk::Box
{
public:
    ModuleShellWidget();
    
    int append_page(Gtk::Widget & w, const Glib::ustring & label);
    void activate_page(int);

    sigc::signal<void, int> signal_activated;
    sigc::signal<void, int> signal_deactivated;
protected:
    
    void set_current_page(int, Gtk::ToggleButton *);
private:
    Gtk::HButtonBox         m_mainbar;
    Gtk::Notebook           m_notebook;
    int                     m_currentpage;
    std::vector<std::pair<Gtk::ToggleButton*, sigc::connection> > m_buttons;
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
