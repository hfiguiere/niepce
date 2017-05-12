/*
 * niepce - niepce/ui/dialogs/editlabels.cpp
 *
 * Copyright (C) 2009-2017 Hubert Figuière
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

#include <boost/format.hpp>

#include <glibmm/i18n.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>

#include "fwk/base/debug.hpp"
#include "fwk/toolkit/application.hpp"
#include "fwk/toolkit/gdkutils.hpp"
#include "fwk/toolkit/undo.hpp"
#include "libraryclient/libraryclient.hpp"
#include "libraryclient/uidataprovider.hpp"
#include "editlabels.hpp"


using libraryclient::LibraryClient;

namespace ui {

EditLabels::EditLabels(const LibraryClient::Ptr & libclient)
    : fwk::Dialog(GLADEDIR"editlabels.ui", "editLabels")
    , m_labels(libclient->getDataProvider()->getLabels())
    , m_lib_client(libclient)
{
    std::fill(m_status.begin(), m_status.end(), false);
}


void EditLabels::setup_widget()
{
    Glib::RefPtr<Gtk::Builder> _builder = builder();

    add_header(_("Edit Labels"));

    const char * colour_fmt = "colorbutton%1%";
    const char * value_fmt = "value%1%";
    for(size_t i = 0; i < 5; i++) {
        bool has_label = m_labels.size() > i;

        Gtk::ColorButton *colourbutton;
        Gtk::Entry *labelentry;

        _builder->get_widget(str(boost::format(colour_fmt) % (i+1)), colourbutton);
        m_colours[i] = colourbutton;
        _builder->get_widget(str(boost::format(value_fmt) % (i+1)), labelentry);
        DBG_ASSERT(labelentry, "couldn't find label");
        m_entries[i] = labelentry;

        if(has_label) {
            Gdk::RGBA colour = fwk::rgbcolour_to_gdkcolor(m_labels[i]->colour());
            colourbutton->set_rgba(colour);
            labelentry->set_text(m_labels[i]->label());
        }
        colourbutton->signal_color_set().connect(
            sigc::bind(sigc::mem_fun(*this, &EditLabels::label_colour_changed), i));
        labelentry->signal_changed().connect(
            sigc::bind(sigc::mem_fun(*this, &EditLabels::label_name_changed), i));
    }
    gtkDialog().signal_response().connect(sigc::mem_fun(*this, &EditLabels::update_labels));
}


void EditLabels::label_name_changed(size_t idx)
{
    m_status[idx] = true;
}


void EditLabels::label_colour_changed(size_t idx)
{
    m_status[idx] = true;
}

void EditLabels::update_labels(int /*response*/)
{
    fwk::UndoTransaction *undo = nullptr;
    for(size_t i = 0; i < 5; i++) {
        if(m_status[i]) {
            bool has_label = m_labels.size() > i;

            DBG_OUT("updating label %lu", i);
            std::string new_name = m_entries[i]->get_text();
            // a newname is NOT valid.
            if(new_name.empty()) {
                continue;
            }
            std::string new_colour
                = fwk::gdkcolor_to_rgbcolour(m_colours[i]->get_rgba()).to_string();
            if(!undo) {
                undo = fwk::Application::app()->begin_undo(_("Change Labels"));
            }

            if(has_label) {
                std::string current_name = m_labels[i]->label();
                std::string current_colour = m_labels[i]->colour().to_string();

                undo->new_command<void>(
                    std::bind(&libraryclient::LibraryClient::updateLabel,
                                m_lib_client, m_labels[i]->id(), new_name,
                                new_colour),
                    std::bind(&libraryclient::LibraryClient::updateLabel,
                                m_lib_client, m_labels[i]->id(), current_name,
                                current_colour));
            }
            else {
                using std::placeholders::_1;
                undo->new_command<int>(
                    std::bind(&libraryclient::LibraryClient::createLabel,
                                m_lib_client, new_name, new_colour),
                    std::bind(&libraryclient::LibraryClient::deleteLabel,
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
