/*
 * niepce - libraryclient/clientimpl.rs
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

use libc::c_char;
use std::collections::VecDeque;
use std::ffi::CStr;
use std::path::PathBuf;
use std::sync;
use std::sync::atomic;
use std::thread;

use fwk::base::propertyvalue::PropertyValue;
use engine::db::{Library, LibraryId};
use engine::db::library::Managed;
use engine::library::op::Op;
use engine::library::commands;
use root::fwk::FileList;
use root::eng::NiepceProperties as Np;


pub struct ClientImpl {
    terminate: sync::Arc<atomic::AtomicBool>,
    tasks: sync::Arc<(sync::Mutex<VecDeque<Op>>, sync::Condvar)>,
}

impl Drop for ClientImpl {
    fn drop(&mut self) {
        self.stop();
    }
}

impl ClientImpl {

    pub fn new(dir: PathBuf, notif_id: u64) -> ClientImpl {
        let tasks = sync::Arc::new((sync::Mutex::new(VecDeque::new()), sync::Condvar::new()));
        let mut terminate = sync::Arc::new(atomic::AtomicBool::new(false));
        let tasks2 = tasks.clone();
        let terminate2 = terminate.clone();

        /* let thread = */ thread::spawn(move || {
            let library = Library::new(dir, notif_id);
            Self::main(&mut terminate, tasks, &library);
        });


        ClientImpl {
            terminate: terminate2,
            tasks: tasks2,
        }
    }

    fn stop(&mut self) {
        self.terminate.store(true, atomic::Ordering::Relaxed);
        self.tasks.1.notify_one();
    }

    fn main(terminate: &mut sync::Arc<atomic::AtomicBool>,
            tasks: sync::Arc<(sync::Mutex<VecDeque<Op>>, sync::Condvar)>,
            library: &Library) {

        while !terminate.load(atomic::Ordering::Relaxed) {

            let elem: Option<Op>;
            {
                let mut queue = tasks.0.lock().unwrap();
                if !queue.is_empty() {
                    elem = queue.pop_front();
                } else {
                    elem = None;
                }
            }

            if let Some(op) = elem {
                op.execute(library);
            }

            let queue = tasks.0.lock().unwrap();
            if queue.is_empty() {
                if let Err(err) = tasks.1.wait(queue) {
                    err_out!("queue lock poisonned: {}", err);
                    break;
                }
            }
        }
    }

    pub fn schedule_op<F>(&mut self, f: F)
        where F: Fn(&Library) -> bool + Send + Sync + 'static {

        let op = Op::new(f);

        let mut queue = self.tasks.0.lock().unwrap();
        queue.push_back(op);
        self.tasks.1.notify_all();
    }

}

#[no_mangle]
pub extern "C" fn libraryclient_clientimpl_new(path: *const c_char, notif_id: u64) -> *mut ClientImpl {
    let dir = PathBuf::from(&*unsafe { CStr::from_ptr(path) }.to_string_lossy());
    Box::into_raw(Box::new(ClientImpl::new(dir, notif_id)))
}

#[no_mangle]
pub extern "C" fn libraryclient_clientimpl_delete(obj: *mut ClientImpl) {
    unsafe { Box::from_raw(obj); }
}

#[no_mangle]
pub extern "C" fn libraryclient_clientimpl_get_all_keywords(client: &mut ClientImpl) {
    client.schedule_op(move |lib| {
        commands::cmd_list_all_keywords(&lib)
    });
}

#[no_mangle]
pub extern "C" fn libraryclient_clientimpl_get_all_folders(client: &mut ClientImpl) {
    client.schedule_op(move |lib| {
        commands::cmd_list_all_folders(&lib)
    });
}

#[no_mangle]
pub extern "C" fn libraryclient_clientimpl_query_folder_content(client: &mut ClientImpl,
                                                                folder_id: LibraryId) {
    client.schedule_op(move |lib| {
        commands::cmd_query_folder_content(&lib, folder_id)
    });
}

#[no_mangle]
pub extern "C" fn libraryclient_clientimpl_count_folder(client: &mut ClientImpl,
                                                        folder_id: LibraryId) {
    client.schedule_op(move |lib| {
        commands::cmd_count_folder(&lib, folder_id)
    });
}

