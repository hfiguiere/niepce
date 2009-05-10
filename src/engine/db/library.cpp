/*
 * niepce - engine/db/library.cpp
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

#include <time.h>
#include <stdio.h>
#include <iostream>

#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <boost/bind.hpp>

#include "fwk/base/color.hpp"
#include "niepce/notifications.hpp"
#include "library.hpp"
#include "metadata.hpp"
#include "fwk/base/debug.hpp"
#include "fwk/utils/exception.hpp"
#include "fwk/utils/exempi.hpp"
#include "fwk/utils/pathutils.hpp"
#include "fwk/utils/db/sqlite/sqlitecnxmgrdrv.hpp"
#include "fwk/utils/db/sqlite/sqlitecnxdrv.hpp"
#include "fwk/utils/db/sqlstatement.hpp"
#include "fwk/toolkit/notificationcenter.hpp"
#include "fwk/toolkit/mimetype.hpp"

using ::fwk::NotificationCenter;
using ::db::SQLStatement;

namespace eng {

const char * s_databaseName = "niepcelibrary.db";


Library::Library(const std::string & dir, const NotificationCenter::Ptr & nc)
    : m_maindir(dir),
      m_dbname(m_maindir + "/" + s_databaseName),
      m_dbmgr(new db::sqlite::SqliteCnxMgrDrv()),
      m_notif_center(nc),
      m_inited(false)
{
    DBG_OUT("dir = %s", dir.c_str());
    db::DBDesc desc("", 0, m_dbname);
    m_dbdrv = m_dbmgr->connect_to_db(desc, "", "");
    m_inited = init();

    m_dbdrv->create_function0("rewrite_xmp", 
                              boost::bind(&Library::triggerRewriteXmp,
                                          this));
}

Library::~Library()
{
}

void Library::triggerRewriteXmp(void)
{
    DBG_OUT("rewrite_xmp");
    notify(NOTIFY_XMP_NEEDS_UPDATE, boost::any());
}

void Library::notify(NotifyType t, const boost::any & param)
{
    fwk::NotificationCenter::Ptr nc(m_notif_center.lock());
    if(nc) {
        DBG_OUT("notif");
        // pass the notification
        fwk::Notification::Ptr n(new fwk::Notification(niepce::NOTIFICATION_LIB));
        fwk::Notification::mutex_t::Lock lock(n->mutex());
        LibNotification ln;
        ln.type = t;
        ln.param = param;
        n->setData(boost::any(ln));
        nc->post(n);
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
    else if(version != DB_SCHEMA_VERSION)
    {
    }
    return true;
}

bool Library::_initDb()
{
    SQLStatement adminTable("CREATE TABLE admin (key TEXT NOT NULL,"
                            " value TEXT)");
    SQLStatement adminVersion(boost::format("INSERT INTO admin (key, value) "
                                            " VALUES ('version', '%1%')") %
                              DB_SCHEMA_VERSION);
    SQLStatement vaultTable("CREATE TABLE vaults (id INTEGER PRIMARY KEY,"
                            " path TEXT)");
    SQLStatement folderTable("CREATE TABLE folders (id INTEGER PRIMARY KEY,"
                             " path TEXT, name TEXT, vault_id INTEGER, "
                             " parent_id INTEGER)");
    SQLStatement fileTable("CREATE TABLE files (id INTEGER PRIMARY KEY,"
                           " main_file INTEGER, name TEXT, parent_id INTEGER,"
                           " orientation INTEGER, file_type INTEGER, "
                           " file_date INTEGER, rating INTEGER, label INTEGER,"
                           " import_date INTEGER, mod_date INTEGER, "
                           " xmp TEXT, xmp_date INTEGER, xmp_file INTEGER,"
                           " jpeg_file INTEGER)");
    SQLStatement fsFileTable("CREATE TABLE fsfiles (id INTEGER PRIMARY KEY,"
                             " path TEXT)");
    SQLStatement keywordTable("CREATE TABLE keywords (id INTEGER PRIMARY KEY,"
                              " keyword TEXT, parent_id INTEGER)");
    SQLStatement keywordingTable("CREATE TABLE keywording (file_id INTEGER,"
                                 " keyword_id INTEGER)");
    SQLStatement labelTable("CREATE TABLE labels (id INTEGER PRIMARY KEY,"
                            " name TEXT, color TEXT)");
    SQLStatement xmpUpdateQueueTable("CREATE TABLE xmp_update_queue "
                                     " (id INTEGER UNIQUE)");
//		SQLStatement collsTable("CREATE TABLE collections (id INTEGER PRIMARY KEY,"
//								" name TEXT)");
//		SQLStatement collectingTable("CREATE TABLE collecting (file_id INTEGER,"
//									 " collection_id INTEGER)");

    SQLStatement fileUpdateTrigger(
        "CREATE TRIGGER file_update_trigger UPDATE ON files "
        " BEGIN"
        "  UPDATE files SET mod_date = strftime('%s','now');"
        " END");
    SQLStatement xmpUpdateTrigger(
        "CREATE TRIGGER xmp_update_trigger UPDATE OF xmp ON files "
        " BEGIN"
        "  INSERT OR IGNORE INTO xmp_update_queue (id) VALUES(new.id);"
        "  SELECT rewrite_xmp(); "
        " END");

    m_dbdrv->execute_statement(adminTable);
    m_dbdrv->execute_statement(adminVersion);
    m_dbdrv->execute_statement(vaultTable);
    m_dbdrv->execute_statement(folderTable);
    m_dbdrv->execute_statement(fileTable);
    m_dbdrv->execute_statement(fsFileTable);
    m_dbdrv->execute_statement(keywordTable);
    m_dbdrv->execute_statement(keywordingTable);
    m_dbdrv->execute_statement(labelTable);
    m_dbdrv->execute_statement(xmpUpdateQueueTable);
//		m_dbdrv->execute_statement(collsTable);
//		m_dbdrv->execute_statement(collectingTable);

    m_dbdrv->execute_statement(fileUpdateTrigger);
    m_dbdrv->execute_statement(xmpUpdateTrigger);
    notify(NOTIFY_NEW_LIBRARY_CREATED, boost::any());
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
    catch(const fwk::Exception & e)
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


int Library::addFsFile(const std::string & file)
{
    int ret = -1;

    SQLStatement sql(boost::format("INSERT INTO fsfiles (path)"
                                   " VALUES ('%1%')") 
                     % file);
    if(m_dbdrv->execute_statement(sql)) {
        int64_t id = m_dbdrv->last_row_id();
        DBG_OUT("last row inserted %d", (int)id);
        ret = id;
    }
    return ret;
}

std::string Library::getFsFile(int id)
{
    std::string p;
    SQLStatement sql(boost::format("SELECT path FROM fsfiles"
                                   " WHERE id='%1%'") 
                     % id);
    try {
        if(m_dbdrv->execute_statement(sql) &&
           m_dbdrv->read_next_row()) {
            std::string path;
            m_dbdrv->get_column_content(0, path);
            p = path;
        }
    }
    catch(fwk::Exception & e)
    {
        DBG_OUT("db exception %s", e.what());
    }

    return p;
}


int Library::addFile(int folder_id, const std::string & file, bool manage)
{
    int ret = -1;
    DBG_ASSERT(!manage, "manage not supported");
    DBG_ASSERT(folder_id != -1, "invalid folder ID");
    try {
        int32_t rating, label_id, orientation;
        std::string label;  
        fwk::MimeType mime = fwk::MimeType(file);
        eng::LibFile::FileType file_type = eng::LibFile::mimetype_to_filetype(mime);
        fwk::XmpMeta meta(file, file_type == eng::LibFile::FILE_TYPE_RAW);
        label_id = 0;
        orientation = meta.orientation();
        rating = meta.rating();
        label = meta.label();
        time_t creation_date = meta.creation_date();
        if(creation_date == -1) {
            creation_date = 0;
        }

        int fs_file_id = addFsFile(file);
        if(fs_file_id <= 0) {
            throw(fwk::Exception("add fsfile failed"));
        }
        SQLStatement sql(boost::format("INSERT INTO files ("
                                       " main_file, name, parent_id, "
                                       " import_date, mod_date, "
                                       " orientation, file_date, rating, label, file_type,"
                                       " xmp) "
                                       " VALUES ("
                                       " '%1%', '%2%', '%3%', "
                                       " '%4%', '%4%',"
                                       " '%5%', '%6%', '%7%', '%8%', '%9%',"
                                       " ?1);") 
                         % fs_file_id % fwk::path_basename(file) % folder_id
                         % time(NULL)
                         % orientation % creation_date % rating
                         % label_id % file_type);
        std::string buf = meta.serialize_inline();
        sql.bind(1, buf);
        if(m_dbdrv->execute_statement(sql)) {
            int64_t id = m_dbdrv->last_row_id();
            DBG_OUT("last row inserted %d", (int)id);
            ret = id;
            const std::vector< std::string > &keywords(meta.keywords());
            std::vector< std::string >::const_iterator iter;
            for(iter = keywords.begin();
                iter != keywords.end(); iter++) 
            {
                int kwid = makeKeyword(*iter);
                if(kwid != -1) {
                    assignKeyword(kwid, id);
                }
            }
        }
    }
    catch(const fwk::Exception & e)
    {
        DBG_OUT("db exception %s", e.what());
        ret = -1;
    }
    catch(const std::exception & e)
    {
        DBG_OUT("unknown exception %s", e.what());
        ret = -1;
    }
    return ret;
}


int Library::addFileAndFolder(const std::string & folder, const std::string & file, 
                              bool manage)
{
    LibFolder::Ptr f;
    f = getFolder(folder);
    if(f == NULL) {
        ERR_OUT("Folder %s not found", folder.c_str());
    }
    return addFile(f ? f->id() : -1, file, manage);
}

int Library::addBundle(int folder_id, const eng::FileBundle::Ptr & bundle, 
                       bool manage)
{
    int file_id = 0;
    file_id = addFile(folder_id, bundle->main_file(), manage);
    if(file_id > 0) {
        int fsfile_id;
        bool success;
        // addXmpSidecar
        if(!bundle->sidecar().empty()) {
            fsfile_id = addFsFile(bundle->sidecar());
            if(fsfile_id > 0) {
                success = addSidecarFileToBundle(file_id, fsfile_id);
            }
        }
        // addJpeg
        if(!bundle->jpeg().empty()) {
            fsfile_id = addFsFile(bundle->jpeg());
            if(fsfile_id > 0) {
                success = addJpegFileToBundle(file_id, fsfile_id);
            }         
        }
    }
    return file_id;
}

bool Library::addSidecarFileToBundle(int file_id, int fsfile_id)
{
    SQLStatement sql(boost::format("UPDATE files SET xmp_file='%2%'"
                                   " WHERE id='%1%';") 
                     % file_id % fsfile_id);
    try {
        return m_dbdrv->execute_statement(sql);
    }
    catch(fwk::Exception & e)
    {
        DBG_OUT("db exception %s", e.what());
    }
    return false;
}


bool Library::addJpegFileToBundle(int file_id, int fsfile_id)
{
    SQLStatement sql(boost::format("UPDATE files SET jpeg_file='%2%',"
                                   " file_type='%3%' "
                                   " WHERE id='%1%';") 
                     % file_id % fsfile_id % LibFile::FILE_TYPE_RAW_JPEG);
    try {
        return m_dbdrv->execute_statement(sql);
    }
    catch(fwk::Exception & e)
    {
        DBG_OUT("db exception %s", e.what());
    }
    return false;
}


LibFolder::Ptr Library::getFolder(const std::string & folder)
{
    LibFolder::Ptr f;
    SQLStatement sql(boost::format("SELECT id,name "
                                   "FROM folders WHERE path='%1%'") 
                     % folder);
		
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
    catch(fwk::Exception & e)
    {
        DBG_OUT("db exception %s", e.what());
    }
    return f;
}


LibFolder::Ptr Library::addFolder(const std::string & folder)
{
    LibFolder::Ptr f;
    SQLStatement sql(boost::format("INSERT INTO folders "
                                   "(path,name,vault_id,parent_id) "
                                   "VALUES('%1%', '%2%', '0', '0')") 
                     % folder % fwk::path_basename(folder));
    try {
        if(m_dbdrv->execute_statement(sql)) {
            int64_t id = m_dbdrv->last_row_id();
            DBG_OUT("last row inserted %d", (int)id);
            f = LibFolder::Ptr(new LibFolder((int)id, 
                                             fwk::path_basename(folder)));
        }
    }
    catch(fwk::Exception & e)
    {
        DBG_OUT("db exception %s", e.what());
    }
    return f;
}


void Library::getAllFolders(const LibFolder::ListPtr & l)
{
    SQLStatement sql("SELECT id,name FROM folders");
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
    catch(fwk::Exception & e)
    {
        DBG_OUT("db exception %s", e.what());
    }
}

static LibFile::Ptr getFileFromDbRow(const db::IConnectionDriver::Ptr & dbdrv)
{
    int32_t id;
    int32_t fid;
    int32_t fsfid;
    std::string pathname;
    std::string name;
    DBG_ASSERT(dbdrv->get_number_of_columns() == 9, "wrong number of columns");
    dbdrv->get_column_content(0, id);
    dbdrv->get_column_content(1, fid);
    dbdrv->get_column_content(2, pathname);
    dbdrv->get_column_content(3, name);
    dbdrv->get_column_content(8, fsfid);
    DBG_OUT("found %s", pathname.c_str());
    LibFile::Ptr f(new LibFile(id, fid, fsfid,
                               pathname, name));
    int32_t val;
    dbdrv->get_column_content(4, val);
    f->setOrientation(val);
    dbdrv->get_column_content(5, val);
    f->setRating(val);
    dbdrv->get_column_content(6, val);
    f->setLabel(val);

    /* Casting needed. Remember that a simple enum like this is just a couple
     * of #define for integers.
     */
    dbdrv->get_column_content(7, val);
    f->setFileType((eng::LibFile::FileType)val);
    return f;
}

