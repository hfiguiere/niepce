/*
 * niepce - niepce/ui/dialogs/editlabels.hpp
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


#include <boost/format.hpp>

#include <glibmm/i18n.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>

#include "editlabels.hpp"


namespace ui {

EditLabels::EditLabels(const eng::Label::List & labels)
    : fwk::Dialog(GLADEDIR"editlabels.ui", "editLabels")
    , m_labels(labels)
{
}


void EditLabels::setup_widget()
{
    Glib::RefPtr<Gtk::Builder> _builder = builder();

    add_header(_("<b>Edit Labels</b>"));

    const char * color_fmt = "color%1%";
    const char * value_fmt = "value%1%";
    for(size_t i = 0; i < 5; i++) {
        if(m_labels.size() <= i) {
            break;
        }
        Gtk::Label *colorlbl;
        Gtk::Entry *labelentry;
        _builder->get_widget(str(boost::format(color_fmt) % (i+1)), colorlbl);
        _builder->get_widget(str(boost::format(value_fmt) % (i+1)), labelentry);
        labelentry->set_text(m_labels[i]->label());
        labelentry->signal_changed().connect(
            sigc::bind(sigc::mem_fun(*this, &EditLabels::label_name_changed), labelentry, i+1));
    }
}


void EditLabels::label_name_changed(Gtk::Entry *w, size_t idx)
{
    m_labels[idx]->set_label(w->get_text());
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
}
