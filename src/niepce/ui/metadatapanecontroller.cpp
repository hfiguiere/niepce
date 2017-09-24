/*
 * niepce - niepce/ui/metadatapanecontroller.cpp
 *
 * Copyright (C) 2008-2013 Hubert Figuiere
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

#include <glibmm/i18n.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>

#include "fwk/base/debug.hpp"
#include "fwk/toolkit/metadatawidget.hpp"
#include "engine/db/properties.hpp"
#include "engine/db/libmetadata.hpp"
#include "metadatapanecontroller.hpp"

namespace ui {

const fwk::MetaDataSectionFormat *
MetaDataPaneController::get_format()
{
    static const fwk::MetaDataFormat s_camerainfo_format[] = {
        { _("Make:"), eng::NpTiffMakeProp, fwk::MetaDT::STRING, true },
        { _("Model:"), eng::NpTiffModelProp, fwk::MetaDT::STRING, true },
        { _("Lens:"), eng::NpExifAuxLensProp, fwk::MetaDT::STRING, true },
        { nullptr, 0, fwk::MetaDT::NONE, true }
    };
    static const fwk::MetaDataFormat s_shootinginfo_format[] = {
        { _("Exposure Program:"), eng::NpExifExposureProgramProp, fwk::MetaDT::STRING, true },
        { _("Speed:"), eng::NpExifExposureTimeProp, fwk::MetaDT::FRAC, true },
        { _("Aperture:"), eng::NpExifFNumberPropProp, fwk::MetaDT::FRAC_DEC, true },
        { _("ISO:"), eng::NpExifIsoSpeedRatingsProp, fwk::MetaDT::STRING, true },
        { _("Exposure Bias:"), eng::NpExifExposureBiasProp, fwk::MetaDT::FRAC_DEC, true },
        { _("Flash:"), eng::NpExifFlashFiredProp, fwk::MetaDT::STRING, true },
        { _("Flash compensation:"), eng::NpExifAuxFlashCompensationProp, fwk::MetaDT::STRING, true },
        { _("Focal length:"), eng::NpExifFocalLengthProp, fwk::MetaDT::FRAC_DEC, true },
        { _("White balance:"), eng::NpExifWbProp, fwk::MetaDT::STRING, true },
        { _("Date:"), eng::NpExifDateTimeOriginalProp, fwk::MetaDT::DATE, false },
        { nullptr, 0, fwk::MetaDT::NONE, true }
    };
    static const fwk::MetaDataFormat s_iptc_format[] = {
        { _("Headline:"), eng::NpIptcHeadlineProp, fwk::MetaDT::STRING, false },
        { _("Caption:"), eng::NpIptcDescriptionProp, fwk::MetaDT::TEXT, false },
        { _("Rating:"), eng::NpXmpRatingProp, fwk::MetaDT::STAR_RATING, false },
        // FIXME change this type to the right one when there is a widget
        { _("Label:"), eng::NpXmpLabelProp, fwk::MetaDT::STRING, true },
        { _("Keywords:"), eng::NpIptcKeywordsProp, fwk::MetaDT::STRING_ARRAY, false },
        { nullptr, 0, fwk::MetaDT::NONE, true }
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
          nullptr
        },
        { nullptr, nullptr
        }
    };
    return s_format;
}

const fwk::PropertySet* MetaDataPaneController::get_property_set()
{
    static fwk::PropertySet* propset = nullptr;
    if(!propset) {
        propset = ffi::fwk_property_set_new();
        const fwk::MetaDataSectionFormat * formats = get_format();

        const fwk::MetaDataSectionFormat * current = formats;
        while(current->section) {
            const fwk::MetaDataFormat * format = current->formats;
            while(format && format->label) {
                ffi::fwk_property_set_add(propset, format->id);
                format++;
            }
            current++;
        }
    }
    return propset;
}

MetaDataPaneController::MetaDataPaneController()
    : Dockable("Metadata", _("Image Properties"),
	       "document-properties" /*, DockItem::DOCKED_STATE*/),
      m_fileid(0)
{
}

MetaDataPaneController::~MetaDataPaneController()
{
}

Gtk::Widget * 
MetaDataPaneController::buildWidget()
{
    if(m_widget) {
        return m_widget;
    }
    auto box = build_vbox();
    m_widget = box;
    DBG_ASSERT(box, "dockable vbox not found");

    auto formats = get_format();

    auto current = formats;
    while(current->section) {
        auto w = Gtk::manage(new fwk::MetaDataWidget(current->section));
        box->pack_start(*w, Gtk::PACK_SHRINK, 0);
        w->set_data_format(current);
        m_widgets.push_back(w);
        w->signal_metadata_changed.connect(
            sigc::mem_fun(*this,
                          &MetaDataPaneController::on_metadata_changed));
        current++;
    }

    return m_widget;
}

void MetaDataPaneController::on_metadata_changed(const fwk::PropertyBagPtr& props,
                                                 const fwk::PropertyBagPtr& old)
{
    signal_metadata_changed.emit(props, old);
}

void MetaDataPaneController::display(eng::library_id_t file_id, const eng::LibMetadata* meta)
{
    m_fileid = file_id;
    DBG_OUT("displaying metadata");
    fwk::PropertyBagPtr properties;
    if(meta) {
        const fwk::PropertySet* propset = get_property_set();
        properties = eng::libmetadata_to_properties(meta, *propset);
    }
    std::for_each(m_widgets.begin(), m_widgets.end(),
                  [properties] (auto w) {
                      w->set_data_source(properties);
                  });
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