void Library::getFolderContent(int folder_id, const LibFile::ListPtr & fl)
{
    SQLStatement sql(boost::format("SELECT files.id,parent_id,fsfiles.path,"
                                   "name,"
                                   "orientation,rating,label,file_type,"
                                   "fsfiles.id"
                                   " FROM files,fsfiles "
                                   " WHERE parent_id='%1%' "
                                   " AND files.main_file=fsfiles.id")
                     % folder_id);
    try {
        if(m_dbdrv->execute_statement(sql)) {
            while(m_dbdrv->read_next_row()) {
                LibFile::Ptr f(getFileFromDbRow(m_dbdrv));
                fl->push_back(f);
            }
        }
    }
    catch(fwk::Exception & e)
    {
        DBG_OUT("db exception %s", e.what());
    }
}

int Library::countFolder(int folder_id)
{
    int count = -1;
    SQLStatement sql(boost::format("SELECT COUNT(id) FROM files WHERE parent_id='%1%';")
                     % folder_id);
    try {
        if(m_dbdrv->execute_statement(sql)) {
            if(m_dbdrv->read_next_row()) {
                m_dbdrv->get_column_content(0, count);
            }
        }			
    }
    catch(fwk::Exception & e)
    {
        DBG_OUT("db exception %s", e.what());
    }
    return count;
}

