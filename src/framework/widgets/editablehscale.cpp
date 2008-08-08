/*
 * niepce - framework/widgets/editablehscale.cpp
 *
 * Copyright (C) 2008 Hubert Figuiere
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

#include <boost/lexical_cast.hpp>

#include <glibmm/property.h>

#include "utils/debug.h"
#include "editablehscale.h"


namespace framework {

EditableHScale::EditableHScale(double min, double max, double step)
    : m_adj(0, min, max, step),
      m_scale(m_adj), m_entry(m_adj),
      m_dirty(false)
{
    m_scale.property_draw_value() = false;
    m_scale.add_events(Gdk::BUTTON_RELEASE_MASK);
    m_scale.signal_button_release_event()
        .connect(sigc::mem_fun(*this, &EditableHScale::on_button_press_event));
    pack_start(m_scale, Gtk::PACK_EXPAND_WIDGET);
    m_entry.set_width_chars(4);
    m_entry.set_digits(2);
    m_entry.set_editable(true);
    m_entry.add_events(Gdk::BUTTON_RELEASE_MASK);
    m_entry.signal_button_release_event()
        .connect(sigc::mem_fun(*this, &EditableHScale::on_button_press_event));
    pack_start(m_entry, Gtk::PACK_SHRINK);

    m_adj.signal_value_changed()
        .connect(sigc::mem_fun(*this, &EditableHScale::on_adj_value_changed));
    add_events(Gdk::BUTTON_RELEASE_MASK);
}


bool EditableHScale::on_button_press_event(GdkEventButton *event)
{
    DBG_OUT("button %d released", event->button);
    if (event->type == GDK_BUTTON_RELEASE && event->button != 1) {
        return false;
    } 
    else {
        Gtk::Widget::on_button_release_event(event);
        if(m_dirty) {
            m_dirty = false;
            DBG_OUT("value_change.emit(%f)", m_adj.get_value());
            m_sig_value_changed.emit(m_adj.get_value());
        }
        return false;
    }
}


void EditableHScale::on_adj_value_changed()
{
    m_dirty = true;
    m_sig_value_changing.emit(m_adj.get_value());
}


sigc::signal<void,double> & EditableHScale::signal_value_changed()
{
    return m_sig_value_changed;
}


sigc::signal<void,double> & EditableHScale::signal_value_changing()
{
    return m_sig_value_changing;
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

