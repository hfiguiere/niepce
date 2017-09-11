/*
 * niepce - fwk/toolkit/metadatawidget.cpp
 *
 * Copyright (C) 2008-2017 Hubert Figuiere
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
#include <boost/format.hpp>
#include <boost/rational.hpp>

#include <glibmm/i18n.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <gtkmm/textview.h>

#include "fwk/base/debug.hpp"
#include "fwk/base/autoflag.hpp"
#include "fwk/base/fractions.hpp"
#include "fwk/utils/exempi.hpp"
#include "fwk/utils/stringutils.hpp"
#include "fwk/toolkit/widgets/ratinglabel.hpp"
#include "fwk/toolkit/widgets/notabtextview.hpp"
#include "fwk/toolkit/widgets/tokentextview.hpp"

// remove
//#include "engine/db/properties.hpp"

#include "metadatawidget.hpp"



namespace fwk {

MetaDataWidget::MetaDataWidget(const Glib::ustring & title)
    : ToolboxItemWidget(title),
      m_fmt(nullptr),
      m_update(false)
{
    m_table.set_column_homogeneous(true);
    m_table.set_row_homogeneous(false);
    m_table.insert_column(0);
    m_table.insert_column(0);
    m_table.set_margin_start(8);
    add(m_table);
    set_sensitive(false);
}

void MetaDataWidget::set_data_format(const MetaDataSectionFormat * fmt)
{
    m_fmt = fmt;
    create_widgets_for_format(fmt);
}

Gtk::Widget* 
MetaDataWidget::create_star_rating_widget(bool readonly, uint32_t id)
{
    fwk::RatingLabel* r = Gtk::manage(new fwk::RatingLabel(0, !readonly));
    if(!readonly) {
        r->signal_changed.connect(
            sigc::bind(
                sigc::mem_fun(*this, 
                              &MetaDataWidget::on_int_changed), 
                id));
    }
    return r;
}

Gtk::Widget*
MetaDataWidget::create_string_array_widget(bool readonly, uint32_t id)
{
    fwk::TokenTextView * ttv = Gtk::manage(new fwk::TokenTextView());
    if(!readonly) {
        ttv->signal_focus_out_event().connect(
            sigc::bind(
                sigc::mem_fun(*this, 
                              &MetaDataWidget::on_string_array_changed),
                ttv, id));
    }
    return ttv;
}

Gtk::Widget*
MetaDataWidget::create_text_widget(bool readonly, uint32_t id)
{
    if(readonly) {
        Gtk::Label * l = Gtk::manage(new Gtk::Label());
        l->set_xalign(0.0f);
        l->set_yalign(0.5f);
        return l;
    }

    Gtk::TextView * e = Gtk::manage(new NoTabTextView());
    e->set_editable(true);
    e->set_wrap_mode(Gtk::WRAP_WORD);
    e->signal_focus_out_event().connect(
        sigc::bind(
            sigc::mem_fun(*this,
                          &MetaDataWidget::on_text_changed),
            e->get_buffer(), id));
    return e;
}

Gtk::Widget*
MetaDataWidget::create_string_widget(bool readonly, uint32_t id)
{
    if(readonly) {
        Gtk::Label * l = Gtk::manage(new Gtk::Label());
        l->set_xalign(0.0f);
        l->set_yalign(0.5f);
        return l;
    }

    Gtk::Entry * e = Gtk::manage(new Gtk::Entry());
    e->set_has_frame(false); // TODO make that a custom widget
    e->signal_focus_out_event().connect(
        sigc::bind(
            sigc::mem_fun(*this,
                          &MetaDataWidget::on_str_changed),
            e, id));
    return e;
}

Gtk::Widget*
MetaDataWidget::create_date_widget(bool /*readonly*/, uint32_t id)
{
    // for now a date widget is just like a string. Read only
    return create_string_widget(true, id);
}

