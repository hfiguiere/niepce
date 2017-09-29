/*
 * niepce - engine/library/commands.hpp
 *
 * Copyright (C) 2007-2015 Hubert Figuière
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

#include "fwk/utils/files.hpp"

extern "C" {
    bool cmd_list_all_keywords(eng::Library* lib);
    bool cmd_list_all_folders(eng::Library* lib);
    bool cmd_import_file(eng::Library* lib, const char* path, eng::LibraryManaged);
    bool cmd_import_files(eng::Library* lib, const char* folder, fwk::FileList* files,
                          eng::LibraryManaged);
    bool cmd_request_metadata(eng::Library* lib, eng::library_id_t id);
    bool cmd_query_folder_content(eng::Library* lib, eng::library_id_t folder_id);
    bool cmd_count_folder(eng::Library* lib, eng::library_id_t folder_id);
    bool cmd_query_keyword_content(eng::Library* lib, eng::library_id_t id);
    bool cmd_write_metadata(eng::Library* lib, eng::library_id_t id);
    bool cmd_move_file_to_folder(eng::Library* lib, eng::library_id_t id,
                                 eng::library_id_t from, eng::library_id_t to);
    bool cmd_set_metadata(eng::Library* lib, eng::library_id_t id, fwk::PropertyIndex meta,
                          const fwk::PropertyValue* value);
    bool cmd_list_all_labels(eng::Library* lib);
    bool cmd_create_label(eng::Library* lib, const char* name, const char* colour);
    bool cmd_delete_label(eng::Library* lib, eng::library_id_t id);
    bool cmd_update_label(eng::Library* lib, eng::library_id_t id, const char* name,
                          const char* colour);
    bool cmd_process_xmp_update_queue(eng::Library* lib, bool write_xmp);
}

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
