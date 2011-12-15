/*
 * niepce - ui/moduleshellwidget.cpp
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

#include <gtkmm/togglebutton.h>

#include "fwk/base/debug.hpp"
#include "ui/moduleshellwidget.hpp"

namespace ui {

ModuleShellWidget::ModuleShellWidget()
    : Gtk::VBox(),
      m_currentpage(-1)
{
    set_spacing(4);
    m_mainbar.set_layout(Gtk::BUTTONBOX_START);
    m_mainbar.set_spacing(4);
    m_notebook.set_show_tabs(false);
    pack_start(m_mainbar, Gtk::PACK_SHRINK);
    pack_start(m_notebook);
}

int
ModuleShellWidget::append_page(Gtk::Widget & w, const Glib::ustring & label)
{
    int idx;
    
    Gtk::ToggleButton* button = Gtk::manage(new Gtk::ToggleButton(label));
    m_mainbar.pack_start(*button);
    idx = m_notebook.append_page(w, label);
    sigc::connection conn = button->signal_toggled().connect(
        sigc::bind(sigc::mem_fun(this, &ModuleShellWidget::set_current_page),
                   idx, button));
    if(m_currentpage == -1) {
        set_current_page(idx, button);
    }
    if((int)m_buttons.size() < idx + 1) {
        m_buttons.resize(idx + 1);
    }
    m_buttons[idx] = std::make_pair(button, conn);
    return idx;
}
	
void ModuleShellWidget::activate_page(int idx)
{
    if(m_currentpage != idx) {
        Gtk::ToggleButton * btn = m_buttons[idx].first;
        set_current_page(idx, btn);
    }
}


void ModuleShellWidget::set_current_page(int idx, Gtk::ToggleButton * btn)
{
    if(m_currentpage == idx) {
        // just preempt. Make sure the button is still active.
        // otherwise it cause an infinite loop.
        m_buttons[m_currentpage].second.block();
        m_buttons[m_currentpage].first->set_active(true);
        m_buttons[m_currentpage].second.unblock();
        return;
    }
    m_notebook.set_current_page(idx);
    if(m_currentpage >= 0) {
        m_buttons[m_currentpage].second.block();
        m_buttons[m_currentpage].first->set_active(false);
        m_buttons[m_currentpage].second.unblock();
    }
    btn->set_active(true);
    signal_deactivated(m_currentpage);
    m_currentpage = idx;
    signal_activated(idx);
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