#[no_mangle]
pub extern "C" fn libraryclient_clientimpl_query_keyword_content(client: &mut ClientImpl,
                                                                 keyword_id: LibraryId) {
    client.schedule_op(move |lib| {
        commands::cmd_query_keyword_content(&lib, keyword_id)
    });
}

#[no_mangle]
pub extern "C" fn libraryclient_clientimpl_request_metadata(client: &mut ClientImpl,
                                                            file_id: LibraryId) {
    client.schedule_op(move |lib| {
        commands::cmd_request_metadata(&lib, file_id)
    });
}

#[no_mangle]
pub extern "C" fn libraryclient_clientimpl_set_metadata(client: &mut ClientImpl,
                                                        file_id: LibraryId,
                                                        meta: Np, value: &PropertyValue) {
    let value2 = value.clone();
    client.schedule_op(move |lib| {
        commands::cmd_set_metadata(&lib, file_id, meta, &value2)
    });
}

#[no_mangle]
pub extern "C" fn libraryclient_clientimpl_write_metadata(client: &mut ClientImpl,
                                                          file_id: LibraryId) {
    client.schedule_op(move |lib| {
        commands::cmd_write_metadata(&lib, file_id)
    });
}

#[no_mangle]
pub extern "C" fn libraryclient_clientimpl_move_file_to_folder(client: &mut ClientImpl,
                                                               file_id: LibraryId,
                                                               from: LibraryId,
                                                               to: LibraryId) {
    client.schedule_op(move |lib| {
        commands::cmd_move_file_to_folder(&lib, file_id, from, to)
    });
}

#[no_mangle]
pub extern "C" fn libraryclient_clientimpl_get_all_labels(client: &mut ClientImpl) {
    client.schedule_op(move |lib| {
        commands::cmd_list_all_labels(&lib)
    });
}

#[no_mangle]
pub extern "C" fn libraryclient_clientimpl_create_label(client: &mut ClientImpl,
                                                        s: *const c_char, c: *const c_char) {
    let name = unsafe { CStr::from_ptr(s) }.to_string_lossy();
    let colour = unsafe { CStr::from_ptr(c) }.to_string_lossy();
    client.schedule_op(move |lib| {
        commands::cmd_create_label(&lib, &name, &colour)
    });
}

#[no_mangle]
pub extern "C" fn libraryclient_clientimpl_delete_label(client: &mut ClientImpl,
                                                        label_id: LibraryId) {
    client.schedule_op(move |lib| {
        commands::cmd_delete_label(&lib, label_id)
    });
}

#[no_mangle]
pub extern "C" fn libraryclient_clientimpl_update_label(client: &mut ClientImpl,
                                                        label_id: LibraryId,
                                                        s: *const c_char, c: *const c_char) {
    let name = unsafe { CStr::from_ptr(s) }.to_string_lossy();
    let colour = unsafe { CStr::from_ptr(c) }.to_string_lossy();
    client.schedule_op(move |lib| {
        commands::cmd_update_label(&lib, label_id, &name, &colour)
    });
}

#[no_mangle]
pub extern "C" fn libraryclient_clientimpl_process_xmp_update_queue(client: &mut ClientImpl,
                                                                    write_xmp: bool) {
    client.schedule_op(move |lib| {
        commands::cmd_process_xmp_update_queue(&lib, write_xmp)
    });
}

#[no_mangle]
pub extern "C" fn libraryclient_clientimpl_import_file(client: &mut ClientImpl,
                                                       file_path: *const c_char,
                                                       manage: Managed) {
    let path = String::from(unsafe { CStr::from_ptr(file_path) }.to_string_lossy());
    client.schedule_op(move |lib| {
        commands::cmd_import_file(&lib, &path, manage)
    });
}

#[no_mangle]
pub extern "C" fn libraryclient_clientimpl_import_files(client: &mut ClientImpl,
                                                        dir: *const c_char, cfiles: &mut FileList,
                                                        manage: Managed) {
    let folder = unsafe { CStr::from_ptr(dir) }.to_string_lossy();
    let mut files: Vec<String> = vec!();
    {
        let len = unsafe { cfiles.size() };
        for i in 0..len {
            let f = unsafe { cfiles.at_cstr(i) };
            let cstr = unsafe { CStr::from_ptr(f) }.to_string_lossy();
            files.push(String::from(cstr));
        }
    }
    client.schedule_op(move |lib| {
        commands::cmd_import_files(&lib, &folder, &files, manage)
    });
}
