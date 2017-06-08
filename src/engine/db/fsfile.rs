

use libc::c_char;
use std::ffi::CStr;
use std::ffi::CString;
use std::path::{ Path, PathBuf };

use super::LibraryId;

/// Describe a file on the file system
pub struct FsFile {
    id: LibraryId,
    path: PathBuf,
    pub cstr: CString,
}

impl FsFile {

    pub fn new(id: LibraryId, path: PathBuf) -> FsFile {
        FsFile {
            id: id, path: path,
            cstr: CString::new("").unwrap(),
        }
    }

    pub fn id(&self) -> LibraryId {
        self.id
    }

    pub fn path(&self) -> &Path {
        self.path.as_path()
    }
}

#[no_mangle]
pub extern fn engine_db_fsfile_new(id: i64, name: *const c_char) -> *mut FsFile {
    let path = PathBuf::from(&*unsafe { CStr::from_ptr(name) }.to_string_lossy());
    let lf = Box::new(FsFile::new(id, path));
    Box::into_raw(lf)
}

#[no_mangle]
pub extern fn engine_db_fsfile_delete(lf: *mut FsFile) {
    unsafe { Box::from_raw(lf) };
}

#[no_mangle]
pub extern fn engine_db_fsfile_path(this: &mut FsFile) -> *const c_char {
    this.cstr = CString::new(&*this.path().to_string_lossy()).unwrap();
    this.cstr.as_ptr()
}
