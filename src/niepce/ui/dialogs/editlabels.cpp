/*
 * niepce - niepce/ui/dialogs/editlabels.cpp
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

#include "fwk/utils/boost.hpp"
#include "fwk/utils/debug.hpp"
#include "fwk/toolkit/application.hpp"
#include "fwk/toolkit/gdkutils.hpp"
#include "fwk/toolkit/undo.hpp"
#include "libraryclient/libraryclient.hpp"
#include "editlabels.hpp"


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

    add_header(_("Edit Labels"));

    const char * color_fmt = "colorbutton%1%";
    const char * value_fmt = "value%1%";
    for(size_t i = 0; i < 5; i++) {
        bool has_label = m_labels.size() > i;

        Gtk::ColorButton *colorbutton;
        Gtk::Entry *labelentry;

        _builder->get_widget(str(boost::format(color_fmt) % (i+1)), colorbutton);
        m_colors[i] = colorbutton;
        _builder->get_widget(str(boost::format(value_fmt) % (i+1)), labelentry);
        DBG_ASSERT(labelentry, "couldn't find label");
        m_entries[i] = labelentry;

        if(has_label) {
            Gdk::Color color = fwk::rgbcolor_to_gdkcolor(m_labels[i]->color());
            colorbutton->set_color(color);
            labelentry->set_text(m_labels[i]->label());
        }
        colorbutton->signal_color_set().connect(
            sigc::bind(sigc::mem_fun(*this, &EditLabels::label_color_changed), i));
        labelentry->signal_changed().connect(
            sigc::bind(sigc::mem_fun(*this, &EditLabels::label_name_changed), i));
    }
    gtkDialog().signal_response().connect(sigc::mem_fun(*this, &EditLabels::update_labels));
}


void EditLabels::label_name_changed(size_t idx)
{
    m_status[idx] = true;
}


void EditLabels::label_color_changed(size_t idx)
{
    m_status[idx] = true;
}

void EditLabels::update_labels(int /*response*/)
{
    fwk::UndoTransaction *undo = NULL;
    for(size_t i = 0; i < 5; i++) {
        if(m_status[i]) {
            bool has_label = m_labels.size() > i;

            DBG_OUT("updating label %d", i);
            std::string new_name = m_entries[i]->get_text();
            // a newname is NOT valid.
            if(new_name.empty()) {
                continue;
            }
            std::string new_color 
                = fwk::gdkcolor_to_rgbcolor(m_colors[i]->get_color()).to_string();
            if(!undo) {
                undo = fwk::Application::app()->begin_undo(_("Change Labels"));
            }

            if(has_label) {
                std::string current_name = m_labels[i]->label();
                std::string current_color = m_labels[i]->color().to_string();

                undo->new_command<void>(
                    boost::bind(&libraryclient::LibraryClient::updateLabel, 
                                m_lib_client, m_labels[i]->id(), new_name, 
                                new_color),
                    boost::bind(&libraryclient::LibraryClient::updateLabel, 
                                m_lib_client, m_labels[i]->id(), current_name, 
                                current_color));
            }
            else {
                undo->new_command<int>(
                    boost::bind(&libraryclient::LibraryClient::createLabel, 
                                m_lib_client, new_name, new_color),
                    boost::bind(&libraryclient::LibraryClient::deleteLabel, 
                                m_lib_client, _1));
            }
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
