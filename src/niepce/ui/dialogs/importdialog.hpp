/*
 * niepce - niepce/ui/dialogs/importdialog.h
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



#pragma once

#include <list>
#include <memory>
#include <string>

#include <glibmm/refptr.h>
#include <glibmm/dispatcher.h>

#include "engine/importer/importedfile.hpp"
#include "fwk/toolkit/gtkutils.hpp"
#include "fwk/toolkit/dialog.hpp"
#include "fwk/toolkit/uiresult.hpp"
#include "imageliststore.hpp"
#include "metadatapanecontroller.hpp"

namespace Gtk {
class Dialog;
class ComboBox;
class CheckButton;
class TreeView;
}

namespace fwk {
class ImageGridView;
class Thumbnail;
}

namespace eng {
class IImporter;
}

namespace ui {

class ImportDialog
	: public fwk::Dialog
{
public:
    typedef std::shared_ptr<ImportDialog> Ptr;

    class PreviewGridModel
        : public Gtk::TreeModel::ColumnRecord
    {
    public:
        Gtk::TreeModelColumn<Glib::ustring> filename;
        Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf>> pixbuf;
        Gtk::TreeModelColumn<eng::ImportedFile::Ptr> file;
        PreviewGridModel()
            { add(filename); add(pixbuf); add(file); }
    };

    ImportDialog();
    virtual ~ImportDialog();

    virtual void setup_widget() override;

//  const std::list<std::string> & to_import() const
//      { return m_list_to_import; }
    const Glib::ustring & sourcePath() const
        { return m_folder_path_source; }
    void setToImport(const Glib::ustring & f);
    const std::shared_ptr<eng::IImporter>& importer() const
        { return m_importer; }
private:
    class ImportParam;

    void doSelectDirectories();
    void append_files_to_import();
    void preview_received();

    std::shared_ptr<eng::IImporter> m_importer; // as shared_ptr<> for lambda capture
    Glib::ustring m_folder_path_source;

    Gtk::ComboBox *m_date_tz_combo;
    Gtk::CheckButton *m_ufraw_import_check;
    Gtk::CheckButton *m_rawstudio_import_check;
    Gtk::Label *m_directory_name;
    Gtk::Entry *m_destinationFolder;
    Gtk::ScrolledWindow *m_attributesScrolled;
    Gtk::ScrolledWindow *m_images_list_scrolled;
    PreviewGridModel m_grid_columns;
    Glib::RefPtr<Gtk::ListStore> m_images_list_model;
    std::map<std::string, Gtk::TreeIter> m_images_list_map;

    Gtk::IconView *m_gridview;

    MetaDataPaneController::Ptr m_metadata_pane;

    fwk::UIResultSingle<std::list<eng::ImportedFile::Ptr>> m_files_to_import;
    fwk::UIResults<std::pair<std::string, fwk::Thumbnail>> m_previews_to_import;
};

}

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  tab-width:4
  fill-column:80
  End:
*/