void
MetaDataWidget::create_widgets_for_format(const MetaDataSectionFormat * fmt)
{
    Gtk::Widget *w = nullptr;
    const MetaDataFormat * current = fmt->formats;
    int n_row = 0;

    while(current && current->label) {
        Gtk::Label *labelw = Gtk::manage(new Gtk::Label(
                                             Glib::ustring("<b>")
                                             + current->label + "</b>"));
        if(current->type != MetaDT::STRING_ARRAY
           && current->type != MetaDT::TEXT) {
            labelw->set_xalign(0.0f);
            labelw->set_yalign(0.5f);
        }
        else {
            // Text can wrap. Different alignment for the label
            labelw->set_xalign(0.0f);
            labelw->set_yalign(0.0f);
        }
        labelw->set_use_markup(true);

        switch(current->type) {
        case MetaDT::STAR_RATING:
            w = create_star_rating_widget(current->readonly, current->id);
            break;
        case MetaDT::STRING_ARRAY:
            w = create_string_array_widget(current->readonly, current->id);
            break;
        case MetaDT::TEXT:
            w = create_text_widget(current->readonly, current->id);
            break;
        case MetaDT::DATE:
            w = create_date_widget(current->readonly, current->id);
            break;
        default:
            w = create_string_widget(current->readonly, current->id);
            break;
        }

        m_table.insert_row(n_row + 1);
        m_table.attach(*labelw, 0, n_row, 1, 1);
        m_table.attach_next_to(*w, *labelw, Gtk::POS_RIGHT, 1, 1);
        m_data_map.insert(std::make_pair(current->id, w));

        current++;
        n_row++;
    }
    m_table.show_all();
}

void MetaDataWidget::clear_widget(const std::pair<const PropertyIndex, Gtk::Widget *> & p)
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
    fwk::TokenTextView *ttv = dynamic_cast<fwk::TokenTextView*>(p.second);
    if(ttv) {
        ttv->set_tokens(fwk::TokenTextView::Tokens());
        return;
    }
    Gtk::TextView * tv = dynamic_cast<Gtk::TextView*>(p.second);
    if(tv) {
        tv->get_buffer()->set_text("");
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
    m_current_data = properties;
    if(!m_data_map.empty()) {
        std::for_each(m_data_map.cbegin(), m_data_map.cend(),
                      [this] (const auto& p) {
                          this->clear_widget(p);
                      });
    }
    set_sensitive(!properties.empty());
    if(properties.empty()) {
        return;
    }
    if(!m_fmt) {
        DBG_OUT("empty format");
        return;
    }

    const MetaDataFormat * current = m_fmt->formats;
    while(current && current->label) {
        auto result = properties.get_value_for_property(current->id);
        if(!result.empty() || !current->readonly) {
            add_data(current, result.unwrap());
        }
        else {
            DBG_OUT("get_property failed id = %d, label = %s",
                    current->id, current->label);
        }
        current++;
    }
}

bool MetaDataWidget::set_fraction_dec_data(Gtk::Widget* w,
                                           const PropertyValue & value)
{
    if (!is_string(value)) {
        ERR_OUT("Data not string(fraction)");
        return false;
    }
    try {
        const std::string& str_value = get_string(value);
        DBG_OUT("set fraction dec %s", str_value.c_str());
        std::string frac = str(boost::format("%.1f")
                               % fwk_fraction_to_decimal(str_value.c_str()));
        AutoFlag flag(m_update);
        static_cast<Gtk::Label*>(w)->set_text(frac);
    }
    catch(...) {
        return false;
    }
    return true;
}

bool MetaDataWidget::set_fraction_data(Gtk::Widget* w,
                                       const PropertyValue & value)
{
    if (!is_string(value)) {
        ERR_OUT("Data not string(fraction)");
        return false;
    }
    try {
        const std::string& str_value = get_string(value);
        DBG_OUT("set fraction %s", str_value.c_str());
        boost::rational<int> r
            = boost::lexical_cast<boost::rational<int> >(str_value);

        std::string frac = str(boost::format("%1%/%2%")
                               % r.numerator() % r.denominator());
        AutoFlag flag(m_update);
        static_cast<Gtk::Label*>(w)->set_text(frac);
    }
    catch(...) {
        return false;
    }
    return true;
}

bool MetaDataWidget::set_star_rating_data(Gtk::Widget* w,
                                          const PropertyValue & value)
{
    if (!is_integer(value)) {
        ERR_OUT("Data not integer");
        return false;
    }
    try {
        int rating = get_integer(value);
        AutoFlag flag(m_update);
        static_cast<fwk::RatingLabel*>(w)->set_rating(rating);
    }
    catch(...) {
        return false;
    }
    return true;
}

bool MetaDataWidget::set_string_array_data(Gtk::Widget* w, const PropertyValue & value)
{
    try {
        AutoFlag flag(m_update);
        fwk::StringArray tokens = boost::get<fwk::StringArray>(value.get_variant());

        static_cast<fwk::TokenTextView*>(w)->set_tokens(tokens);
    }
    catch(...) {
        return false;
    }
    return true;
}

