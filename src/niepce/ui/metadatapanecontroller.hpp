/*
 * niepce - ui/metadatapanecontroller.h
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

#ifndef __UI_METADATAPANECONTROLLER_H__
#define __UI_METADATAPANECONTROLLER_H__

#include <gtkmm/box.h>

#include "engine/db/librarytypes.hpp"
#include "engine/db/libmetadata.hpp"
#include "fwk/utils/exempi.hpp"
#include "fwk/toolkit/dockable.hpp"

namespace fwk {
struct MetaDataSectionFormat;
class MetaDataWidget;
class Dock;
class PropertyBag;
}

namespace ui {
	
class MetaDataPaneController
    : public fwk::Dockable
{
public:
    typedef std::tr1::shared_ptr<MetaDataPaneController> Ptr;
    MetaDataPaneController();
    ~MetaDataPaneController();
    virtual Gtk::Widget * buildWidget(const Glib::RefPtr<Gtk::UIManager> &);
    void display(eng::library_id_t file_id, const eng::LibMetadata::Ptr & meta);
    eng::library_id_t displayed_file() const 
        { return m_fileid; }

    sigc::signal<void, const fwk::PropertyBag &, const fwk::PropertyBag &> signal_metadata_changed;
private:
    void on_metadata_changed(const fwk::PropertyBag &, 
                             const fwk::PropertyBag & old);

    std::vector<fwk::MetaDataWidget *> m_widgets;
    
    static const fwk::MetaDataSectionFormat * get_format();
    static const fwk::PropertySet & get_property_set();
    
    eng::library_id_t m_fileid;
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
