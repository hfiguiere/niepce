/*
 * niepce - niepce/ui/importdialog.cpp
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
#include <gtkmm/button.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/combobox.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/iconview.h>
#include <gtkmm/label.h>
#include <gtkmm/liststore.h>
#include <gtkmm/stock.h>
#include <gtkmm/builder.h>

#include "fwk/base/debug.hpp"
#include "fwk/utils/pathutils.hpp"
#include "fwk/toolkit/configuration.hpp"
#include "fwk/toolkit/application.hpp"
#include "importdialog.hpp"

using fwk::Configuration;
using fwk::Application;

namespace ui {

ImportDialog::ImportDialog()
    : fwk::Dialog(GLADEDIR"importdialog.ui", "importDialog"),
      m_date_tz_combo(NULL),
	  m_ufraw_import_check(NULL),
	  m_rawstudio_import_check(NULL),
	  m_directory_name(NULL),
      m_imagesList(NULL)
{
}


void ImportDialog::setup_widget()
{
    if(m_is_setup) {
        return;
    }

    add_header(_("Import"));

    Glib::RefPtr<Gtk::Builder> _builder = builder();
    Gtk::Button *select_directories = NULL;

    _builder->get_widget("select_directories", select_directories);
    select_directories->signal_clicked().connect(
        sigc::mem_fun(*this, &ImportDialog::do_select_directories));
    _builder->get_widget("date_tz_combo", m_date_tz_combo);
    _builder->get_widget("ufraw_import_check", m_ufraw_import_check);
    _builder->get_widget("rawstudio_import_check", m_rawstudio_import_check);
    _builder->get_widget("directory_name", m_directory_name);
    _builder->get_widget("imagesList", m_imagesList);
    _builder->get_widget("destinationFolder", m_destinationFolder);
    m_imagesListModel = m_imagesListModelRecord.inject(*m_imagesList);
    m_imagesList->set_model(m_imagesListModel);
    m_is_setup = true;
}


void ImportDialog::do_select_directories()
{
    Configuration & cfg = Application::app()->config();
	
    Gtk::FileChooserDialog dialog(gtkWindow(), _("Import picture folder"),
                                  Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
	
    dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    dialog.add_button(_("Import"), Gtk::RESPONSE_OK);
    dialog.set_select_multiple(false);
    std::string last_import_location;
    last_import_location = cfg.getValue("last_import_location", "");
    if(!last_import_location.empty()) {
        dialog.set_filename(last_import_location);
    }
	
    int result = dialog.run();
    switch(result)
    {
    case Gtk::RESPONSE_OK:
        set_to_import(dialog.get_filename());
        break;
    default:
        break;
    }
}

void ImportDialog::set_to_import(const Glib::ustring & f)
{
    m_folder_path_to_import = f;
    m_destinationFolder->set_text(fwk::path_basename(f));
    m_directory_name->set_text(f);
//
    m_imagesListModel->clear();
    fwk::FileList::Ptr list_to_import 
        = fwk::FileList::getFilesFromDirectory(f, 
                                               boost::bind(&fwk::filter_xmp_out, _1));
    for(std::list<std::string>::const_iterator i = list_to_import->begin();
        i != list_to_import->end(); ++i) {
        DBG_OUT("selected %s", i->c_str());
        Gtk::TreeIter iter = m_imagesListModel->append();
        iter->set_value(m_imagesListModelRecord.m_col1, *i);
    }
}

}

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  tab-width:4
  fill-column:99
  End:
*/
