/*
 * niepce - engine/db/library.h
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



#ifndef _DB_LIBRARY_H_
#define _DB_LIBRARY_H_

#include <string>

#include <memory>
#include <boost/any.hpp>

#include "fwk/toolkit/notificationcenter.hpp"
#include "fwk/utils/db/iconnectiondriver.hpp"
#include "fwk/utils/db/iconnectionmanagerdriver.hpp"
#include "engine/db/librarytypes.hpp"
#include "engine/db/libfolder.hpp"
#include "engine/db/libfile.hpp"
#include "engine/db/libmetadata.hpp"
#include "engine/db/filebundle.hpp"
#include "engine/db/keyword.hpp"
#include "engine/db/label.hpp"
#include "engine/library/notification.hpp"

// The database schema version. Increase at each change.
// Some will be persistent and have a conversion TBD.
#define DB_SCHEMA_VERSION 6

namespace fwk {
class RgbColour;
}

namespace eng {

class Library
{
public:
    typedef std::shared_ptr<Library> Ptr;

    /** Whether to import managed. */
    enum class Managed {
        NO = 0,
        YES
    };

    Library(const std::string & dir, const fwk::NotificationCenter::Ptr & nc);
    virtual ~Library();

    bool ok() const
        { return m_inited; }
    /** set the main library directory */
//		void setMainDir(const std::string & dir)
//			{ m_maindir = dir; }
    /** return the main directory */
    const std::string & mainDir() const
        { return m_maindir; }
    /** get the path to the DB file */
    const std::string & dbName() const
        { return m_dbname; }

    void notify(LibNotification&& n);

    /** add a file to the library
     * @param folder the path of the containing folder
     * @param file the file path
     * @param manage pass Managed::YES if the library *manage* the file. Currently unsupported.
     */
    library_id_t addFileAndFolder(const std::string & folder,
                                  const std::string & file, Managed manage);

    /** add a fs file to the library
     * @param file the file path
     * @return the id of the fs_file, -1 in case of error
     */
    library_id_t addFsFile(const std::string & file);

    /** Get a FsFile from an id
     * @param id the id of the FsFile
     * @return the path. Empty if not found.
     */
    std::string getFsFile(library_id_t id);

    /** add a file to the library
     * @param folder_id the id of the containing folder
     * @param file the file path
     * @param manage pass Managed::YES if the library *manage* the file. Currently unsupported.
     */
    library_id_t addFile(library_id_t folder_id, const std::string & file, Managed manage);

    /** add a bundle of files to the library
     * @param folder_id the id of the containing folder
     * @param bundle the bundle
     * @param manage pass Managed::YES if the library *manage* the file. Currently unsupported.
     */
    library_id_t addBundle(library_id_t folder_id,
                           const eng::FileBundle::Ptr & bundle,
                           Managed manage);
    /** add a sidecar fsfile to a bundle (file)
     * @param file_id the id of the file bundle
     * @param fsfile_id the id of the fsfile
     * @return true if success
     */
    bool addSidecarFileToBundle(library_id_t file_id, library_id_t fsfile_id);
    /** add a jpeg fsfile to a bundle (file)
     * @param file_id the id of the file bundle
     * @param fsfile_id the id of the fsfile
     * @return true if success
     */
    bool addJpegFileToBundle(library_id_t file_id, library_id_t fsfile_id);

    /** Get a specific folder id from the library
     * @param folder the folder path to check
     * @return the folder, NULL if not found
     */
    LibFolder::Ptr getFolder(const std::string & folder);

    /** Add a folder
     * @param folder the folder path
     */
    LibFolder::Ptr addFolder(const std::string & folder);
    /** List all the folders.
     * @param l the list of LibFolder
     */
    void getAllFolders(const LibFolder::ListPtr & l);

    /** List the folder content
     * @param folder_id id of the folder
     * @param fl the resulting file list
     */
    void getFolderContent(library_id_t folder_id, const LibFile::ListPtr & fl);
    int countFolder(library_id_t folder_id);
    void getAllKeywords(const KeywordListPtr & l);
    void getKeywordContent(library_id_t keyword_id, const LibFile::ListPtr & fl);
    /** get the metadata block (XMP) */
    void getMetaData(library_id_t file_id, const LibMetadata::Ptr & );
    /** set the metadata block (XMP) */
    bool setMetaData(library_id_t file_id, const LibMetadata::Ptr & );
    bool setMetaData(library_id_t file_id, fwk::PropertyIndex meta,
                     const fwk::PropertyValue & value);
    bool writeMetaData(library_id_t file_id);

    bool moveFileToFolder(library_id_t file_id, library_id_t folder_id);

    void getAllLabels(const eng::Label::ListPtr & l);
    library_id_t addLabel(const std::string & name, const std::string & colour);
    library_id_t addLabel(const std::string & name, const fwk::RgbColour & c);
    bool updateLabel(library_id_t label_id, const std::string & name, const std::string & colour);
    bool deleteLabel(library_id_t label_id);

    /** Trigger the processing of the XMP update queue */
    bool processXmpUpdateQueue(bool rewrite_xmp);

    /** Locate the keyword, creating it if needed
     * @param keyword the keyword to locate
     * @return -1 if not found (shouldn't happen) or the id of the
     * keyword, either found or just created.
     */
    library_id_t makeKeyword(const std::string & keyword);
    /** Assign a keyword to a file.
     * @param kw_id the keyword id
     * @param file_id the file id
     * @return true if success, false if error
     */
    bool assignKeyword(library_id_t kw_id, library_id_t file_id);
    /** Unassign all keyword for a file.
     * @param file_id the file id
     * @return true if success, false if error
     */
    bool unassignAllKeywordsForFile(library_id_t file_id);

    int checkDatabaseVersion();

    db::IConnectionDriver::Ptr dbDriver()
        { return m_dbdrv; }
private:
    bool init();
    bool _initDb();

    bool getXmpIdsInQueue(std::vector<library_id_t> & ids);
    /** rewrite the XMP sidecar for the file whose id is %id
     * and remove it from the queue.
     */
    bool rewriteXmpForId(library_id_t id, bool rewrite_xmp);

    /** set an "internal" metadata of type int */
    bool setInternalMetaDataInt(library_id_t file_id, const char* col,
                                int32_t value);

    std::string                       m_maindir;
    std::string                       m_dbname;
    db::IConnectionManagerDriver::Ptr m_dbmgr;
    db::IConnectionDriver::Ptr        m_dbdrv;
    std::weak_ptr<fwk::NotificationCenter>  m_notif_center;
    bool                              m_inited;
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

