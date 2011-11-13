/*
 * niepce - fwk/toolkit/metadatawidget.h
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

#ifndef __NIEPCE_FRAMEWORK_META_DATA_WIDGET_H__
#define __NIEPCE_FRAMEWORK_META_DATA_WIDGET_H__


#include <map>
#include <string>

#include <gtkmm/table.h>
#include <gtkmm/textview.h>

#include "fwk/base/propertybag.hpp"
#include "fwk/toolkit/widgets/toolboxitemwidget.hpp"

namespace xmp {
struct MetaDataSectionFormat;
struct MetaDataFormat;
}

namespace fwk {

enum MetaDataType {
    META_DT_NONE = 0,
    META_DT_STRING,
    META_DT_STRING_ARRAY,
    META_DT_TEXT,
    META_DT_DATE,
    META_DT_FRAC, 
    META_DT_STAR_RATING
};

struct MetaDataFormat {
    const char * label;
    uint32_t     id;
    MetaDataType type;
    bool         readonly;
};
	
struct MetaDataSectionFormat {
    const char * section;
    const MetaDataFormat * formats;
};


class XmpMeta;

class MetaDataWidget 
	: public fwk::ToolboxItemWidget
{
public:
    MetaDataWidget(const Glib::ustring & title);
    
    void add_data(const MetaDataFormat * current,
                  const PropertyValue & value);
    void set_data_format(const MetaDataSectionFormat * fmt);
    void set_data_source(const fwk::PropertyBag & properties);
    
    sigc::signal<void, const fwk::PropertyBag &, const fwk::PropertyBag &> signal_metadata_changed;
protected:
    bool on_str_changed(GdkEventFocus*, Gtk::Entry *, fwk::PropertyIndex prop);
    bool on_text_changed(GdkEventFocus*, Glib::RefPtr<Gtk::TextBuffer> b, fwk::PropertyIndex prop);
    void on_int_changed(int, fwk::PropertyIndex prop);
private:
    void clear_widget(std::pair<const PropertyIndex, Gtk::Widget *> & p);
    void emit_metadata_changed(fwk::PropertyIndex prop, const fwk::PropertyValue & value);

    Gtk::Table    m_table;
    std::map<const PropertyIndex, Gtk::Widget *> m_data_map;
    fwk::PropertyBag m_current_data;
    const MetaDataSectionFormat * m_fmt;
    bool m_update;
};

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

#endif