void Library::getAllKeywords(const Keyword::ListPtr & l)
{
    SQLStatement sql("SELECT id,keyword FROM keywords ORDER BY keyword");
    try {
        if(m_dbdrv->execute_statement(sql)) {
            while(m_dbdrv->read_next_row()) {
                int32_t id;
                std::string name;
                m_dbdrv->get_column_content(0, id);
                m_dbdrv->get_column_content(1, name);
                l->push_back(Keyword::Ptr(new Keyword(id, name)));
            }
        }
    }
    catch(fwk::Exception & e)
    {
        DBG_OUT("db exception %s", e.what());
    }
}
	
int Library::makeKeyword(const std::string & keyword)
{
    int keyword_id = -1;
    SQLStatement sql("SELECT id FROM keywords WHERE "
                     "keyword=?1;");
    sql.bind(1, keyword);
    try {
        if(m_dbdrv->execute_statement(sql)) {
            if(m_dbdrv->read_next_row()) {
                m_dbdrv->get_column_content(0, keyword_id);
            }
        }
    }
    catch(fwk::Exception & e)
    {
        DBG_OUT("db exception %s", e.what());
    }
    if(keyword_id == -1) {
        SQLStatement sql2("INSERT INTO keywords (keyword, parent_id) "
                          " VALUES(?1, 0);");
        sql2.bind(1, keyword);
        try {
            if(m_dbdrv->execute_statement(sql2)) {
                keyword_id = m_dbdrv->last_row_id();
                Keyword::Ptr kw(new Keyword(keyword_id, keyword));
                notify(NOTIFY_ADDED_KEYWORD, boost::any(kw));
            }
        }
        catch(fwk::Exception & e)
        {
            DBG_OUT("db exception %s", e.what());
        }
    }

    return keyword_id;
}


