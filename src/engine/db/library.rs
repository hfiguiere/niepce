/*
 * niepce - engine/db/library.rs
 *
 * Copyright (C) 2017 Hubert Figuière
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

use std::path::{Path, PathBuf};
use std::fs::File;
use std::io::Write;

use chrono::Utc;
use rusqlite;

use super::{FromDb, LibraryId};
use engine::db::label::Label;
use engine::db::libfolder;
use engine::db::libfolder::LibFolder;
use engine::db::libfile;
use engine::db::libfile::LibFile;
use engine::db::libmetadata::LibMetadata;
use engine::db::filebundle::{FileBundle, Sidecar};
use engine::db::keyword::Keyword;
use engine::library::notification::Notification as LibNotification;
use engine::library::notification::engine_library_notify;
use fwk;
use fwk::PropertyValue;
use root::eng::NiepceProperties as Np;

#[repr(i32)]
#[derive(PartialEq, Clone, Copy)]
pub enum Managed {
    NO = 0,
    YES = 1,
}

const DB_SCHEMA_VERSION: i32 = 7;
const DATABASENAME: &str = "niepcelibrary.db";

pub struct Library {
    // maindir: PathBuf,
    dbpath: PathBuf,
    dbconn: Option<rusqlite::Connection>,
    inited: bool,
    notif_id: u64,
}

impl Library {
    pub fn new(dir: PathBuf, notif_id: u64) -> Library {
        let mut dbpath = dir.clone();
        dbpath.push(DATABASENAME);
        let mut lib = Library {
            // maindir: dir,
            dbpath: dbpath,
            dbconn: None,
            inited: false,
            notif_id: notif_id,
        };

        lib.inited = lib.init();

        lib
    }

    fn init(&mut self) -> bool {
        let conn_attempt = rusqlite::Connection::open(self.dbpath.clone());
        if !conn_attempt.is_ok() {
            return false;
        }
        let conn = conn_attempt.unwrap();
        let notif_id = self.notif_id;
        if let Ok(_) = conn.create_scalar_function("rewrite_xmp", 0, false, |_| {
            Library::notify_by_id(notif_id, Box::new(LibNotification::XmpNeedsUpdate));
            Ok(true)
        }) {
            self.dbconn = Some(conn);
        } else {
            err_out!("failed to create scalar functin.");
            return false;
        }

        let version = self.check_database_version();
        if version == -1 {
            // error
            dbg_out!("version check -1");
        } else if version == 0 {
            // let's create our DB
            dbg_out!("version == 0");
            return self.init_db();
        } else if version != DB_SCHEMA_VERSION {
            // WAT?
        }
        true
    }

    pub fn dbpath(&self) -> &Path {
        &self.dbpath
    }

    pub fn is_ok(&self) -> bool {
        self.inited
    }

    fn check_database_version(&self) -> i32 {
        if let Some(ref conn) = self.dbconn {
            if let Ok(mut stmt) = conn.prepare("SELECT value FROM admin WHERE key='version'") {
                let mut rows = stmt.query(&[]).unwrap();
                if let Some(Ok(row)) = rows.next() {
                    let value: String = row.get(0);
                    if let Ok(v) = i32::from_str_radix(&value, 10) {
                        return v;
                    } else {
                        return -1;
                    }
                }
            } else {
                // if query fail we assume 0 to create the database.
                return 0;
            }
        }

        -1
    }

    fn init_db(&mut self) -> bool {
        if let Some(ref conn) = self.dbconn {
            conn.execute("CREATE TABLE admin (key TEXT NOT NULL, value TEXT)", &[])
                .unwrap();
            conn.execute(
                "INSERT INTO admin (key, value) \
                 VALUES ('version', ?1)",
                &[&DB_SCHEMA_VERSION],
            ).unwrap();
            conn.execute(
                "CREATE TABLE vaults (id INTEGER PRIMARY KEY, path TEXT)",
                &[],
            ).unwrap();
            conn.execute(
                "CREATE TABLE folders (id INTEGER PRIMARY KEY,\
                 path TEXT, name TEXT, \
                 vault_id INTEGER DEFAULT 0, \
                 locked INTEGER DEFAULT 0, \
                 virtual INTEGER DEFAULT 0, \
                 expanded INTEGER DEFAULT 0, \
                 parent_id INTEGER)",
                &[],
            ).unwrap();
            let trash_type = libfolder::FolderVirtualType::TRASH as i32;
            conn.execute(
                "insert into folders (name, locked, virtual, parent_id, path) \
                 values (:1, 1, :2, 0, '')",
                &[&"Trash", &trash_type],
            ).unwrap();

            conn.execute(
                "CREATE TABLE files (id INTEGER PRIMARY KEY,\
                 main_file INTEGER, name TEXT, parent_id INTEGER,\
                 orientation INTEGER, file_type INTEGER,\
                 file_date INTEGER, rating INTEGER DEFAULT 0, \
                 label INTEGER, flag INTEGER DEFAULT 0, \
                 import_date INTEGER, mod_date INTEGER, \
                 xmp TEXT, xmp_date INTEGER, xmp_file INTEGER,\
                 jpeg_file INTEGER)",
                &[],
            ).unwrap();
            conn.execute(
                "CREATE TABLE fsfiles (id INTEGER PRIMARY KEY,\
                 path TEXT)",
                &[],
            ).unwrap();
            // version = 7
            conn.execute(
                "CREATE TABLE sidecars (file_id INTEGER,\
                 fsfile_id INTEGER, type INTEGER, UNIQUE(file_id, fsfile_id))",
                &[],
            ).unwrap();
            conn.execute(
                "CREATE TRIGGER file_delete_trigger AFTER DELETE ON files \
                 BEGIN \
                 DELETE FROM sidecars WHERE file_id = old.id; \
                 END",
                &[],
            ).unwrap();
            //
            conn.execute(
                "CREATE TABLE keywords (id INTEGER PRIMARY KEY,\
                 keyword TEXT, parent_id INTEGER DEFAULT 0)",
                &[],
            ).unwrap();
            conn.execute(
                "CREATE TABLE keywording (file_id INTEGER,\
                 keyword_id INTEGER, UNIQUE(file_id, keyword_id))",
                &[],
            ).unwrap();
            conn.execute(
                "CREATE TABLE labels (id INTEGER PRIMARY KEY,\
                 name TEXT, color TEXT)",
                &[],
            ).unwrap();
            conn.execute("CREATE TABLE xmp_update_queue (id INTEGER UNIQUE)", &[])
                .unwrap();
            conn.execute(
                "CREATE TRIGGER file_update_trigger UPDATE ON files \
                 BEGIN \
                 UPDATE files SET mod_date = strftime('%s','now');\
                 END",
                &[],
            ).unwrap();
            conn.execute(
                "CREATE TRIGGER xmp_update_trigger UPDATE OF xmp ON files \
                 BEGIN \
                 INSERT OR IGNORE INTO xmp_update_queue (id) VALUES(new.id);\
                 SELECT rewrite_xmp();\
                 END",
                &[],
            ).unwrap();

            self.notify(Box::new(LibNotification::LibCreated));
            return true;
        }
        false
    }

    pub fn notify(&self, notif: Box<LibNotification>) {
        unsafe {
            engine_library_notify(self.notif_id, Box::into_raw(notif));
        }
    }

    pub fn notify_by_id(id: u64, notif: Box<LibNotification>) {
        unsafe {
            engine_library_notify(id, Box::into_raw(notif));
        }
    }

    pub fn add_jpeg_file_to_bundle(&self, file_id: LibraryId, fsfile_id: LibraryId) -> bool {
        if let Some(ref conn) = self.dbconn {
            let filetype: i32 = libfile::FileType::RAW_JPEG.into();
            if let Ok(c) = conn.execute(
                "UPDATE files SET jpeg_file=:1 file_type=:3 WHERE id=:2;",
                &[&fsfile_id, &file_id, &filetype],
            ) {
                if c == 1 {
                    return true;
                }
            }
        }
        false
    }

    pub fn add_xmp_sidecar_to_bundle(&self, file_id: LibraryId, fsfile_id: LibraryId) -> bool {
        if let Some(ref conn) = self.dbconn {
            if let Ok(c) = conn.execute(
                "UPDATE files SET xmp_file=:1 WHERE id=:2;",
                &[&fsfile_id, &file_id],
            ) {
                if c == 1 {
                    return true;
                }
            }
        }
        false
    }

    pub fn add_sidecar_file_to_bundle(&self, file_id: LibraryId, sidecar: &Sidecar) -> bool {
        let sidecar_t: (i32, &String) = match sidecar {
            &Sidecar::Live(ref p) => (1, p),
            &Sidecar::Thumbnail(ref p) => (2, p),
            _ => return false,
        };
        let fsfile_id = self.add_fs_file(&sidecar_t.1);
        if fsfile_id == -1 {
            return false;
        }
        if let Some(ref conn) = self.dbconn {
            if let Ok(c) = conn.execute(
                "INSERT INTO sidecars (file_id, fsfile_id, type) VALUES(:1, :2, :3)",
                &[&file_id, &fsfile_id, &sidecar_t.0],
            ) {
                if c == 1 {
                    return true;
                }
            }
        }
        false
    }

    pub fn leaf_name_for_pathname(pathname: &str) -> Option<String> {
        let name = try_opt!(Path::new(pathname).file_name());
        Some(String::from(try_opt!(name.to_str())))
    }

    fn get_content(&self, id: LibraryId, sql_where: &str) -> Option<Vec<LibFile>> {
        let conn = try_opt!(self.dbconn.as_ref());
        let sql = format!(
            "SELECT {} FROM {} \
             WHERE {} \
             AND files.main_file=fsfiles.id",
            LibFile::read_db_columns(),
            LibFile::read_db_tables(),
            sql_where
        );
        let mut stmt = try_opt!(conn.prepare(&sql).ok());
        let mut rows = try_opt!(stmt.query(&[&id]).ok());
        let mut files: Vec<LibFile> = vec![];
        while let Some(Ok(row)) = rows.next() {
            files.push(LibFile::read_from(&row));
        }
        Some(files)
    }

    /// Add a folder at the root.
    pub fn add_folder(&self, name: &str, path: Option<String>) -> Option<LibFolder> {
        self.add_folder_into(name, path, 0)
    }

    /// Add folder with name into parent whose id is `into`.
    /// A value of 0 means root.
    pub fn add_folder_into(
        &self,
        name: &str,
        path: Option<String>,
        into: LibraryId,
    ) -> Option<LibFolder> {
        let conn = try_opt!(self.dbconn.as_ref());
        let c = try_opt!(conn.execute(
            "INSERT INTO folders (path,name,vault_id,parent_id) VALUES(:1, :2, '0', :3)",
            &[&path, &name, &into]
        ).ok());
        if c != 1 {
            return None;
        }
        let id = conn.last_insert_rowid();
        dbg_out!("last row inserted {}", id);
        let mut lf = LibFolder::new(id, &name, path);
        lf.set_parent(into);
        return Some(lf);
    }

    pub fn delete_folder(&self, id: LibraryId) -> bool {
        if let Some(ref conn) = self.dbconn {
            if let Some(c) = conn.execute("DELETE FROM folders WHERE id=:1", &[&id]).ok() {
                if c == 1 {
                    return true;
                }
            }
        }
        false
    }

    pub fn get_folder(&self, folder: &str) -> Option<LibFolder> {
        let foldername = try_opt!(Self::leaf_name_for_pathname(folder));
        let conn = try_opt!(self.dbconn.as_ref());
        let sql = format!(
            "SELECT {} FROM {} WHERE path=:1",
            LibFolder::read_db_columns(),
            LibFolder::read_db_tables()
        );
        let mut stmt = try_opt!(conn.prepare(&sql).ok());
        let mut rows = try_opt!(stmt.query(&[&foldername]).ok());
        let row = try_opt!(try_opt!(rows.next()).ok());
        return Some(LibFolder::read_from(&row));
    }

    pub fn get_all_folders(&self) -> Option<Vec<LibFolder>> {
        let conn = try_opt!(self.dbconn.as_ref());
        let sql = format!(
            "SELECT {} FROM {}",
            LibFolder::read_db_columns(),
            LibFolder::read_db_tables()
        );
        let mut stmt = try_opt!(conn.prepare(&sql).ok());
        let mut rows = try_opt!(stmt.query(&[]).ok());
        let mut folders: Vec<LibFolder> = vec![];
        while let Some(Ok(row)) = rows.next() {
            folders.push(LibFolder::read_from(&row));
        }
        Some(folders)
    }

    pub fn get_folder_content(&self, folder_id: LibraryId) -> Option<Vec<LibFile>> {
        self.get_content(folder_id, "parent_id = :1")
    }

    pub fn count_folder(&self, folder_id: LibraryId) -> i64 {
        if let Some(ref conn) = self.dbconn {
            if let Ok(mut stmt) = conn.prepare(
                "SELECT COUNT(id) FROM files \
                 WHERE parent_id=:1;",
            ) {
                let mut rows = stmt.query(&[&folder_id]).unwrap();
                if let Some(Ok(row)) = rows.next() {
                    return row.get(0);
                }
            }
        }
        -1
    }

    pub fn get_all_keywords(&self) -> Option<Vec<Keyword>> {
        let conn = try_opt!(self.dbconn.as_ref());
        let sql = format!(
            "SELECT {} FROM {}",
            Keyword::read_db_columns(),
            Keyword::read_db_tables()
        );
        let mut stmt = try_opt!(conn.prepare(&sql).ok());
        let mut rows = try_opt!(stmt.query(&[]).ok());
        let mut keywords: Vec<Keyword> = vec![];
        while let Some(Ok(row)) = rows.next() {
            keywords.push(Keyword::read_from(&row));
        }
        Some(keywords)
    }

    pub fn count_keyword(&self, id: LibraryId) -> i64 {
        if let Some(ref conn) = self.dbconn {
            if let Ok(mut stmt) = conn.prepare(
                "SELECT COUNT(keyword_id) FROM keywording \
                 WHERE keyword_id=:1;",
            ) {
                let mut rows = stmt.query(&[&id]).unwrap();
                if let Some(Ok(row)) = rows.next() {
                    return row.get(0);
                }
            }
        }
        -1
    }

    pub fn add_fs_file(&self, file: &str) -> LibraryId {
        if let Some(ref conn) = self.dbconn {
            if let Ok(c) = conn.execute("INSERT INTO fsfiles (path) VALUES(:1)", &[&file]) {
                if c != 1 {
                    return -1;
                }
                return conn.last_insert_rowid();
            }
        }

        -1
    }

    fn get_fs_file(&self, id: LibraryId) -> Option<String> {
        let conn = try_opt!(self.dbconn.as_ref());
        let mut stmt = try_opt!(conn.prepare("SELECT path FROM fsfiles WHERE id=:1").ok());
        let mut rows = try_opt!(stmt.query(&[&id]).ok());
        let row = try_opt!(try_opt!(rows.next()).ok());
        let path: String = row.get(0);
        return Some(path);
    }

    pub fn add_bundle(
        &self,
        folder_id: LibraryId,
        bundle: &FileBundle,
        manage: Managed,
    ) -> LibraryId {
        let file_id = self.add_file(folder_id, bundle.main(), manage);
        if file_id > 0 {
            if !bundle.xmp_sidecar().is_empty() {
                let fsfile_id = self.add_fs_file(bundle.xmp_sidecar());
                if fsfile_id > 0 {
                    self.add_xmp_sidecar_to_bundle(file_id, fsfile_id);
                }
            }
            if !bundle.jpeg().is_empty() {
                let fsfile_id = self.add_fs_file(bundle.jpeg());
                if fsfile_id > 0 {
                    self.add_jpeg_file_to_bundle(file_id, fsfile_id);
                }
            }
        }
        file_id
    }

    pub fn add_file(&self, folder_id: LibraryId, file: &str, manage: Managed) -> LibraryId {
        let mut ret: LibraryId = -1;
        dbg_assert!(manage == Managed::NO, "manage not supported");
        dbg_assert!(folder_id != -1, "invalid folder ID");
        let mime = fwk::MimeType::new(file);
        let file_type = libfile::mimetype_to_filetype(&mime);
        let label_id: LibraryId = 0;
        let orientation: i32;
        let rating: i32;
        //let label: String; // XXX fixme
        let flag: i32;
        let creation_date: fwk::Time;
        let xmp: String;
        let meta = fwk::XmpMeta::new_from_file(file, file_type == libfile::FileType::RAW);
        if let Some(ref meta) = meta {
            orientation = meta.orientation().unwrap_or(0);
            rating = meta.rating().unwrap_or(0);
            //label = meta.label().unwrap_or(String::from(""));
            flag = meta.flag().unwrap_or(0);
            if let Some(ref date) = meta.creation_date() {
                creation_date = date.timestamp();
            } else {
                creation_date = 0
            }
            xmp = meta.serialize_inline();
        } else {
            orientation = 0;
            rating = 0;
            //label = String::from("");
            flag = 0;
            creation_date = 0;
            xmp = String::from("");
        }

        let filename = Self::leaf_name_for_pathname(file).unwrap_or(String::from(""));
        let fs_file_id = self.add_fs_file(file);
        if fs_file_id <= 0 {
            err_out!("add fsfile failed");
            return 0;
        }

        if let Some(ref conn) = self.dbconn {
            let ifile_type = file_type as i32;
            let time = Utc::now().timestamp();
            ret = match conn.execute(
                "INSERT INTO files (\
                 main_file, name, parent_id, \
                 import_date, mod_date, \
                 orientation, file_date, rating, label, \
                 file_type, flag, xmp) \
                 VALUES (\
                 :1, :2, :3, :4, :5, :6, :7, :8, :9, :10, :11, :12)",
                &[
                    &fs_file_id,
                    &filename,
                    &folder_id,
                    &time,
                    &time,
                    &orientation,
                    &creation_date,
                    &rating,
                    &label_id,
                    &ifile_type,
                    &flag,
                    &xmp,
                ],
            ) {
                Ok(c) => {
                    let mut id = -1;
                    if c == 1 {
                        id = conn.last_insert_rowid();
                        if let Some(mut meta) = meta {
                            let keywords = meta.keywords();
                            for k in keywords {
                                let kwid = self.make_keyword(k);
                                if kwid != -1 {
                                    self.assign_keyword(kwid, id);
                                }
                            }
                        }
                    }
                    id
                }
                Err(_) => -1,
            }
        }

        ret
    }

    pub fn make_keyword(&self, keyword: &str) -> LibraryId {
        if let Some(ref conn) = self.dbconn {
            if let Ok(mut stmt) = conn.prepare(
                "SELECT id FROM keywords WHERE \
                 keyword=:1;",
            ) {
                let mut rows = stmt.query(&[&keyword]).unwrap();
                if let Some(Ok(row)) = rows.next() {
                    let keyword_id = row.get(0);
                    if keyword_id > 0 {
                        return keyword_id;
                    }
                }
            }

            if let Ok(c) = conn.execute(
                "INSERT INTO keywords (keyword, parent_id) VALUES(:1, 0);",
                &[&keyword],
            ) {
                if c != 1 {
                    return -1;
                }
                let keyword_id = conn.last_insert_rowid();
                self.notify(Box::new(LibNotification::AddedKeyword(Keyword::new(
                    keyword_id,
                    keyword,
                ))));
                return keyword_id;
            }
        }
        -1
    }

    pub fn assign_keyword(&self, kw_id: LibraryId, file_id: LibraryId) -> bool {
        if let Some(ref conn) = self.dbconn {
            if let Ok(_) = conn.execute(
                "INSERT OR IGNORE INTO keywording\
                 (file_id, keyword_id) \
                 VALUES(:1, :2)",
                &[&kw_id, &file_id],
            ) {
                return true;
            }
        }
        false
    }

    pub fn get_keyword_content(&self, keyword_id: LibraryId) -> Option<Vec<LibFile>> {
        self.get_content(
            keyword_id,
            "files.id IN \
             (SELECT file_id FROM keywording \
             WHERE keyword_id=:1) ",
        )
    }

    pub fn get_metadata(&self, file_id: LibraryId) -> Option<LibMetadata> {
        let conn = try_opt!(self.dbconn.as_ref());
        let sql = format!(
            "SELECT {} FROM {} WHERE id=:1",
            LibMetadata::read_db_columns(),
            LibMetadata::read_db_tables()
        );
        let mut stmt = try_opt!(conn.prepare(&sql).ok());
        let mut rows = try_opt!(stmt.query(&[&file_id]).ok());
        let row = try_opt!(try_opt!(rows.next()).ok());
        return Some(LibMetadata::read_from(&row));
    }

    fn unassign_all_keywords_for_file(&self, file_id: LibraryId) -> bool {
        if let Some(ref conn) = self.dbconn {
            if let Ok(_) = conn.execute(
                "DELETE FROM keywording \
                 WHERE file_id=:1;",
                &[&file_id],
            ) {
                // XXX check success.
                return true;
            }
        }
        false
    }

    fn set_internal_metadata(&self, file_id: LibraryId, column: &str, value: i32) -> bool {
        if let Some(ref conn) = self.dbconn {
            match conn.execute(
                format!(
                    "UPDATE files SET {}=:1 \
                     WHERE id=:2;",
                    column
                ).as_ref(),
                &[&value, &file_id],
            ) {
                Ok(_) => {
                    // XXX check success.
                    return true;
                }
                Err(err) => {
                    err_out!("error setting internal metadata {}", err);
                }
            }
        }
        false
    }

    fn set_metadata_block(&self, file_id: LibraryId, metablock: &LibMetadata) -> bool {
        let xmp = metablock.serialize_inline();
        if let Some(ref conn) = self.dbconn {
            if let Ok(_) = conn.execute(
                "UPDATE files SET xmp=:1 \
                 WHERE id=:2;",
                &[&xmp, &file_id],
            ) {
                // XXX check success.
                return true;
            } else {
                err_out!("error setting metadatablock");
            }
        }
        false
    }

    pub fn set_metadata(&self, file_id: LibraryId, meta: Np, value: &PropertyValue) -> bool {
        let mut retval = false;
        match meta {
            Np::NpXmpRatingProp |
            Np::NpXmpLabelProp |
            Np::NpTiffOrientationProp |
            Np::NpNiepceFlagProp => {
                match *value {
                    PropertyValue::Int(i) => {
                        // internal
                        // make the column mapping more generic.
                        let column = match meta {
                            Np::NpXmpRatingProp =>
                                "rating",
                            Np::NpXmpLabelProp =>
                                "orientation",
                            Np::NpTiffOrientationProp =>
                                "label",
                            Np::NpNiepceFlagProp =>
                                "flag",
                            _ =>
                                unreachable!()
                        };
                        if !column.is_empty() {
                            retval = self.set_internal_metadata(file_id, column, i);
                            if !retval {
                                err_out!("set_internal_metadata() failed");
                                return false;
                            }
                        }
                    },
                    _ =>
                        err_out!("improper value type for {:?}", meta),
                }
            },
            Np::NpIptcKeywordsProp => {
                self.unassign_all_keywords_for_file(file_id);

                match value {
                    &PropertyValue::StringArray(ref keywords) =>
                        for kw in keywords {
                            let id = self.make_keyword(&kw);
                            if id != -1 {
                                self.assign_keyword(id, file_id);
                            }
                        },
                    _ =>
                        err_out!("improper value_type for {:?} : {:?}", meta, value),
                }
            },
            _ =>
                // XXX TODO
                err_out!("unhandled meta {:?}", meta),
        }
        if let Some(mut metablock) = self.get_metadata(file_id) {
            metablock.set_metadata(meta, value);
            metablock.touch();
            retval = self.set_metadata_block(file_id, &metablock);
        }

        retval
    }

    pub fn move_file_to_folder(&self, file_id: LibraryId, folder_id: LibraryId) -> bool {
        if let Some(ref conn) = self.dbconn {
            if let Ok(mut stmt) = conn.prepare(
                "SELECT id FROM folders WHERE \
                 id=:1;",
            ) {
                let mut rows = stmt.query(&[&folder_id]).unwrap();
                if let Some(Ok(_)) = rows.next() {
                    if let Ok(_) = conn.execute(
                        "UPDATE files SET parent_id = :1 \
                         WHERE id = :2;",
                        &[&folder_id, &file_id],
                    ) {
                        return true;
                    }
                }
            }
        }
        false
    }

    pub fn get_all_labels(&self) -> Option<Vec<Label>> {
        let conn = try_opt!(self.dbconn.as_ref());
        let sql = format!(
            "SELECT {} FROM {} ORDER BY id;",
            Label::read_db_columns(),
            Label::read_db_tables()
        );
        let mut stmt = try_opt!(conn.prepare(&sql).ok());
        let mut rows = try_opt!(stmt.query(&[]).ok());
        let mut labels: Vec<Label> = vec![];
        while let Some(Ok(row)) = rows.next() {
            labels.push(Label::read_from(&row));
        }
        Some(labels)
    }

    pub fn add_label(&self, name: &str, colour: &str) -> LibraryId {
        if let Some(ref conn) = self.dbconn {
            if let Ok(c) = conn.execute(
                "INSERT INTO  labels (name,color) \
                 VALUES (:1, :2);",
                &[&name, &colour],
            ) {
                if c != 1 {
                    return -1;
                }
                let label_id = conn.last_insert_rowid();
                dbg_out!("last row inserted {}", label_id);
                return label_id;
            }
        }
        -1
    }

    pub fn update_label(&self, label_id: LibraryId, name: &str, colour: &str) -> bool {
        if let Some(ref conn) = self.dbconn {
            if let Ok(_) = conn.execute(
                "UPDATE labels SET name=:2, color=:3 \
                 FROM labels WHERE id=:1;",
                &[&label_id, &name, &colour],
            ) {
                // XXX check success.
                return true;
            }
        }
        false
    }

    pub fn delete_label(&self, label_id: LibraryId) -> bool {
        if let Some(ref conn) = self.dbconn {
            if let Ok(_) = conn.execute("DELETE FROM labels WHERE id=:1;", &[&label_id]) {
                // XXX check success.
                return true;
            }
        }
        false
    }

    fn get_xmp_ids_in_queue(&self) -> Option<Vec<LibraryId>> {
        let conn = try_opt!(self.dbconn.as_ref());
        let mut stmt = try_opt!(conn.prepare("SELECT id FROM xmp_update_queue;").ok());
        let mut ids = Vec::<LibraryId>::new();
        let mut rows = try_opt!(stmt.query(&[]).ok());
        while let Some(Ok(row)) = rows.next() {
            let id: i64 = row.get(0);
            ids.push(id);
        }
        return Some(ids);
    }

    pub fn write_metadata(&self, id: LibraryId) -> bool {
        self.rewrite_xmp_for_id(id, true)
    }

    fn rewrite_xmp_for_id(&self, id: LibraryId, write_xmp: bool) -> bool {
        // XXX
        // Rework this so that:
        // 1. it returns a Err<>
        // 2. it only delete if the xmp file has been updated properly
        // 3. make sure the update happened correctly, possibly ensure we don't
        // clobber the xmp.
        if let Some(ref conn) = self.dbconn {
            if let Ok(_) = conn.execute("DELETE FROM xmp_update_queue WHERE id=:1;", &[&id]) {
                // we don't want to write the XMP so we don't need to list them.
                if !write_xmp {
                    return true;
                }
                if let Ok(mut stmt) = conn.prepare(
                    "SELECT xmp, main_file, xmp_file FROM files \
                     WHERE id=:1;",
                ) {
                    let mut rows = stmt.query(&[&id]).unwrap();
                    while let Some(Ok(row)) = rows.next() {
                        let xmp_buffer: String = row.get(0);
                        let main_file_id: LibraryId = row.get(1);
                        let xmp_file_id: LibraryId = row.get(2);
                        let spath: PathBuf;
                        if let Some(ref p) = self.get_fs_file(main_file_id) {
                            spath = PathBuf::from(p);
                        } else {
                            // XXX we should report that error.
                            dbg_assert!(false, "couldn't find the main file");
                            continue;
                        }
                        let mut p: Option<PathBuf> = None;
                        if xmp_file_id > 0 {
                            if let Some(p2) = self.get_fs_file(xmp_file_id) {
                                p = Some(PathBuf::from(p2));
                            }
                            dbg_assert!(!p.is_none(), "couldn't find the xmp file path");
                        }
                        if p.is_none() {
                            p = Some(spath.with_extension("xmp"));
                            dbg_assert!(
                                *p.as_ref().unwrap() != spath,
                                "path must have been changed"
                            );
                        }
                        let p = p.unwrap();
                        if p.exists() {
                            dbg_out!("{:?} already exist", p);
                        }
                        let mut xmppacket = fwk::XmpMeta::new();
                        xmppacket.unserialize(&xmp_buffer);
                        if let Ok(mut f) = File::create(p.clone()) {
                            let sidecar = xmppacket.serialize();
                            if f.write(sidecar.as_bytes()).is_ok() && (xmp_file_id <= 0) {
                                let xmp_file_id = self.add_fs_file(p.to_string_lossy().as_ref());
                                dbg_assert!(xmp_file_id > 0, "couldn't add xmp_file");
                                // XXX handle error
                                let res = self.add_xmp_sidecar_to_bundle(id, xmp_file_id);
                                dbg_assert!(res, "addSidecarFileToBundle failed");
                            }
                        }
                    }
                    return true;
                }
            }
        }
        false
    }

    pub fn process_xmp_update_queue(&self, write_xmp: bool) -> bool {
        if let Some(ids) = self.get_xmp_ids_in_queue() {
            for id in ids {
                self.rewrite_xmp_for_id(id, write_xmp);
            }

            return true;
        }

        false
    }
}

#[cfg(test)]
mod test {
    use std::path::Path;
    use std::path::PathBuf;
    use std::fs;
    use engine::db::filebundle::FileBundle;

    struct AutoDelete {
        path: PathBuf,
    }
    impl AutoDelete {
        pub fn new(path: &Path) -> AutoDelete {
            AutoDelete {
                path: PathBuf::from(path),
            }
        }
    }
    impl Drop for AutoDelete {
        fn drop(&mut self) {
            fs::remove_file(&self.path).is_ok();
        }
    }

    #[test]
    fn library_works() {
        use super::{Library, Managed};

        let lib = Library::new(PathBuf::from("."), 0);
        let _autodelete = AutoDelete::new(lib.dbpath());

        assert!(lib.is_ok());
        assert!(lib.check_database_version() == super::DB_SCHEMA_VERSION);

        let folder_added = lib.add_folder("foo", Some(String::from("foo")));
        assert!(folder_added.is_some());
        let folder_added = folder_added.unwrap();
        assert!(folder_added.id() > 0);

        let f = lib.get_folder("foo");
        assert!(f.is_some());
        let f = f.unwrap();
        assert_eq!(folder_added.id(), f.id());

        let id = f.id();
        lib.add_folder_into("bar", Some(String::from("bar")), id);
        let f = lib.get_folder("bar");
        assert!(f.is_some());
        let f = f.unwrap();
        assert_eq!(f.parent(), id);

        let folders = lib.get_all_folders();
        assert!(folders.is_some());
        let folders = folders.unwrap();
        assert_eq!(folders.len(), 3);

        let file_id = lib.add_file(folder_added.id(), "foo/myfile", super::Managed::NO);
        assert!(file_id > 0);

        assert!(!lib.move_file_to_folder(file_id, 100));
        assert!(lib.move_file_to_folder(file_id, folder_added.id()));
        let count = lib.count_folder(folder_added.id());
        assert_eq!(count, 1);

        let fl = lib.get_folder_content(folder_added.id());
        assert!(fl.is_some());
        let fl = fl.unwrap();
        assert_eq!(fl.len(), count as usize);
        assert_eq!(fl[0].id(), file_id);

        let kwid1 = lib.make_keyword("foo");
        assert!(kwid1 > 0);
        let kwid2 = lib.make_keyword("bar");
        assert!(kwid2 > 0);

        // duplicate keyword
        let kwid3 = lib.make_keyword("foo");
        // should return kwid1 because it already exists.
        assert_eq!(kwid3, kwid1);

        assert!(lib.assign_keyword(kwid1, file_id));
        assert!(lib.assign_keyword(kwid2, file_id));

        let fl2 = lib.get_keyword_content(kwid1);
        assert!(fl2.is_some());
        let fl2 = fl2.unwrap();
        assert_eq!(fl2.len(), 1);
        assert_eq!(fl2[0].id(), file_id);

        let kl = lib.get_all_keywords();
        assert!(kl.is_some());
        let kl = kl.unwrap();
        assert_eq!(kl.len(), 2);

        // Testing bundles
        let mut bundle = FileBundle::new();
        assert!(bundle.add("img_0123.crw"));
        assert!(bundle.add("img_0123.jpg"));
        assert!(bundle.add("img_0123.thm"));
        assert!(bundle.add("img_0123.xmp"));

        let bundle_id = lib.add_bundle(folder_added.id(), &bundle, Managed::NO);
        assert!(bundle_id > 0);
    }
}
