/*
 * niepce - fwk/toolkit/metadatawidget.cpp
 *
 * Copyright (C) 2008-2011 Hubert Figuiere
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


#include <utility>

#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include <glibmm/i18n.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>

#include "fwk/base/debug.hpp"
#include "fwk/base/autoflag.hpp"
#include "fwk/base/fractions.hpp"
#include "fwk/utils/exempi.hpp"
#include "fwk/utils/stringutils.hpp"
#include "fwk/toolkit/widgets/ratinglabel.hpp"

// remove
#include "engine/db/properties.hpp"

#include "metadatawidget.hpp"



namespace fwk {

MetaDataWidget::MetaDataWidget(const Glib::ustring & title)
    : ToolboxItemWidget(title),
      m_table(1, 2, false),
      m_fmt(NULL),
      m_update(false)
{
    add(m_table);
}

void MetaDataWidget::set_data_format(const MetaDataSectionFormat * fmt)
{
    m_fmt = fmt;
}

void MetaDataWidget::clear_widget(std::pair<const PropertyIndex, Gtk::Widget *> & p)
{
    AutoFlag flag(m_update);
    Gtk::Label * l = dynamic_cast<Gtk::Label*>(p.second);
    if(l) {
        l->set_text("");
        return;
    }
    Gtk::Entry * e = dynamic_cast<Gtk::Entry*>(p.second);
    if(e) {
        e->set_text("");
        return;
    }
    fwk::RatingLabel * rl = dynamic_cast<fwk::RatingLabel*>(p.second);
    if(rl) {
        rl->set_rating(0);
        return;
    }
}

void MetaDataWidget::set_data_source(const fwk::PropertyBag & properties)
{
    DBG_OUT("set data source");
    if(!m_data_map.empty()) {
        std::for_each(m_data_map.begin(), m_data_map.end(),
                      boost::bind(&MetaDataWidget::clear_widget, this, _1));
    }
    if(properties.empty()) {
        return;
    }
    if(!m_fmt) {
        DBG_OUT("empty format");
        return;
    }

    const MetaDataFormat * current = m_fmt->formats;
    while(current && current->label) {
        PropertyValue v;
        if(properties.get_value_for_property(current->id, v) || !current->readonly) {
            add_data(current, v);
        }
        else {
            DBG_OUT("get_property failed id = %d, label = %s",
                    current->id, current->label);
        }
        current++;
    }
}


void MetaDataWidget::add_data(const MetaDataFormat * current,
                              const PropertyValue & value)
{
    Gtk::Widget *w = NULL;
    int n_row;
    std::map<PropertyIndex, Gtk::Widget *>::iterator iter 
        = m_data_map.end();
    if(m_data_map.empty()) {
        n_row = 0;
    }
    else {
        iter = m_data_map.find(current->id);
        n_row = m_table.property_n_rows();
    }
    if(iter == m_data_map.end()) {
        Gtk::Label *labelw = Gtk::manage(new Gtk::Label(
                                             Glib::ustring("<b>") 
                                             + current->label + "</b>"));
        labelw->set_alignment(0.0f, 0.5f);
        labelw->set_use_markup(true);

        if(current->type == META_DT_STAR_RATING) {
            fwk::RatingLabel * r = Gtk::manage(new fwk::RatingLabel(0, !current->readonly));
            if(!current->readonly) {
                r->signal_changed.connect(
                    sigc::bind(
                        sigc::mem_fun(*this, 
                                      &MetaDataWidget::on_int_changed), 
                        current->id));
            }
            w = r;
        }
        else {
            if(current->readonly) {
                Gtk::Label * l = Gtk::manage(new Gtk::Label());
                l->set_alignment(0.0f, 0.5f);
                w = l;
            }
            else {
                Gtk::Entry * e = Gtk::manage(new Gtk::Entry());
                e->signal_focus_out_event().connect(
                    sigc::bind(
                        sigc::mem_fun(*this, 
                                      &MetaDataWidget::on_str_changed),
                        e, current->id));
                w = e;
            }
        }

        m_table.resize(n_row + 1, 2);
        m_table.attach(*labelw, 0, 1, n_row, n_row+1, 
                       Gtk::FILL, Gtk::SHRINK, 4, 0);
        m_table.attach(*w, 1, 2, n_row, n_row+1, 
                       Gtk::EXPAND|Gtk::FILL, Gtk::SHRINK, 4, 0);
        m_data_map.insert(std::make_pair(current->id, w));
    }
    else {
        w = static_cast<Gtk::Label*>(iter->second);
    }
    switch(current->type) {
    case META_DT_FRAC:
    {
        try {
            double decimal = fwk::fraction_to_decimal(boost::get<std::string>(value));
            std::string frac = boost::lexical_cast<std::string>(decimal);
            AutoFlag flag(m_update);
            static_cast<Gtk::Label*>(w)->set_text(frac);
        }
        catch(...) {
            DBG_OUT("conversion of '%u' to frac failed", current->id);
        }
        break;
    }
    case META_DT_STAR_RATING:
    {
        try {
            int rating = boost::get<int>(value);
            AutoFlag flag(m_update);
            static_cast<fwk::RatingLabel*>(w)->set_rating(rating);
        }
        catch(...) {
            DBG_OUT("conversion of '%u' to int failed", current->id);
        }
        break;
    }
    default:
        try {
            AutoFlag flag(m_update);
            if(current->readonly) {
                static_cast<Gtk::Label*>(w)->set_text(boost::get<std::string>(value));
            }
            else {
                static_cast<Gtk::Entry*>(w)->set_text(boost::get<std::string>(value));
            }
        }
        catch(...) {
            DBG_OUT("conversion of '%u' to string failed", current->id);
        }
        break;
    }
    m_table.show_all();
}

bool MetaDataWidget::on_str_changed(GdkEventFocus*, Gtk::Entry *e, fwk::PropertyIndex prop)
{
    if(m_update) {
        return true;
    }
    fwk::PropertyBag props;
    props.set_value_for_property(prop, fwk::PropertyValue(e->get_text()));
    signal_metadata_changed.emit(props);
    return true;
}

void MetaDataWidget::on_int_changed(int value, fwk::PropertyIndex prop)
{
    if(m_update) {
        return;
    }
    fwk::PropertyBag props;
    props.set_value_for_property(prop, fwk::PropertyValue(value));
    signal_metadata_changed.emit(props);
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
