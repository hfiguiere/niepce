/*
 * niepce - niepce/ui/importdialog.cpp
 *
 * Copyright (C) 2008-2014 Hubert Figuiere
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


#include <future>
#include <functional>

#include <glibmm/i18n.h>
#include <gtkmm/button.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/combobox.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/iconview.h>
#include <gtkmm/label.h>
#include <gtkmm/liststore.h>
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
    : fwk::Dialog(GLADEDIR"importdialog.ui", "importDialog")
    , m_date_tz_combo(nullptr)
    , m_ufraw_import_check(nullptr)
    , m_rawstudio_import_check(nullptr)
    , m_directory_name(nullptr)
    , m_imagesList(nullptr)
{
}


void ImportDialog::setup_widget()
{
    if(m_is_setup) {
        return;
    }

    add_header(_("Import"));

    Glib::RefPtr<Gtk::Builder> a_builder = builder();
    Gtk::Button *select_directories = nullptr;

    a_builder->get_widget("select_directories", select_directories);
    select_directories->signal_clicked().connect(
        sigc::mem_fun(*this, &ImportDialog::doSelectDirectories));
    a_builder->get_widget("date_tz_combo", m_date_tz_combo);
    a_builder->get_widget("ufraw_import_check", m_ufraw_import_check);
    a_builder->get_widget("rawstudio_import_check", m_rawstudio_import_check);
    a_builder->get_widget("directory_name", m_directory_name);
    a_builder->get_widget("imagesList", m_imagesList);
    a_builder->get_widget("destinationFolder", m_destinationFolder);

    m_imagesListModel = m_imagesListModelRecord.inject(*m_imagesList);
    m_imagesList->set_model(m_imagesListModel);
    m_is_setup = true;
}


void ImportDialog::doSelectDirectories()
{
    Configuration & cfg = Application::app()->config();

    Gtk::FileChooserDialog dialog(gtkWindow(), _("Import picture folder"),
                                  Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);

    dialog.add_button(_("Cancel"), Gtk::RESPONSE_CANCEL);
    dialog.add_button(_("Import"), Gtk::RESPONSE_OK);
    dialog.set_select_multiple(false);

    std::string last_import_location = cfg.getValue("last_import_location", "");
    if(!last_import_location.empty()) {
        dialog.set_filename(last_import_location);
    }

    int result = dialog.run();
    switch(result)
    {
    case Gtk::RESPONSE_OK:
        setToImport(dialog.get_filename());
        break;
    default:
        break;
    }
}

void ImportDialog::setToImport(const Glib::ustring & f)
{

    auto future1 = std::async(std::launch::async,
                              [f] () {
                                  return fwk::FileList::getFilesFromDirectory(
                                      f, &fwk::filter_xmp_out);
                              });

    m_folder_path_to_import = f;
    m_destinationFolder->set_text(fwk::path_basename(f));
    m_directory_name->set_text(f);

    m_imagesListModel->clear();

    auto list_to_import = future1.get();

    std::for_each(list_to_import->begin(), list_to_import->end(),
                  [this] (const std::string & s) {
                      DBG_OUT("selected %s", s.c_str());
                      Gtk::TreeIter iter = m_imagesListModel->append();
                      iter->set_value(m_imagesListModelRecord.m_col1, s);
                  }
        );
}

}

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  tab-width:2
  fill-column:99
  End:
*/
