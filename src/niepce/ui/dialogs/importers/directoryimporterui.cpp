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

#include "fwk/toolkit/application.hpp"
#include "fwk/toolkit/configuration.hpp"
#include "engine/importer/directoryimporter.hpp"
#include "directoryimporterui.hpp"

namespace ui {

  DirectoryImporterUI::DirectoryImporterUI()
    : m_importer(std::make_shared<eng::DirectoryImporter>())
  {
  }

  std::shared_ptr<eng::IImporter> DirectoryImporterUI::get_importer()
  {
    return m_importer;
  }

  std::string DirectoryImporterUI::select_source(const fwk::Frame& frame)
  {
    fwk::Configuration & cfg = fwk::Application::app()->config();

    Glib::ustring filename;
    {
      Gtk::FileChooserDialog dialog(frame.gtkWindow(), _("Import picture folder"),
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
    return filename.raw();
  }
}
