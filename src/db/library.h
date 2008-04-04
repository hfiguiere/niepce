/*
 * niepce - db/library.h
 *
 * Copyright (C) 2007-2008 Hubert Figuiere
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

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/any.hpp>
#include <boost/filesystem/path.hpp>

#include "framework/notificationcenter.h"
#include "db/iconnectiondriver.h"
#include "db/iconnectionmanagerdriver.h"
#include "db/libfolder.h"
#include "db/libfile.h"
#include "db/keyword.h"


namespace db {

	class Library
	{
	public:
		typedef boost::shared_ptr<Library> Ptr;

		typedef enum {
			NOTIFY_NONE = 0,
			NOTIFY_ADDED_FOLDERS,
			NOTIFY_ADDED_FILES,
			NOTIFY_ADDED_KEYWORDS,
			NOTIFY_ADDED_KEYWORD,
			NOTIFY_FOLDER_CONTENT_QUERIED,
			NOTIFY_KEYWORD_CONTENT_QUERIED,
			NOTIFY_METADATA_QUERIED,
			NOTIFY_FOLDER_COUNTED
		} NotifyType;

		Library(const std::string & dir, const framework::NotificationCenter::Ptr & nc);
		virtual ~Library();

		bool ok()
			{ return m_inited; }
		/** set the main library directory */
//		void setMainDir(const std::string & dir)
//			{ m_maindir = dir; }
		/** return the main directory */
		const boost::filesystem::path & mainDir() const
			{ return m_maindir; }
		/** get the path to the DB file */
		const boost::filesystem::path & dbName() const
			{ return m_dbname; }

		void notify(NotifyType t, const boost::any & param);

		/** add a file to the library
		 * @param folder the path of the containing folder
		 * @param file the file path
		 * @param manage pass true it the library *manage* the file. Currently unsupported.
		 */
		int addFileAndFolder(const boost::filesystem::path & folder, 
							 const boost::filesystem::path & file, bool manage);
		/** add a file to the library
		 * @param folder_id the id of the containing folder
		 * @param file the file path
		 * @param manage pass true it the library *manage* the file. Currently unsupported.
		 */
		int addFile(int folder_id, const boost::filesystem::path & file, 
					bool manage);
		
		/** Get a specific folder id from the library
		 * @param folder the folder path to check
		 * @return the folder, NULL if not found
		 */
		LibFolder::Ptr getFolder(const boost::filesystem::path & folder);
		/** Add a folder
		 * @param folder the folder path
		 */
		LibFolder::Ptr addFolder(const boost::filesystem::path & folder);
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

		boost::filesystem::path           m_maindir;
		boost::filesystem::path           m_dbname;
		db::IConnectionManagerDriver::Ptr m_dbmgr;
		db::IConnectionDriver::Ptr        m_dbdrv;
		boost::weak_ptr<framework::NotificationCenter>  m_notif_center;
		bool                              m_inited;
	};

	
	struct LibNotification
	{
		Library::NotifyType type;
		boost::any          param;
	};

}

#endif