bool Library::assignKeyword(int kw_id, int file_id)
{
    bool ret = false;
    SQLStatement sql(boost::format("INSERT INTO keywording (file_id, keyword_id) "
                                   " VALUES('%1%', '%2%');") 
                     % file_id % kw_id );
    try {
        ret = m_dbdrv->execute_statement(sql);
    }
    catch(fwk::Exception & e)
    {
        DBG_OUT("db exception %s", e.what());
    }
    return ret;
}


void Library::getKeywordContent(int keyword_id, const LibFile::ListPtr & fl)
{
    SQLStatement sql(boost::format("SELECT files.id,parent_id,fsfiles.path,"
                                   "name,orientation,rating,label,file_type,"
                                   " fsfiles.id "
                                   " FROM files,fsfiles "
                                   " WHERE files.id IN "
                                   " (SELECT file_id FROM keywording "
                                   " WHERE keyword_id='%1%') "
                                   " AND fsfiles.id = files.main_file;")
                     % keyword_id);
    try {
        if(m_dbdrv->execute_statement(sql)) {
            while(m_dbdrv->read_next_row()) {
                LibFile::Ptr f(getFileFromDbRow(m_dbdrv));
                fl->push_back(f);
            }
        }
    }
    catch(fwk::Exception & e)
    {
        DBG_OUT("db exception %s", e.what());
    }
}