bool MetaDataWidget::set_text_data(Gtk::Widget* w, bool readonly,
                                   const PropertyValue & value)
{
    if (!is_string(value)) {
        ERR_OUT("Data not string");
        return false;
    }
    try {
        AutoFlag flag(m_update);
        if(readonly) {
            static_cast<Gtk::Label*>(w)->set_text(get_string(value));
        }
        else {
            static_cast<Gtk::TextView*>(w)->get_buffer()->set_text(get_string(value));
        }
    }
    catch(...) {
        return false;
    }
    return true;
}

bool MetaDataWidget::set_string_data(Gtk::Widget* w, bool readonly,
                                     const PropertyValue & value)
{
    if (!is_string(value)) {
        ERR_OUT("Data not string");
        return false;
    }
    try {
        AutoFlag flag(m_update);
        if(readonly) {
            static_cast<Gtk::Label*>(w)->set_text(get_string(value));
        }
        else {
            static_cast<Gtk::Entry*>(w)->set_text(get_string(value));
        }
    }
    catch(...) {
        return false;
    }
    return true;
}

bool MetaDataWidget::set_date_data(Gtk::Widget* w, const PropertyValue & value)
{
    try {
        AutoFlag flag(m_update);
        fwk::DatePtr date = boost::get<fwk::DatePtr>(value.get_variant());
        static_cast<Gtk::Label*>(w)->set_text(fwk::date_to_string(date.get()));

        DBG_OUT("setting date data %s", fwk::date_to_string(date.get()).c_str());
    }
    catch(...) {
        return false;
    }
    return true;
}

void MetaDataWidget::add_data(const MetaDataFormat * current,
                              const PropertyValue & value)
{
    if (is_empty(value)) {
        return;
    }

    Gtk::Widget *w = nullptr;
    auto iter = m_data_map.find(current->id);
    if(iter == m_data_map.end()) {
        ERR_OUT("no widget for property");
        return;
    }

    w = static_cast<Gtk::Label*>(iter->second);

    switch(current->type) {
    case MetaDT::FRAC_DEC:
        set_fraction_dec_data(w, value);
        break;
    case MetaDT::FRAC:
        set_fraction_data(w, value);
        break;
    case MetaDT::STAR_RATING:
        set_star_rating_data(w, value);
        break;
    case MetaDT::STRING_ARRAY:
        set_string_array_data(w, value);
        break;
    case MetaDT::TEXT:
        set_text_data(w, current->readonly, value);
        break;
    case MetaDT::DATE:
        set_date_data(w, value);
        break;
    default:
        set_string_data(w, current->readonly, value);
        break;
    }
}

bool MetaDataWidget::on_str_changed(GdkEventFocus*, Gtk::Entry *e, 
                                    fwk::PropertyIndex prop)
{
    if(m_update) {
        return true;
    }
    emit_metadata_changed(prop, fwk::PropertyValue(e->get_text()));
    return true;
}

bool MetaDataWidget::on_text_changed(GdkEventFocus*, 
                                     Glib::RefPtr<Gtk::TextBuffer> b, 
                                     fwk::PropertyIndex prop)
{
    if(m_update) {
        return true;
    }
    emit_metadata_changed(prop,
                          fwk::PropertyValue(b->get_text()));
    return true;
}

bool MetaDataWidget::on_string_array_changed(GdkEventFocus*, 
                                             fwk::TokenTextView * ttv,
                                             fwk::PropertyIndex prop)
{
    if(m_update) {
        return true;
    }
    fwk::TokenTextView::Tokens tok;
    ttv->get_tokens(tok);
    emit_metadata_changed(prop, 
                          fwk::PropertyValue(tok));
    return true;
}

void MetaDataWidget::on_int_changed(int value, fwk::PropertyIndex prop)
{
    if(m_update) {
        return;
    }
    emit_metadata_changed(prop, fwk::PropertyValue(value));
}

void MetaDataWidget::emit_metadata_changed(fwk::PropertyIndex prop,
                                           const fwk::PropertyValue & value)
{
    fwk::PropertyBag props, old_props;
    props.set_value_for_property(prop, value);
    auto result = m_current_data.get_value_for_property(prop);
    if (!result.empty()) {
        old_props.set_value_for_property(prop, result.unwrap());
    }
    signal_metadata_changed.emit(props, old_props);
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
