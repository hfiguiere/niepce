/*
 * niepce - libraryclient/libraryclient.hpp
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

#ifndef _LIBRARYCLIENT_H_
#define _LIBRARYCLIENT_H_

#include <string>
#include <memory>

#include "fwk/base/propertybag.hpp"
#include "fwk/base/util.hpp"
#include "engine/library/clienttypes.hpp"
#include "engine/library/thumbnailcache.hpp"
#include "engine/db/library.hpp"
#include "engine/db/librarytypes.hpp"

namespace fwk {
class Moniker;
class NotificationCenter;
}

namespace libraryclient {

class UIDataProvider;

class ClientImpl;

class LibraryClient
{
public:
    NON_COPYABLE(LibraryClient);
    LibraryClient(const fwk::Moniker & moniker, uint64_t notif_id);
    virtual ~LibraryClient();
    // @return false in case of error.
    bool ok() const;

    static eng::tid_t newTid();
    /** get all the keywords
     * @return transaction ID
     */
    eng::tid_t getAllKeywords();
    /** get all the folder
     * @return transaction ID
     */
    eng::tid_t getAllFolders();

    eng::tid_t queryFolderContent(eng::library_id_t id);
    eng::tid_t queryKeywordContent(eng::library_id_t id);
    eng::tid_t countFolder(eng::library_id_t id);
    eng::tid_t requestMetadata(eng::library_id_t id);

    /** set the metadata */
    eng::tid_t setMetadata(eng::library_id_t id, fwk::PropertyIndex meta,
                           const fwk::PropertyValuePtr & value);
    eng::tid_t moveFileToFolder(eng::library_id_t file_id, eng::library_id_t from_folder,
                                eng::library_id_t to_folder);
    eng::tid_t write_metadata(eng::library_id_t file_id);

    /** get all the labels */
    eng::tid_t getAllLabels();
    eng::tid_t createLabel(const std::string & s, const std::string & color);
    eng::tid_t deleteLabel(int id);
    /** update a label */
    eng::tid_t updateLabel(eng::library_id_t id, const std::string & new_name,
                           const std::string & new_color);

    /** tell to process the Xmp update Queue */
    eng::tid_t processXmpUpdateQueue(bool write_xmp);

    /** Import file
     * @param path the file path
     * @param manage true if imported file have to be managed
     */
    void importFile(const std::string & path, eng::Managed manage);

    /** Import files from a directory
     * @param dir the directory
     * @param manage true if imports have to be managed
     */
    void importFromDirectory(const std::string & dir, eng::Managed manage);

    eng::ThumbnailCache & thumbnailCache()
        { return m_thumbnailCache; }

    const std::unique_ptr<UIDataProvider>& getDataProvider() const
        { return m_uidataprovider; }

    // state
    eng::library_id_t trash_id() const
        {
	    return m_trash_id;
        }
    void set_trash_id(eng::library_id_t id)
        {
	    m_trash_id = id;
        }
private:
    std::unique_ptr<ClientImpl> m_pImpl;

    eng::ThumbnailCache m_thumbnailCache;
    std::unique_ptr<UIDataProvider> m_uidataprovider;
    eng::library_id_t m_trash_id;
};

typedef std::shared_ptr<LibraryClient> LibraryClientPtr;

}

#endif
/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  c-basic-offset:4
  tab-width:4
  indent-tabs-mode:nil
  fill-column:80
  End:
*/
