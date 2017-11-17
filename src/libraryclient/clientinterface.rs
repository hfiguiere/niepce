/*
 * niepce - libraryclient/clientinterface.rs
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

use fwk::base::PropertyValue;
use engine::db::LibraryId;
use engine::db::library::Managed;
use root::eng::NiepceProperties as Np;

/// Client interface.
pub trait ClientInterface {

    /// get all the keywords
    fn get_all_keywords(&mut self);
    fn query_keyword_content(&mut self, id: LibraryId);
    fn count_keyword(&mut self, id: LibraryId);

    /// get all the folder
    fn get_all_folders(&mut self);
    fn query_folder_content(&mut self, id: LibraryId);
    fn count_folder(&mut self, id: LibraryId);
    fn create_folder(&mut self, name: String, path: Option<String>);
    fn delete_folder(&mut self, id: LibraryId);

    fn request_metadata(&mut self, id: LibraryId);
    /// set the metadata
    fn set_metadata(&mut self, id: LibraryId, meta: Np, value: &PropertyValue);
    fn write_metadata(&mut self, id: LibraryId);

    fn move_file_to_folder(&mut self, file_id: LibraryId, from: LibraryId,
                               to: LibraryId);
    /// get all the labels
    fn get_all_labels(&mut self);
    fn create_label(&mut self, label: String, colour: String);
    fn delete_label(&mut self, id: LibraryId);
    /// update a label
    fn update_label(&mut self, id: LibraryId, new_name: String, new_colour: String);

    /// tell to process the Xmp update Queue
    fn process_xmp_update_queue(&mut self, write_xmp: bool);

    /// Import file
    /// @param path the file path
    /// @param manage true if imported file have to be managed
    fn import_file(&mut self, path: String, manage: Managed);
    /// Import files from a directory
    /// @param dir the directory
    /// @param manage true if imports have to be managed
    fn import_from_directory(&mut self, dir: String, files: Vec<String>, manage: Managed);
}

/// Sync client interface
pub trait ClientInterfaceSync {

    /// Create a keyword. Return the id for the keyword.
    /// If the keyword already exists, return its `LibraryId`.
    fn create_keyword_sync(&mut self, keyword: String) -> LibraryId;

    /// Create a label. Return the id of the newly created label.
    fn create_label_sync(&mut self, name: String, colour: String) -> LibraryId;

    /// Create a folder. Return the id of the newly created folder.
    fn create_folder_sync(&mut self, name: String, path: Option<String>) -> LibraryId;
}