void Library::getMetaData(int file_id, const LibMetadata::Ptr & meta)
{
    SQLStatement sql(boost::format("SELECT xmp FROM files "
                                   " WHERE id='%1%';")
                     % file_id);
    try {
        if(m_dbdrv->execute_statement(sql)) {
            while(m_dbdrv->read_next_row()) {
                std::string xml;
                m_dbdrv->get_column_content(0, xml);
                meta->unserialize(xml.c_str());
            }
        }
    }
    catch(fwk::Exception & e)
    {
        DBG_OUT("db exception %s", e.what());
    }
}




bool Library::setInternalMetaDataInt(int file_id, const char* col,
                                     int32_t value)
{
    bool ret = false;
    DBG_OUT("setting metadata in column %s", col);
    SQLStatement sql(boost::format("UPDATE files SET %1%='%2%' "
                                   " WHERE id='%3%';")
                     % col % value % file_id);
    try {
        ret = m_dbdrv->execute_statement(sql);
    }
    catch(fwk::Exception & e)
    {
        DBG_OUT("db exception %s", e.what());
        ret = false;
    }
    return ret;
}

/** set metadata block
 * @param file_id the file ID to set the metadata block
 * @param meta the metadata block
 * @return false on error
 */
bool Library::setMetaData(int file_id, const LibMetadata::Ptr & meta)
{
    bool ret = false;
    SQLStatement sql(boost::format("UPDATE files SET xmp=?1 "
                                   " WHERE id='%1%';")
                     % file_id);
    sql.bind(1, meta->serialize_inline());
    try {
        ret = m_dbdrv->execute_statement(sql);
    }
    catch(fwk::Exception & e)
    {
        DBG_OUT("db exception %s", e.what());
        ret = false;
    }
    return ret;    
}


