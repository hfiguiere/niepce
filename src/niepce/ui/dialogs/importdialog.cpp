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

#include <gtkmm/button.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/combobox.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/iconview.h>
#include <gtkmm/label.h>
#include <gtkmm/liststore.h>
#include <gtkmm/builder.h>
#include <gtkmm/stack.h>

#include "fwk/base/debug.hpp"
#include "fwk/utils/pathutils.hpp"
#include "fwk/toolkit/widgets/imagegridview.hpp"
#include "engine/importer/directoryimporter.hpp"
#include "engine/importer/importedfile.hpp"
#include "importdialog.hpp"
#include "importers/directoryimporterui.hpp"

namespace ui {

ImportDialog::ImportDialog()
  : fwk::Dialog(GLADEDIR"importdialog.ui", "importDialog")
  , m_current_importer(nullptr)
  , m_importer_ui_stack(nullptr)
  , m_date_tz_combo(nullptr)
  , m_ufraw_import_check(nullptr)
  , m_rawstudio_import_check(nullptr)
  , m_destination_folder(nullptr)
  , m_import_source_combo(nullptr)
  , m_attributes_scrolled(nullptr)
  , m_images_list_scrolled(nullptr)
{
}

ImportDialog::~ImportDialog()
{
}

void ImportDialog::add_importer_ui(IImporterUI& importer)
{
  m_import_source_combo->append(importer.id(), importer.name());
  Gtk::Widget* importer_widget = importer.setup_widget(
    std::static_pointer_cast<Frame>(shared_from_this()));
  m_importer_ui_stack->add(*importer_widget, importer.name());
  importer.set_source_selected_callback([this] (const std::string& source) {
      this->set_to_import(source);
    });
}

void ImportDialog::setup_widget()
{
    if(m_is_setup) {
        return;
    }

    Glib::RefPtr<Gtk::Builder> a_builder = builder();
    a_builder->get_widget("date_tz_combo", m_date_tz_combo);
    a_builder->get_widget("ufraw_import_check", m_ufraw_import_check);
    a_builder->get_widget("rawstudio_import_check", m_rawstudio_import_check);
    a_builder->get_widget("destinationFolder", m_destination_folder);

    // Sources
    a_builder->get_widget("importer_ui_stack", m_importer_ui_stack);
    a_builder->get_widget("import_source_combo", m_import_source_combo);
    m_import_source_combo->signal_changed().connect([] {});

    // Directory source, hardcoded.
    // XXX fix it
    m_importers[0] = std::make_shared<DirectoryImporterUI>();
    add_importer_ui(*m_importers[0]);

    // XXX restore from preferences.
    m_current_importer = m_importers[0];
    m_import_source_combo->set_active_id(m_current_importer->id());

    // Metadata pane.
    a_builder->get_widget("attributes_scrolled", m_attributes_scrolled);
    m_metadata_pane = MetaDataPaneController::Ptr(new MetaDataPaneController);
    auto w = m_metadata_pane->buildWidget();
    add(m_metadata_pane);
    m_attributes_scrolled->add(*w);
    w->show_all();

    // Gridview of previews.
    a_builder->get_widget("images_list_scrolled", m_images_list_scrolled);
    m_images_list_model = Gtk::ListStore::create(m_grid_columns);
    m_gridview = Gtk::manage(
        new Gtk::IconView(
            Glib::RefPtr<Gtk::TreeModel>::cast_dynamic(m_images_list_model)));
    m_gridview->set_pixbuf_column(m_grid_columns.pixbuf);
    m_gridview->set_text_column(m_grid_columns.filename);
    m_gridview->set_item_width(100);
    m_gridview->show();
    m_images_list_scrolled->add(*m_gridview);
    m_images_list_scrolled->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

    m_previews_to_import.connect(
      sigc::mem_fun(this, &ImportDialog::preview_received));
    m_files_to_import.connect(
      sigc::mem_fun(this, &ImportDialog::append_files_to_import));

    m_is_setup = true;
}

void ImportDialog::set_to_import(const std::string& f)
{
    m_images_list_model->clear();
    m_images_list_map.clear();
    m_files_to_import.clear();

    auto importer = get_importer();
    m_files_to_import.run(
      [this, f, importer] () {
        return importer->list_source_content(
          f,
          [this] (std::list<eng::ImportedFile::Ptr>&& list_to_import) {
            this->m_files_to_import.send_data(std::move(list_to_import));
          });
      });

    m_folder_path_source = f;
    m_destination_folder->set_text(fwk::path_basename(f));
}

void ImportDialog::append_files_to_import()
{
  auto files_to_import = m_files_to_import.recv_data();

  if (!m_images_list_model) {
    ERR_OUT("No image list model");
    return;
  }
  // request the previews to the importer.
  std::list<std::string> paths;
  for(const auto & f : files_to_import) {
    DBG_OUT("selected %s", f->name().c_str());
    paths.push_back(f->name());
    Gtk::TreeIter iter = m_images_list_model->append();
    m_images_list_map.insert(std::make_pair(f->name(), iter));
    iter->set_value(m_grid_columns.filename, Glib::ustring(f->name()));
    iter->set_value(m_grid_columns.file, std::move(f));
  }

  auto importer = get_importer();
  auto source = m_folder_path_source.raw();
  m_previews_to_import.run(
    [this, importer, source, paths] () {
      return importer->get_previews_for(
        source, paths,
        [this] (const std::string& path, const fwk::Thumbnail& thumbnail) {
          this->m_previews_to_import.send_data(
            std::make_pair(path, thumbnail));
        });
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
