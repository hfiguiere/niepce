/*
 * niepce - niepce/ui/dialogs/importdialog.cpp
 *
 * Copyright (C) 2008-2017 Hubert Figuière
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
#include "fwk/toolkit/widgets/imagegridview.hpp"
#include "engine/importer/directoryimporter.hpp"
#include "engine/importer/importedfile.hpp"
#include "importdialog.hpp"

using fwk::Configuration;
using fwk::Application;

namespace ui {

ImportDialog::ImportDialog()
    : fwk::Dialog(GLADEDIR"importdialog.ui", "importDialog")
    , m_importer(nullptr)
    , m_date_tz_combo(nullptr)
    , m_ufraw_import_check(nullptr)
    , m_rawstudio_import_check(nullptr)
    , m_directory_name(nullptr)
    , m_destinationFolder(nullptr)
    , m_attributesScrolled(nullptr)
    , m_images_list_scrolled(nullptr)
{
}

ImportDialog::~ImportDialog()
{
}

void ImportDialog::setup_widget()
{
    if(m_is_setup) {
        return;
    }

    Glib::RefPtr<Gtk::Builder> a_builder = builder();
    Gtk::Button *select_directories = nullptr;

    a_builder->get_widget("select_directories", select_directories);
    select_directories->signal_clicked().connect(
        sigc::mem_fun(*this, &ImportDialog::doSelectDirectories));
    a_builder->get_widget("date_tz_combo", m_date_tz_combo);
    a_builder->get_widget("ufraw_import_check", m_ufraw_import_check);
    a_builder->get_widget("rawstudio_import_check", m_rawstudio_import_check);
    a_builder->get_widget("directory_name", m_directory_name);
    a_builder->get_widget("destinationFolder", m_destinationFolder);

    // Metadata pane.
    a_builder->get_widget("attributes_scrolled", m_attributesScrolled);
    m_metadata_pane = MetaDataPaneController::Ptr(new MetaDataPaneController);
    auto w = m_metadata_pane->buildWidget();
    add(m_metadata_pane);
    m_attributesScrolled->add(*w);
    w->show_all();

    // Gridview of previews.
    a_builder->get_widget("images_list_scrolled", m_images_list_scrolled);
    m_images_list_model = Gtk::ListStore::create(m_grid_columns);
    m_gridview = Gtk::manage(
        new Gtk::IconView(
            Glib::RefPtr<Gtk::TreeModel>::cast_dynamic(m_images_list_model)));
    m_gridview->set_pixbuf_column(m_grid_columns.pixbuf);
    m_gridview->set_text_column(m_grid_columns.filename);
    m_gridview->show();
    m_images_list_scrolled->add(*m_gridview);
    m_images_list_scrolled->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

    m_previews_to_import.connect(
      sigc::mem_fun(this, &ImportDialog::preview_received));
    m_files_to_import.connect(
      sigc::mem_fun(this, &ImportDialog::append_files_to_import));

    m_is_setup = true;
}

// XXX doesn't belong here
void ImportDialog::doSelectDirectories()
{
  Configuration & cfg = Application::app()->config();

  Glib::ustring filename;
  {
    Gtk::FileChooserDialog dialog(gtkWindow(), _("Import picture folder"),
                                  Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);

    dialog.add_button(_("Cancel"), Gtk::RESPONSE_CANCEL);
    dialog.add_button(_("Select"), Gtk::RESPONSE_OK);
    dialog.set_select_multiple(false);

    std::string last_import_location = cfg.getValue("last_import_location", "");
    if (!last_import_location.empty()) {
      dialog.set_filename(last_import_location);
    }

    int result = dialog.run();
    switch(result)
    {
    case Gtk::RESPONSE_OK:
      filename = dialog.get_filename();
      break;
    default:
      break;
    }
  }
  if (!filename.empty()) {
    setToImport(filename);
  }
}

// XXX doesn't belong here. Or must be deeply modified to deal with the Importer
void ImportDialog::setToImport(const Glib::ustring & f)
{
    if (!m_importer) {
        // FIXME this should be the right kind
        m_importer = std::make_shared<eng::DirectoryImporter>();
    }

    eng::IImporter::SourceContentReady source_content_ready =
      [this] (std::list<eng::ImportedFile::Ptr>&& list_to_import) {
        this->m_files_to_import.send_data(std::move(list_to_import));
      };


    m_images_list_model->clear();
    m_images_list_map.clear();
    m_files_to_import.clear();

    auto importer = m_importer;
    auto source_content =
      std::async(std::launch::async,
                 [f, importer, source_content_ready] () {
                   return importer->listSourceContent(f, source_content_ready);
                 });

    m_folder_path_source = f;
    m_destinationFolder->set_text(fwk::path_basename(f));
    m_directory_name->set_text(f);
}

void ImportDialog::append_files_to_import()
{
  auto files_to_import = m_files_to_import.recv_data();

  if (!m_images_list_model) {
    ERR_OUT("No image list model");
    return;
  }
  // request the previews to the importer.tn.pixmap.pixbuf()
  std::list<std::string> paths;
  for(const auto & f : files_to_import) {
    DBG_OUT("selected %s", f->name().c_str());
    paths.push_back(f->name());
    Gtk::TreeIter iter = m_images_list_model->append();
    m_images_list_map.insert(std::make_pair(f->name(), iter));
    iter->set_value(m_grid_columns.filename, Glib::ustring(f->name()));
    iter->set_value(m_grid_columns.file, std::move(f));
  }

  eng::IImporter::PreviewReady preview_ready = [this] (const std::string& path,
                                                       const fwk::Thumbnail& thumbnail) {
    this->m_previews_to_import.send_data(std::make_pair(path, thumbnail));
  };

  auto importer = m_importer;
  auto source = m_folder_path_source.raw();
  std::async(std::launch::async,
             [importer, source, paths, preview_ready] () {
               return importer->get_previews_for(source, paths, preview_ready);
             });
}

void ImportDialog::preview_received()
{
  auto result = m_previews_to_import.recv_data();
  if (!result.empty()) {
    auto preview = result.unwrap();
    auto iter = m_images_list_map.find(preview.first);
    if (iter != m_images_list_map.end()) {
      iter->second->set_value(m_grid_columns.pixbuf, preview.second.pixbuf());
    }
  }
}


}

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  c-basic-offset:2
  indent-tabs-mode:nil
  tab-width:2
  fill-column:99
  End:
*/