/** set metadata 
 * @param file_id the file ID to set the metadata block
 * @param meta the metadata index
 * @param value the value to set
 * @return false on error
 */
bool Library::setMetaData(int file_id, int meta, 
                          const boost::any & value)
{
    bool retval = false;
    DBG_OUT("setting metadata in column %x", meta);
    switch(meta) {
    case MAKE_METADATA_IDX(eng::META_NS_XMPCORE, eng::META_XMPCORE_RATING):
    case MAKE_METADATA_IDX(eng::META_NS_XMPCORE, eng::META_XMPCORE_LABEL):
    case MAKE_METADATA_IDX(eng::META_NS_TIFF, eng::META_TIFF_ORIENTATION):
        if(value.type() == typeid(int32_t)) {
            // internal.
            int32_t nvalue = boost::any_cast<int32_t>(value);
            // make the column mapping more generic.
            const char * col = NULL;
            switch(meta) {
            case MAKE_METADATA_IDX(eng::META_NS_XMPCORE, eng::META_XMPCORE_RATING):
                col = "rating";
                break;
            case MAKE_METADATA_IDX(eng::META_NS_TIFF, eng::META_TIFF_ORIENTATION):
                col = "orientation";
                break;
            case MAKE_METADATA_IDX(eng::META_NS_XMPCORE, eng::META_XMPCORE_LABEL):
                col = "label";
                break;
            }
            if(col) {
                retval = setInternalMetaDataInt(file_id, col, nvalue);
            }
        }
        break;
    default:
        // external.
        ERR_OUT("unknown metadata to set");
        return false;
    }
    LibMetadata::Ptr metablock(new LibMetadata(file_id));
    getMetaData(file_id, metablock);
    retval = metablock->setMetaData(meta, value);
    retval = metablock->touch();
    retval = setMetaData(file_id, metablock);
    return retval;
}

void Library::getAllLabels(const Label::ListPtr & l)
{
    SQLStatement sql("SELECT id,name,color FROM labels ORDER BY id");
    try {
        if(m_dbdrv->execute_statement(sql)) {
            while(m_dbdrv->read_next_row()) {
                int32_t id;
                std::string name;
                std::string color;
                m_dbdrv->get_column_content(0, id);
                m_dbdrv->get_column_content(1, name);
                m_dbdrv->get_column_content(2, color);
                l->push_back(Label::Ptr(new Label(id, name, color)));
            }
        }
    }
    catch(fwk::Exception & e)
    {
        DBG_OUT("db exception %s", e.what());
    }
}


int Library::addLabel(const std::string & name, const std::string & color)
{
    int ret = -1;

    SQLStatement sql(boost::format("INSERT INTO labels (name,color)"
                                   " VALUES ('%1%', '%2%')") 
                     % name % color);
    if(m_dbdrv->execute_statement(sql)) {
        int64_t id = m_dbdrv->last_row_id();
        DBG_OUT("last row inserted %d", (int)id);
        ret = id;
    }
    return ret;
}


int Library::addLabel(const std::string & name, const fwk::RgbColor & c)
{
    return addLabel(name, c.to_string());
}


