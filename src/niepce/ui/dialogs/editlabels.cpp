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


#include <algorithm>

#include <boost/bind.hpp>
#include <boost/format.hpp>

#include <glibmm/i18n.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>

#include "fwk/utils/debug.h"
#include "editlabels.hpp"
#include "libraryclient/libraryclient.hpp"
#include "fwk/toolkit/application.hpp"
#include "fwk/toolkit/undo.hpp"


using libraryclient::LibraryClient;

namespace ui {

EditLabels::EditLabels(const eng::Label::List & labels, const LibraryClient::Ptr & libclient)
    : fwk::Dialog(GLADEDIR"editlabels.ui", "editLabels")
    , m_labels(labels)
    , m_lib_client(libclient)
{
    std::fill(m_status.begin(), m_status.end(), false);
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
        DBG_ASSERT(labelentry, "couldn't find label");
        labelentry->set_text(m_labels[i]->label());
        labelentry->property_text().signal_changed().connect(
            sigc::bind(sigc::mem_fun(*this, &EditLabels::label_name_changed), i));
        m_entries[i] = labelentry;
    }
    gtkDialog().signal_response().connect(sigc::mem_fun(*this, &EditLabels::update_labels));
}


void EditLabels::label_name_changed(size_t idx)
{
    m_status[idx] = true;
}


void EditLabels::update_labels(int /*response*/)
{
    fwk::UndoTransaction *undo = NULL;
    for(int i = 0; i < 5; i++) {
        if(m_status[i]) {
            DBG_OUT("updating label %d", i);
            std::string current_name = m_labels[i]->label();
            std::string new_name = m_entries[i]->get_text();
            if(!undo) {
                undo = fwk::Application::app()->begin_undo(_("Change Label name"));
            }
            undo->new_command(boost::bind(&libraryclient::LibraryClient::renameLabel, 
                                          m_lib_client, m_labels[i]->id(), new_name),
                              boost::bind(&libraryclient::LibraryClient::renameLabel, 
                                          m_lib_client, m_labels[i]->id(), current_name));
        }
    }
    if(undo) {
        undo->execute();
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
}
