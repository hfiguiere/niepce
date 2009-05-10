/*
 * niepce - engine/db/library.h
 *
 * Copyright (C) 2007-2009 Hubert Figuiere
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

#include <tr1/memory>
#include <boost/any.hpp>

#include "fwk/toolkit/notificationcenter.hpp"
#include "fwk/utils/db/iconnectiondriver.hpp"
#include "fwk/utils/db/iconnectionmanagerdriver.hpp"
#include "engine/db/libfolder.hpp"
#include "engine/db/libfile.hpp"
#include "engine/db/libmetadata.hpp"
#include "engine/db/filebundle.hpp"
#include "engine/db/keyword.hpp"
#include "engine/db/label.hpp"

// The database schema version. Increase at each change.
// Some will be persistent and have a conversion TBD.
#define DB_SCHEMA_VERSION 3

namespace fwk {
class RgbColor;
}

namespace eng {

class Library
{
public:
		typedef std::tr1::shared_ptr<Library> Ptr;

		typedef enum {
        NOTIFY_NONE = 0,
        NOTIFY_NEW_LIBRARY_CREATED,
        NOTIFY_ADDED_FOLDERS,
        NOTIFY_ADDED_FILES,
        NOTIFY_ADDED_KEYWORDS,
        NOTIFY_ADDED_KEYWORD,
        NOTIFY_ADDED_LABELS,
        NOTIFY_FOLDER_CONTENT_QUERIED,
        NOTIFY_KEYWORD_CONTENT_QUERIED,
        NOTIFY_METADATA_QUERIED,
        NOTIFY_METADATA_CHANGED,
        NOTIFY_LABEL_CHANGED,
        NOTIFY_LABEL_DELETED,
        NOTIFY_XMP_NEEDS_UPDATE,
        NOTIFY_FOLDER_COUNTED
		} NotifyType;

		Library(const std::string & dir, const fwk::NotificationCenter::Ptr & nc);
		virtual ~Library();

		bool ok()
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

		void notify(NotifyType t, const boost::any & param);

		/** add a file to the library
		 * @param folder the path of the containing folder
		 * @param file the file path
		 * @param manage pass true it the library *manage* the file. Currently unsupported.
		 */
		int addFileAndFolder(const std::string & folder, 
                         const std::string & file, bool manage);

    /** add a fs file to the library  
     * @param file the file path
     * @return the id of the fs_file, -1 in case of error
     */
    int addFsFile(const std::string & file);

    /** Get a FsFile from an id
     * @param id the id of the FsFile
     * @return the path. Empty if not found.
     */
    std::string getFsFile(int id);

		/** add a file to the library
		 * @param folder_id the id of the containing folder
		 * @param file the file path
		 * @param manage pass true it the library *manage* the file. Currently unsupported.
		 */
		int addFile(int folder_id, const std::string & file, bool manage);

		/** add a bundle of files to the library
		 * @param folder_id the id of the containing folder
		 * @param bundle the bundle
		 * @param manage pass true it the library *manage* the file. Currently unsupported.
		 */
    int addBundle(int folder_id, const eng::FileBundle::Ptr & bundle, 
                  bool manage);
    /** add a sidecar fsfile to a bundle (file)
     * @param file_id the id of the file bundle
     * @param fsfile_id the id of the fsfile
     * @return true if success
     */
    bool addSidecarFileToBundle(int file_id, int fsfile_id);
    /** add a jpeg fsfile to a bundle (file)
     * @param file_id the id of the file bundle
     * @param fsfile_id the id of the fsfile
     * @return true if success
     */
    bool addJpegFileToBundle(int file_id, int fsfile_id);
		
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
		void getFolderContent(int folder_id, const LibFile::ListPtr & fl);
		int countFolder(int folder_id);
		void getAllKeywords(const Keyword::ListPtr & l);
		void getKeywordContent(int keyword_id, const LibFile::ListPtr & fl);
    /** get the metadata block (XMP) */
		void getMetaData(int file_id, const LibMetadata::Ptr & );
    /** set the metadata block (XMP) */
    bool setMetaData(int file_id, const LibMetadata::Ptr & );
    bool setMetaData(int file_id, int meta, const boost::any & value);

		void getAllLabels(const eng::Label::ListPtr & l);
    int addLabel(const std::string & name, const std::string & color);
    int addLabel(const std::string & name, const fwk::RgbColor & c);
    bool updateLabel(int label_id, const std::string & name, const std::string & color);
    bool deleteLabel(int label_id);

    /** Trigger the processing of the XMP update queue */
    bool processXmpUpdateQueue();

		/** Locate the keyword, creating it if needed
		 * @param keyword the keyword to locate
		 * @return -1 if not found (shouldn't happen) or the id of the
		 * keyword, either found or just created.
		 */
		int makeKeyword(const std::string & keyword);
		/** Assign a keyword to a file.
		 * @param kw_id the keyword id
		 * @param file_id the file id
		 * @return true if success, false if error
		 */
		bool assignKeyword(int kw_id, int file_id);

		int checkDatabaseVersion();
		
		db::IConnectionDriver::Ptr dbDriver()
        { return m_dbdrv; }
private:
		bool init();
		bool _initDb();

    /** external sqlite fucntion to trigger the rewrite of the XMP */
    void triggerRewriteXmp(void);
    bool getXmpIdsInQueue(std::vector<int> & ids);
    /** rewrite the XMP sidecar for the file whose id is %id
     * and remove it from the queue.
     */
    bool rewriteXmpForId(int id);

    /** set an "internal" metadata of type int */
    bool setInternalMetaDataInt(int file_id, const char* col,
                                int32_t value);

    std::string                       m_maindir;
    std::string                       m_dbname;
		db::IConnectionManagerDriver::Ptr m_dbmgr;
		db::IConnectionDriver::Ptr        m_dbdrv;
		std::tr1::weak_ptr<fwk::NotificationCenter>  m_notif_center;
		bool                              m_inited;
};

	
struct LibNotification
{
		Library::NotifyType type;
		boost::any          param;
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

