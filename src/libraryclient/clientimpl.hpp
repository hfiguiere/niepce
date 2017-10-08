/*
 * niepce - libraryclient/clientimpl.hpp
 *
 * Copyright (C) 2007-2017 Hubert Figuière
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


#ifndef _LIBRARYCLIENT_CLIENTIMPL_H_
#define _LIBRARYCLIENT_CLIENTIMPL_H_

#include <string>

#include "fwk/base/moniker.hpp"

#include "rust_bindings.hpp"

namespace libraryclient {

class ClientImpl
{
public:
    static std::unique_ptr<ClientImpl> makeClientImpl(const fwk::Moniker & moniker,
                                                      uint64_t notif_id);

    ClientImpl(const fwk::Moniker & moniker, uint64_t notif_id);
    virtual ~ClientImpl();
    bool ok() const;

    void getAllKeywords();
    void queryKeywordContent(eng::library_id_t id);
    void getAllFolders();
    void queryFolderContent(eng::library_id_t id);
    void countFolder(eng::library_id_t id);
    void requestMetadata(eng::library_id_t id);
    void setMetadata(eng::library_id_t id, eng::Np meta,
                     const fwk::PropertyValuePtr & value);
    void write_metadata(eng::library_id_t file_id);

    void moveFileToFolder(eng::library_id_t file_id,
                          eng::library_id_t from_folder_id,
                          eng::library_id_t to_folder_id);

    void getAllLabels();
    void createLabel(const std::string & s, const std::string & color);
    void deleteLabel(int id);
    void updateLabel(eng::library_id_t id, const std::string & new_name,
                     const std::string & new_color);

    void processXmpUpdateQueue(bool write_xmp);

    void importFile(const std::string & path, eng::Managed manage);
    void importFromDirectory(const std::string & dir, eng::Managed manage);

protected:
    const fwk::Moniker m_moniker;
    ffi::ClientImpl* m_impl;
};

}


#endif
/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
