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
    delete m_importer;
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
    m_imagesListModel = Gtk::ListStore::create(m_grid_columns);
    m_gridview = Gtk::manage(
        new Gtk::IconView(
            Glib::RefPtr<Gtk::TreeModel>::cast_dynamic(m_imagesListModel)));
    m_gridview->set_pixbuf_column(m_grid_columns.pixbuf);
    m_gridview->set_text_column(m_grid_columns.filename);
    m_gridview->show();
    m_images_list_scrolled->add(*m_gridview);
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
    if (!m_importer) {
        // FIXME this should be the right kind
        m_importer = new eng::DirectoryImporter;
    }
    auto target_content = std::async(std::launch::async,
                                     [f, this] () {
                                         return m_importer->listTargetContent(
                                             f);
                                     });

    m_folder_path_to_import = f;
    m_destinationFolder->set_text(fwk::path_basename(f));
    m_directory_name->set_text(f);

    m_imagesListModel->clear();

    if(target_content.get()) {
        auto list_to_import = m_importer->getTargetContent();

        for(const auto & f : list_to_import) {
            DBG_OUT("selected %s", f->name().c_str());
            Gtk::TreeIter iter = m_imagesListModel->append();
            iter->set_value(m_grid_columns.filename, Glib::ustring(f->name()));
            iter->set_value(m_grid_columns.file, std::move(f));
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
