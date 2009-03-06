/*
 * niepce - fwk/toolkit/metadatawidget.cpp
 *
 * Copyright (C) 2008-2009 Hubert Figuiere
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

#include "fwk/utils/fractions.hpp"
#include "fwk/utils/exempi.h"
#include "fwk/utils/stringutils.h"
#include "fwk/utils/debug.h"

#include "metadatawidget.hpp"



namespace fwk {

MetaDataWidget::MetaDataWidget(const Glib::ustring & title)
    : ToolboxItemWidget(title),
      m_table(1, 2, false),
      m_fmt(NULL)
{
    add(m_table);
}

void MetaDataWidget::set_data_format(const xmp::MetaDataSectionFormat * fmt)
{
    m_fmt = fmt;
}

namespace {
static 
void clear_widget(std::pair<const std::string, Gtk::Widget *> & p)
{
    Gtk::Label * l = dynamic_cast<Gtk::Label*>(p.second);
    if(l) {
        l->set_text("");
    }
}
}

void MetaDataWidget::set_data_source(const utils::XmpMeta * xmp)
{
    DBG_OUT("set data source");
    if(!m_data_map.empty()) {
        std::for_each(m_data_map.begin(), m_data_map.end(),
                      boost::bind(&clear_widget, _1));
    }
    if(!xmp) {
        return;
    }
    if(!m_fmt) {
        DBG_OUT("empty format");
        return;
    }

    const xmp::MetaDataFormat * current = m_fmt->formats;
    xmp::ScopedPtr<XmpStringPtr> value(xmp_string_new());
    while(current && current->label) {
        std::string id(current->property);
        id += "-";
        id += current->ns;
        if(current->type == xmp::META_DT_STRING_ARRAY) {
            xmp::ScopedPtr<XmpIteratorPtr> 
                iter(xmp_iterator_new(xmp->xmp(), current->ns,
                                      current->property, XMP_ITER_JUSTLEAFNODES));
            std::vector<std::string> vec;
            while(xmp_iterator_next(iter, NULL, NULL, value, NULL)) {
                vec.push_back(xmp_string_cstr(value));
            }
            std::string v = utils::join(vec, ", ");
            add_data(id, current->label, v.c_str(), current->type);
        }
        else {
            const char * v = "";
            if(xmp_get_property(xmp->xmp(), current->ns,
                                current->property, value, NULL)) {
                v = xmp_string_cstr(value);
            }
            else {
                DBG_OUT("get_property failed id = %s, ns = %s, prop = %s,"
                        "label = %s",
                        id.c_str(), current->ns, current->property,
                        current->label);
            }
            add_data(id, current->label, v, current->type);
        }
        current++;
    }
}


void MetaDataWidget::add_data(const std::string & id, 
                              const std::string & label,
                              const char * value,
                              xmp::MetaDataType type)
{
    Gtk::Label *w = NULL;
    int n_row;
    std::map<std::string, Gtk::Widget *>::iterator iter 
        = m_data_map.end();
    if(m_data_map.empty()) {
        n_row = 0;
    }
    else {
        iter = m_data_map.find(id);
        n_row = m_table.property_n_rows();
    }
    if(iter == m_data_map.end()) {
        Gtk::Label *labelw = Gtk::manage(new Gtk::Label(
                                             Glib::ustring("<b>") 
                                             + label + "</b>"));
        labelw->set_alignment(0, 0.5);
        labelw->set_use_markup(true);

        w = Gtk::manage(new Gtk::Label());
        w->set_alignment(0, 0.5);

        m_table.resize(n_row + 1, 2);
        m_table.attach(*labelw, 0, 1, n_row, n_row+1, 
                       Gtk::FILL, Gtk::SHRINK, 4, 0);
        m_table.attach(*w, 1, 2, n_row, n_row+1, 
                       Gtk::EXPAND|Gtk::FILL, Gtk::SHRINK, 4, 0);
        m_data_map.insert(std::make_pair(id, w));
    }
    else {
        w = static_cast<Gtk::Label*>(iter->second);
    }
    switch(type) {
    case xmp::META_DT_FRAC:
    {
        double decimal = fwk::fraction_to_decimal(value);
        std::string frac = boost::lexical_cast<std::string>(decimal);
        w->set_text(frac);
        break;
    }
    default:
        w->set_text(value);
        break;
    }
    m_table.show_all();
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