bool Library::updateLabel(int label_id, const std::string & name, const std::string & color)
{
    SQLStatement sql(boost::format("UPDATE labels SET name='%2%', color='%3%'"
                                   " WHERE id='%1%';") 
                     % label_id % name % color);
    try {
        return m_dbdrv->execute_statement(sql);
    }
    catch(fwk::Exception & e)
    {
        DBG_OUT("db exception %s", e.what());
    }
    return false;
}


bool Library::deleteLabel(int label_id)
{
    
    SQLStatement sql(boost::format("DELETE FROM labels "
                                   " WHERE id='%1%';") % label_id);
    try {
        return m_dbdrv->execute_statement(sql);
    }
    catch(fwk::Exception & e)
    {
        DBG_OUT("db exception %s", e.what());
    }
    return false;
}


bool Library::getXmpIdsInQueue(std::vector<int> & ids)
{
    SQLStatement sql("SELECT id  FROM xmp_update_queue;");
    try {
        if(m_dbdrv->execute_statement(sql)) {
            while(m_dbdrv->read_next_row()) {
                int32_t id;
                m_dbdrv->get_column_content(0, id);
                ids.push_back(id);
            }
        }
    }
    catch(fwk::Exception & e)
    {
        DBG_OUT("db exception %s", e.what());
        return false;
    }
    return true;
}


bool Library::rewriteXmpForId(int id)
{
    SQLStatement del(boost::format("DELETE FROM xmp_update_queue "
                                   " WHERE id='%1%';") % id);
    SQLStatement getxmp(boost::format("SELECT xmp, main_file, xmp_file FROM files "
                                   " WHERE id='%1%';") % id);
    try {
        
        if(m_dbdrv->execute_statement(del) 
           && m_dbdrv->execute_statement(getxmp)) {
            while(m_dbdrv->read_next_row()) {
                std::string xmp_buffer;
                int main_file_id;
                int xmp_file_id;
                m_dbdrv->get_column_content(0, xmp_buffer);
                m_dbdrv->get_column_content(1, main_file_id);
                m_dbdrv->get_column_content(2, xmp_file_id);
                std::string spath = getFsFile(main_file_id);
                DBG_ASSERT(!spath.empty(), "couldn't find the main file");
                std::string p;
                if(xmp_file_id > 0) {
                    p = getFsFile(xmp_file_id);
                    DBG_ASSERT(!p.empty(), "couldn't find the xmp file path");
                }
                if(p.empty()) {
                    p = fwk::path_replace_extension(spath, ".xmp");
                    DBG_ASSERT(p != spath, "path must have been changed");
                }
                if(fwk::path_exists(p)) {
                    DBG_OUT("%s already exist", p.c_str());
                    // TODO backup
                }
                // TODO probably a faster way to do that
                fwk::XmpMeta xmppacket;
                xmppacket.unserialize(xmp_buffer.c_str());
                // TODO use different API
                FILE * f = fopen(p.c_str(), "w");
                if(f) {
                    std::string sidecar = xmppacket.serialize();
                    fwrite(sidecar.c_str(), sizeof(std::string::value_type),
                           sidecar.size(), f);
                    fclose(f);
                    if(xmp_file_id <= 0) {
                        xmp_file_id = addFsFile(p);
                        DBG_ASSERT(xmp_file_id > 0, "couldn't add xmp_file");
                        bool res = addSidecarFileToBundle(id, xmp_file_id);
                        DBG_ASSERT(res, "addSidecarFileToBundle failed");
                    }
                }
                // TODO rewrite the modified date in the files table
                // caveat: this will trigger this rewrite recursively.
            }
        }
    }
    catch(fwk::Exception & e)
    {
        DBG_OUT("db exception %s", e.what());
        return false;
    }    
    return true;
}


bool Library::processXmpUpdateQueue()
{
    bool retval = false;
    std::vector<int> ids;
    retval = getXmpIdsInQueue(ids);
    if(retval) {
        std::for_each(ids.begin(), ids.end(),
                     boost::bind(&Library::rewriteXmpForId,
                                 this, _1));
    }
    return retval;
}


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
