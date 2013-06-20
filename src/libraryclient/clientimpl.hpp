/*
 * niepce - libraryclient/clientimpl.hpp
 *
 * Copyright (C) 2007-2013 Hubert Figuiere
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
#include "engine/library/clienttypes.hpp"
#include "engine/library/op.hpp"

namespace libraryclient {

class LocalLibraryServer;

class ClientImpl
{
public:
    static ClientImpl *makeClientImpl(const fwk::Moniker & moniker,
                                      const fwk::NotificationCenter::Ptr & nc);

    ClientImpl(const fwk::Moniker & moniker, const fwk::NotificationCenter::Ptr & nc);
    virtual ~ClientImpl();
    bool ok() const;

    eng::tid_t getAllKeywords();
    eng::tid_t queryKeywordContent(eng::library_id_t id);
    eng::tid_t getAllFolders();
    eng::tid_t queryFolderContent(eng::library_id_t id);
    eng::tid_t countFolder(eng::library_id_t id);
    eng::tid_t requestMetadata(eng::library_id_t id);
    eng::tid_t setMetadata(eng::library_id_t id, int meta,
                           const fwk::PropertyValue & value);
    eng::tid_t write_metadata(eng::library_id_t file_id);

    eng::tid_t moveFileToFolder(eng::library_id_t file_id,
                                eng::library_id_t from_folder_id,
                                eng::library_id_t to_folder_id);

    eng::tid_t getAllLabels();
    eng::tid_t createLabel(const std::string & s, const std::string & color);
    eng::tid_t deleteLabel(int id);
    eng::tid_t updateLabel(eng::library_id_t id, const std::string & new_name,
                           const std::string & new_color);

    eng::tid_t processXmpUpdateQueue(bool write_xmp);

    eng::tid_t importFromDirectory(const std::string & dir, bool manage);

protected:
    const fwk::Moniker m_moniker;
    LocalLibraryServer *m_localLibrary;
private:
    /** do the dirty work of scheduling the op */
    eng::tid_t schedule_op(const eng::Op::function_t & func);
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
