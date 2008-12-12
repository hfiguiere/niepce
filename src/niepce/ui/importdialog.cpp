/*
 * niepce - ui/importdialog.cpp
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


#include <boost/bind.hpp>

#include <glibmm/i18n.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/button.h>
#include <gtkmm/stock.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/combobox.h>
#include <gtkmm/label.h>

#include "fwk/toolkit/configuration.h"
#include "fwk/toolkit/application.h"
#include "importdialog.h"

using framework::Configuration;
using framework::Application;

namespace ui {

ImportDialog::ImportDialog()
	: m_date_tz_combo(NULL),
	  m_ufraw_import_check(NULL),
	  m_rawstudio_import_check(NULL),
	  m_directory_name(NULL)
{
}


Gtk::Widget * ImportDialog::buildWidget()
{
	Glib::RefPtr<Gnome::Glade::Xml> xml 
		= Gnome::Glade::Xml::create(GLADEDIR"importdialog.glade");
	Gtk::Dialog *dlg = NULL;
	dlg = xml->get_widget("importDialog", dlg);

	Gtk::Button *select_directories = xml->get_widget("select_directories",
													  select_directories);
	select_directories->signal_clicked().connect(
			boost::bind(&ImportDialog::do_select_directories, this));
	m_date_tz_combo = xml->get_widget("date_tz_combo", m_date_tz_combo);
	m_ufraw_import_check = xml->get_widget("ufraw_import_check", 
										   m_ufraw_import_check);
	m_rawstudio_import_check = xml->get_widget("rawstudio_import_check",
											   m_rawstudio_import_check);
	m_directory_name = xml->get_widget("directory_name",
									   m_directory_name);
	
	return dlg;
}


void ImportDialog::do_select_directories()
{
	Configuration & cfg = Application::app()->config();
	
	Gtk::FileChooserDialog dialog(gtkWindow(), _("Import picture folder"),
								  Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
	
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button(_("Import"), Gtk::RESPONSE_OK);
	
	std::string last_import_location;
	last_import_location = cfg.getValue("last_import_location", "");
	if(!last_import_location.empty()) {
		dialog.set_filename(last_import_location);
	}
	
	int result = dialog.run();
	switch(result)
	{
	case Gtk::RESPONSE_OK:
		m_to_import = dialog.get_filename();
		m_directory_name->set_label(m_to_import);
		break;
	default:
		break;
	}
}


}

