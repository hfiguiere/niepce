/*
 * niepce - db/library.cpp
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

#include <time.h>
#include <iostream>

#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

#include "niepce/notifications.h"
#include "library.h"
#include "utils/exception.h"
#include "utils/exempi.h"
#include "sqlite/sqlitecnxmgrdrv.h"
#include "sqlite/sqlitecnxdrv.h"
#include "sqlstatement.h"
#include "framework/notificationcenter.h"

using framework::NotificationCenter;

namespace bfs = boost::filesystem;

namespace db {

	const char * s_databaseName = "niepcelibrary.db";

	Library::Library(const std::string & dir, NotificationCenter * nc)
		: m_maindir(dir),
		  m_dbname(m_maindir / s_databaseName),
		  m_dbmgr(new db::sqlite::SqliteCnxMgrDrv()),
		  m_notif_center(nc),
		  m_inited(false)
	{
		DBG_OUT("dir = %s", dir.c_str());
		db::DBDesc desc("", 0, m_dbname.string());
		m_dbdrv = m_dbmgr->connect_to_db(desc, "", "");
		m_inited = init();
	}


	Library::~Library()
	{
	}


	void Library::notify(NotifyType t, const boost::any & param)
	{
		if(m_notif_center) {
			DBG_OUT("notif");
			// pass the notification
			framework::Notification::Ptr n(new framework::Notification(niepce::NOTIFICATION_LIB));
			LibNotification ln;
			ln.type = t;
			ln.param = param;
			n->setData(boost::any(ln));
			m_notif_center->post(n);
		}
		else {
			DBG_OUT("try to send a notification without notification center");
		}
	}

	/** init the database
	 * @return true is the DB is inited. false if it fail.
	 */
	bool Library::init()
	{
		int version = checkDatabaseVersion();
		if(version == -1) {
			// error
			DBG_OUT("version check -1");
		}
		else if(version == 0) {
			// let's create our DB
			DBG_OUT("version == 0");
			return _initDb();
		}
		return true;
	}

	bool Library::_initDb()
	{
		SQLStatement adminTable("CREATE TABLE admin (key TEXT NOT NULL,"
								" value TEXT)");
		SQLStatement adminVersion("INSERT INTO admin (key, value) "
								  " VALUES ('version', '1')");
		SQLStatement vaultTable("CREATE TABLE vaults (id INTEGER PRIMARY KEY,"
								" path TEXT)");
		SQLStatement folderTable("CREATE TABLE folders (id INTEGER PRIMARY KEY,"
								 " path TEXT, name TEXT, vault_id INTEGER, "
								 " parent_id INTEGER)");
		SQLStatement fileTable("CREATE TABLE files (id INTEGER PRIMARY KEY,"
							   " path TEXT, name TEXT, parent_id INTEGER,"
							   " orientation INTEGER, file_date INTEGER,"
							   " rating INTEGER, label INTEGER,"
							   " import_date INTEGER, mod_date INTEGER, "
							   " xmp TEXT)");
		SQLStatement keywordTable("CREATE TABLE keywords (id INTEGER PRIMARY KEY,"
								  " keyword TEXT, parent_id INTEGER)");
		SQLStatement keywordingTable("CREATE TABLE keywording (file_id INTEGER,"
									 " keyword_id INTEGER)");
//		SQLStatement collsTable("CREATE TABLE collections (id INTEGER PRIMARY KEY,"
//								" name TEXT)");
//		SQLStatement collectingTable("CREATE TABLE collecting (file_id INTEGER,"
//									 " collection_id INTEGER)");

		SQLStatement fileUpdateTrigger(
			"CREATE TRIGGER file_update_trigger UPDATE ON files "
			" BEGIN"
			"  UPDATE files SET mod_date = strftime('%s','now');"
			" END");

		m_dbdrv->execute_statement(adminTable);
		m_dbdrv->execute_statement(adminVersion);
		m_dbdrv->execute_statement(vaultTable);
		m_dbdrv->execute_statement(folderTable);
		m_dbdrv->execute_statement(fileTable);
		m_dbdrv->execute_statement(keywordTable);
		m_dbdrv->execute_statement(keywordingTable);
//		m_dbdrv->execute_statement(collsTable);
//		m_dbdrv->execute_statement(collectingTable);

		m_dbdrv->execute_statement(fileUpdateTrigger);
		
		return true;
	}

	/** check that database verion
	 * @return the DB version. -1 in case of error. 0 is can't read it.
	 */
	int Library::checkDatabaseVersion()
	{
		int v = 0;
		std::string version;
		try {
			SQLStatement sql("SELECT value FROM admin WHERE key='version'");
			
			if(m_dbdrv->execute_statement(sql)) {
				if(m_dbdrv->read_next_row() 
				   && m_dbdrv->get_column_content(0, version)) {
					v = boost::lexical_cast<int>(version);
				}
			}
		}
		catch(const utils::Exception & e)
		{
			DBG_OUT("db exception %s", e.what());
			v = -1;
		}
		catch(const boost::bad_lexical_cast &)
		{
			DBG_OUT("version is %s, can't convert to int", version.c_str());
			v = 0;
		}
		catch(...)
		{
			v = -1;
		}
		return v;
	}


	int Library::addFile(int folder_id, const bfs::path & file, bool manage)
	{
		int ret = -1;
		DBG_ASSERT(!manage, "manage not supported");
		DBG_ASSERT(folder_id != -1, "invalid folder ID");
		try {
			int32_t rating, label_id, orientation;
			std::string label;
			utils::XmpMeta meta(file);
			label_id = 0;
			orientation = meta.orientation();
			rating = meta.rating();
			label = meta.label();
			SQLStatement sql(boost::format("INSERT INTO files (path, name, "
										   " parent_id, import_date, mod_date, "
										   " orientation, file_date, rating, label, "
										   " xmp) "
										   " VALUES ('%1%', '%2%', '%3%', "
										   " '%4%', '%4%', '%5%', '0', '%6%', '%7%', ?1);") 
							 % file.string() % file.leaf() % folder_id
							 % time(NULL) % orientation % rating % folder_id);
			std::string buf = meta.serialize_inline();
			sql.bind(1, buf);
			if(m_dbdrv->execute_statement(sql)) {
				int64_t id = m_dbdrv->last_row_id();
				DBG_OUT("last row inserted %d", (int)id);
				ret = id;
			}
		}
		catch(const utils::Exception & e)
		{
			DBG_OUT("db exception %s", e.what());
		}
		catch(const std::exception & e)
		{
			DBG_OUT("unknown exception %s", e.what());
		}
		return ret;
	}


	int Library::addFileAndFolder(const bfs::path & folder, const bfs::path & file, 
								  bool manage)
	{
		LibFolder::Ptr f;
		f = getFolder(folder);
		if(f == NULL) {
			ERR_OUT("Folder %s not found", folder.string().c_str());
		}
		return addFile(f ? f->id() : -1, file, manage);
	}
	

	LibFolder::Ptr Library::getFolder(const bfs::path & folder)
	{
		LibFolder::Ptr f;
		SQLStatement sql(boost::format("SELECT id,name "
									   "FROM folders WHERE path='%1%'") 
						 % folder.string());
		
		try {
			if(m_dbdrv->execute_statement(sql)) {
				if(m_dbdrv->read_next_row()) {
					int32_t id;
					std::string name;
					m_dbdrv->get_column_content(0, id);
					m_dbdrv->get_column_content(1, name);
					f = LibFolder::Ptr(new LibFolder(id, name));
				}
			}
		}
		catch(utils::Exception & e)
		{
			DBG_OUT("db exception %s", e.what());
		}
		return f;
	}


	LibFolder::Ptr Library::addFolder(const bfs::path & folder)
	{
		LibFolder::Ptr f;
		SQLStatement sql(boost::format("INSERT INTO folders "
									   "(path,name,vault_id,parent_id) "
									   "VALUES('%1%', '%2%', '0', '0');") 
						 % folder.string() % folder.leaf());
		try {
			if(m_dbdrv->execute_statement(sql)) {
				int64_t id = m_dbdrv->last_row_id();
				DBG_OUT("last row inserted %d", (int)id);
				f = LibFolder::Ptr(new LibFolder((int)id, folder.leaf()));
			}
		}
		catch(utils::Exception & e)
		{
			DBG_OUT("db exception %s", e.what());
		}
		return f;
	}


	void Library::getAllFolders(const LibFolder::ListPtr & l)
	{
		SQLStatement sql("SELECT id,name FROM folders;");
		try {
			if(m_dbdrv->execute_statement(sql)) {
				while(m_dbdrv->read_next_row()) {
					int32_t id;
					std::string name;
					m_dbdrv->get_column_content(0, id);
					m_dbdrv->get_column_content(1, name);
					l->push_back(LibFolder::Ptr(new LibFolder(id, name)));
				}
			}
		}
		catch(utils::Exception & e)
		{
			DBG_OUT("db exception %s", e.what());
		}
	}

	void Library::getFolderContent(int folder_id, const LibFile::ListPtr & fl)
	{
		SQLStatement sql(boost::format("SELECT id,parent_id,path,name,"
									   "orientation,rating,label FROM files "
									   " WHERE parent_id='%1%'")
						 % folder_id);
		try {
			if(m_dbdrv->execute_statement(sql)) {
				while(m_dbdrv->read_next_row()) {
					int32_t id;
					int32_t fid;
					std::string pathname;
					std::string name;
					m_dbdrv->get_column_content(0, id);
					m_dbdrv->get_column_content(1, fid);
					m_dbdrv->get_column_content(2, pathname);
					m_dbdrv->get_column_content(3, name);
					DBG_OUT("found %s", pathname.c_str());
					LibFile::Ptr f(new LibFile(id, folder_id,
											   bfs::path(pathname), 
											   name));
					int32_t val;
					m_dbdrv->get_column_content(4, val);
					f->setOrientation(val);
					m_dbdrv->get_column_content(5, val);
					f->setRating(val);
					m_dbdrv->get_column_content(6, val);
					f->setLabel(val);
					fl->push_back(f);
					
				}
			}
		}
		catch(utils::Exception & e)
		{
			DBG_OUT("db exception %s", e.what());
		}
		
	}

}
