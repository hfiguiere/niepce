/*
 * niepce - niepce/ui/metadatapanecontroller.cpp
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

#include <boost/bind.hpp>

#include <glibmm/i18n.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <gtkmm/stock.h>

#include "fwk/base/debug.hpp"
#include "fwk/toolkit/metadatawidget.hpp"
#include "engine/db/properties.hpp"
#include "engine/db/xmpproperties.hpp"
#include "metadatapanecontroller.hpp"

namespace ui {
	
const fwk::MetaDataSectionFormat *
MetaDataPaneController::get_format() 
{
    static const fwk::MetaDataFormat s_camerainfo_format[] = {
        { _("Make:"), eng::NpTiffMakeProp, fwk::META_DT_STRING, true },
        { _("Model:"), eng::NpTiffModelProp, fwk::META_DT_STRING, true },
        { _("Lens:"), eng::NpExifAuxLensProp, fwk::META_DT_STRING, true },
        { NULL, 0, fwk::META_DT_NONE, true }
    };
    static const fwk::MetaDataFormat s_shootinginfo_format[] = {
        { _("Exposure Program:"), eng::NpExifExposureProgramProp, fwk::META_DT_STRING, true },
        { _("Speed:"), eng::NpExifExposureTimeProp, fwk::META_DT_STRING, true },
        { _("Aperture:"), eng::NpExifFNumberPropProp, fwk::META_DT_FRAC, true },
        { _("ISO:"), eng::NpExifIsoSpeedRatingsProp, fwk::META_DT_STRING_ARRAY, true },
        { _("Exposure Bias:"), eng::NpExifExposureBiasProp, fwk::META_DT_FRAC, true },
        { _("Flash:"), eng::NpExifFlashFiredProp, fwk::META_DT_STRING, true },
        { _("Flash compensation:"), eng::NpExifAuxFlashCompensationProp, fwk::META_DT_STRING, true },
        { _("Focal length:"), eng::NpExifFocalLengthProp, fwk::META_DT_FRAC, true },
        { _("White balance:"), eng::NpExifWbProp, fwk::META_DT_STRING, true },
        { _("Date:"), eng::NpExifDateTimeOriginalProp, fwk::META_DT_DATE, false },
        { NULL, 0, fwk::META_DT_NONE, true }
    };
    static const fwk::MetaDataFormat s_iptc_format[] = {
        { _("Headline:"), eng::NpIptcHeadlineProp, fwk::META_DT_STRING, false },
        { _("Caption:"), eng::NpIptcDescriptionProp, fwk::META_DT_TEXT, false },
        { _("Rating:"), eng::NpXmpRatingProp, fwk::META_DT_STAR_RATING, false },
        { _("Label:"), eng::NpXmpLabelProp, fwk::META_DT_STRING, false },            
        { _("Keywords:"), eng::NpIptcKeywordsProp, fwk::META_DT_STRING_ARRAY, false },
        { NULL, 0, fwk::META_DT_NONE, true }			
    };
    static const fwk::MetaDataSectionFormat s_format[] = {
        { _("Camera Information"),
          s_camerainfo_format
        },
        { _("Shooting Information"),
          s_shootinginfo_format
        },
        { _("IPTC"),
          s_iptc_format
        },
        { _("Rights"),
          NULL
        },
        { NULL, NULL
        }
    };
    return s_format;
}

const fwk::PropertySet & MetaDataPaneController::get_property_set()
{
    static fwk::PropertySet propset;
    if(propset.empty()) {
        const fwk::MetaDataSectionFormat * formats = get_format();
        
        const fwk::MetaDataSectionFormat * current = formats;
        while(current->section) {
            const fwk::MetaDataFormat * format = current->formats;
            while(format && format->label) {
                propset.insert(format->id);
                format++;
            }
            current++;
        }
    }
    return propset;
}

  
MetaDataPaneController::MetaDataPaneController()
    : Dockable("Metadata", _("Image Properties"), 
	       Gtk::Stock::PROPERTIES.id /*, DockItem::DOCKED_STATE*/),
      m_fileid(0)
{
}

MetaDataPaneController::~MetaDataPaneController()
{
}

Gtk::Widget * 
MetaDataPaneController::buildWidget(const Glib::RefPtr<Gtk::UIManager> & )
{
    if(m_widget) {
        return m_widget;
    }
    Gtk::VBox *vbox = build_vbox();
    m_widget = vbox;
    DBG_ASSERT(vbox, "dockable vbox not found");
    
    const fwk::MetaDataSectionFormat * formats = get_format();
    
    const fwk::MetaDataSectionFormat * current = formats;
    while(current->section) {
        fwk::MetaDataWidget *w = Gtk::manage(new fwk::MetaDataWidget(current->section));
        vbox->pack_start(*w, Gtk::PACK_SHRINK, 0);
        w->set_data_format(current);
        m_widgets.push_back(w);
        w->signal_metadata_changed.connect(
            sigc::mem_fun(*this, 
                          &MetaDataPaneController::on_metadata_changed));
        current++;
    }
    
    return m_widget;
}
  
void MetaDataPaneController::on_metadata_changed(const fwk::PropertyBag & props)
{
    signal_metadata_changed.emit(props);
}


void MetaDataPaneController::display(eng::library_id_t file_id, const fwk::XmpMeta * meta)
{
    m_fileid = file_id;
    DBG_OUT("displaying metadata");
    fwk::PropertyBag properties;
    if(meta) {
        const fwk::PropertySet & propset = get_property_set();
        eng::convert_xmp_to_properties(meta, propset, properties);
    }
    std::for_each(m_widgets.begin(), m_widgets.end(),
		  boost::bind(&fwk::MetaDataWidget::set_data_source,
			      _1, properties));
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
