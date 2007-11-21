/*
 * niepce - db/library.h
 *
 * Copyright (C) 2007 Hubert Figuiere
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
#include <boost/any.hpp>
#include <boost/filesystem/path.hpp>

#include "iconnectiondriver.h"
#include "iconnectionmanagerdriver.h"
#include "libfolder.h"
#include "libfile.h"


namespace framework {
	class NotificationCenter;
}


namespace db {

	class Library
	{
	public:
		typedef boost::shared_ptr<Library> Ptr;

		typedef enum {
			NOTIFY_NONE = 0,
			NOTIFY_ADDED_FOLDERS,
			NOTIFY_ADDED_FILES,
			NOTIFY_FOLDER_CONTENT_QUERIED
		} NotifyType;

		Library(const std::string & dir, framework::NotificationCenter * nc);
		virtual ~Library();

		bool ok()
			{ return m_inited; }
		/** set the main library directory */
//		void setMainDir(const std::string & dir)
//			{ m_maindir = dir; }
		/** return the main directory */
		const std::string & mainDir() const
			{ return m_maindir; }
		const std::string & dbName() const
			{ return m_dbname; }

		void notify(NotifyType t, const boost::any & param);

		/** add a file to the library
		 * @param folder the path of the containing folder
		 * @param file the file path
		 * @param manage pass true it the library *manage* the file. Currently unsupported.
		 */
		int addFile2(const boost::filesystem::path & folder, 
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

		int checkDatabaseVersion();
		
		db::IConnectionDriver::Ptr dbDriver()
			{ return m_dbdrv; }
	private:
		bool init();
		bool _initDb();

		std::string                       m_maindir;
		std::string                       m_dbname;
		db::IConnectionManagerDriver::Ptr m_dbmgr;
		db::IConnectionDriver::Ptr        m_dbdrv;
		framework::NotificationCenter *   m_notif_center;
		bool                              m_inited;
	};

	
	struct LibNotification
	{
		Library::NotifyType type;
		boost::any          param;
	};

}

#endif
