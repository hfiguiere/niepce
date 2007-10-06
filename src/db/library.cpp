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

#include <iostream>

#include <boost/lexical_cast.hpp>

#include "library.h"
#include "utils/exception.h"
#include "sqlite/sqlitecnxmgrdrv.h"
#include "sqlite/sqlitecnxdrv.h"
#include "sqlstatement.h"

namespace db {

	const char * s_databaseName = "niepcelibrary.db";

	Library::Library(const std::string & dir)
		: m_maindir(dir),
			m_dbname(m_maindir + "/" + s_databaseName),
			m_dbmgr(new db::sqlite::SqliteCnxMgrDrv()),
			m_inited(false)
	{
		DBG_OUT("dir = %s", dir.c_str());
		db::DBDesc desc("", 0, m_dbname);
		m_dbdrv = m_dbmgr->connect_to_db(desc, "", "");
		m_inited = init();
	}

	Library::~Library()
	{
	}

	/** init the database
	 * @return true is the DB is inited. false if it fail.
	 */
	bool Library::init()
	{
		int version = checkDatabaseVersion();
		if(version == -1) {
			// error
		}
		else if(version == 0) {
			// let's create our DB
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
		m_dbdrv->execute_statement(adminTable);
		m_dbdrv->execute_statement(adminVersion);

		SQLStatement vaultTable("CREATE TABLE vaults (id INTEGER PRIMARY KEY,"
														" path TEXT)");
		m_dbdrv->execute_statement(vaultTable);

		SQLStatement folderTable("CREATE TABLE folders (id INTEGER PRIMARY KEY,"
														 " path TEXT, name TEXT, vault_id INTEGER, "
														 " parent_id INTEGER)");
		m_dbdrv->execute_statement(folderTable);

		SQLStatement fileTable("CREATE TABLE files (id INTEGER PRIMARY KEY,"
													 " path TEXT, parent_id INTEGER)");
		m_dbdrv->execute_statement(fileTable);

		SQLStatement keywordTable("CREATE TABLE keywords (id INTEGER PRIMARY KEY,"
															" keyword TEXT, parent_id INTEGER)");
		m_dbdrv->execute_statement(keywordTable);

		SQLStatement keywordingTable("CREATE TABLE keywording (file_id INTEGER,"
																 " keyword_id INTEGER)");
		m_dbdrv->execute_statement(keywordingTable);

		SQLStatement collsTable("CREATE TABLE collections (id INTEGER PRIMARY KEY,"
														" name TEXT)");
		m_dbdrv->execute_statement(collsTable);
								
		SQLStatement collectingTable("CREATE TABLE collecting (file_id INTEGER,"
																 " collection_id INTEGER)");
		m_dbdrv->execute_statement(collectingTable);

		return true;
	}

	/** check that database verion
	 * @return the DB version. -1 in case of error. 0 is can't read it.
	 */
	int Library::checkDatabaseVersion()
	{
		int v;
		try {
			SQLStatement sql("SELECT value FROM admin WHERE key='version'");
			
			m_dbdrv->execute_statement(sql);
			m_dbdrv->read_next_row();
			std::string version;
			m_dbdrv->get_column_content(0, version);
			v = boost::lexical_cast<int>(version);
		}
		catch(utils::Exception & e)
		{
			std::cerr << "db exception" << std::endl;
			v = -1;
		}
		catch(boost::bad_lexical_cast & e)
		{
			v = 0;
		}
		catch(...)
		{
			v = -1;
		}
		return v;
	}
	
}
