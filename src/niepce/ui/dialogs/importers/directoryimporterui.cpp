/*
 * niepce - ui/dialogs/importer/directoryimporterui.cpp
 *
 * Copyright (C) 2017 Hubert Figuière
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
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/label.h>

#include "fwk/toolkit/application.hpp"
#include "fwk/toolkit/configuration.hpp"
#include "engine/importer/directoryimporter.hpp"
#include "directoryimporterui.hpp"

namespace ui {

DirectoryImporterUI::DirectoryImporterUI()
  : m_importer(std::make_shared<eng::DirectoryImporter>())
  , m_name(_("Directory"))
{
}

std::shared_ptr<eng::IImporter> DirectoryImporterUI::get_importer()
{
  return m_importer;
}

const std::string& DirectoryImporterUI::name() const
{
  return m_name;
}

const std::string& DirectoryImporterUI::id() const
{
  return m_importer->id();
}

Gtk::Widget* DirectoryImporterUI::setup_widget(const fwk::Frame::Ptr& frame)
{
  m_frame = frame;
  Gtk::Button* select_directories = nullptr;
  m_builder = Gtk::Builder::create_from_file(GLADEDIR"directoryimporterui.ui",
                                             "main_widget");
  Gtk::Box* main_widget = nullptr;
  m_builder->get_widget("main_widget", main_widget);
  m_builder->get_widget("select_directories", select_directories);
  select_directories->signal_clicked().connect(
    sigc::mem_fun(*this, &DirectoryImporterUI::do_select_directories));
  m_builder->get_widget("directory_name", m_directory_name);
  return main_widget;
}

void DirectoryImporterUI::set_source_selected_callback(const SourceSelected& cb)
{
  m_source_selected_cb = cb;
}

void DirectoryImporterUI::do_select_directories()
{
  auto source = select_source();
  if (!source.empty() && m_source_selected_cb) {
    m_source_selected_cb(source);
  }
}

std::string DirectoryImporterUI::select_source()
{
  fwk::Configuration & cfg = fwk::Application::app()->config();

  Glib::ustring filename;
  {
    auto frame = m_frame.lock();
    Gtk::FileChooserDialog dialog(frame->gtkWindow(), _("Import picture folder"),
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
  m_directory_name->set_text(filename);
  return filename.raw();
}

}
