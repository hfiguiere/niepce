/*
 * niepce - ui/metadatapanecontroller.cpp
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

#include <glibmm/i18n.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <boost/bind.hpp>
#include <exempi/xmpconsts.h>
#include "utils/debug.h"
#include "utils/exempi.h"
#include "framework/metadatawidget.h"
#include "metadatapanecontroller.h"

using namespace xmp;

namespace ui {

	
	const MetaDataSectionFormat *
	MetaDataPaneController::get_format() 
	{
		static const MetaDataFormat s_camerainfo_format[] = {
			{ _("Make:"), NS_TIFF, "Make", META_DT_STRING, true },
			{ _("Model:"), NS_TIFF, "Model", META_DT_STRING, true },
			{ _("Lens:"), NS_EXIF_AUX, "Lens", META_DT_STRING, true },
			{ NULL, NULL, NULL, META_DT_NONE, true }
		};
		static const MetaDataFormat s_shootinginfo_format[] = {
			{ _("Exposure Program:"), NS_EXIF, "ExposureProgram", META_DT_STRING, true },
			{ _("Speed:"), NS_EXIF, "ExposureTime", META_DT_STRING, true },
			{ _("Aperture:"), NS_EXIF, "FNumber", META_DT_STRING, true },
			{ _("ISO:"), NS_EXIF, "ISOSpeedRatings", META_DT_STRING_ARRAY, true },
			{ _("Exposure Bias:"), NS_EXIF, "ExposureBiasValue", META_DT_STRING, true },
			// this one is fishy as it hardcode the prefix.
			{ _("Flash:"), NS_EXIF, "Flash/exif:Fired", META_DT_STRING, true },
			{ _("Flash compensation:"), NS_EXIF_AUX, "FlashCompensation", META_DT_STRING, true },
			{ _("Focal length:"), NS_EXIF, "FocalLength", META_DT_STRING, true },
			{ _("White balance:"), NS_EXIF, "WhiteBalance", META_DT_STRING, true },
			{ _("Date:"), NS_EXIF, "DateTimeOriginal", META_DT_DATE, false },
			{ NULL, NULL, NULL, META_DT_NONE, true }
		};
		static const MetaDataFormat s_iptc_format[] = {
			{ _("Rating:"), NS_XAP, "Rating", META_DT_STAR_RATING, false },
			{ _("Keywords:"), NS_DC, "subject", META_DT_STRING_ARRAY, false },
			{ NULL, NULL, NULL, META_DT_NONE, true }			
		};
		static const MetaDataSectionFormat s_format[] = {
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

	MetaDataPaneController::MetaDataPaneController()
		: m_metapane(false)
	{
	}

	
	Gtk::Widget * MetaDataPaneController::buildWidget()
	{
		m_widget = &m_metapane;

		const MetaDataSectionFormat * formats = get_format();
		
		const MetaDataSectionFormat * current = formats;
		while(current->section) {
			framework::MetaDataWidget *w = Gtk::manage(new framework::MetaDataWidget(current->section));
			m_metapane.pack_start(*w, Gtk::PACK_SHRINK, 0);
			w->set_data_format(current);
			m_widgets.push_back(w);
			current++;
		}

		return &m_metapane;
	}


	void MetaDataPaneController::display(const utils::XmpMeta * meta)
	{
		DBG_OUT("displaying metadata");
		std::for_each(m_widgets.begin(), m_widgets.end(),
					  boost::bind(&framework::MetaDataWidget::set_data_source,
								  _1, meta));
	}

}
