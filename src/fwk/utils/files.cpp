/* -*- mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode:nil; -*- */
/*
 * niepce - fwk/utils/files.cpp
 *
 * Copyright (C) 2007-2018 Hubert Figuiere
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

#include <boost/algorithm/string.hpp>

#include <giomm/file.h>
#include <glibmm/miscutils.h>

#include "fwk/base/debug.hpp"
#include "files.hpp"
#include "pathutils.hpp"

#include "rust_bindings.hpp"

namespace fwk {

std::string make_tmp_dir(const std::string& base)
{
    GError *err = nullptr;
    char* tmp_dir = g_dir_make_tmp(base.c_str(), &err);
    if (!tmp_dir) {
        if (err) {
            ERR_OUT("g_dir_mak_tmp(%s) failed: %s", base.c_str(), err->message);
            g_error_free(err);
        }
        return "";
    }
    std::string tmp_dir_path = tmp_dir;
    g_free(tmp_dir);
    return tmp_dir_path;
}

bool filter_none(const Glib::RefPtr<Gio::FileInfo> & )
{
    return true;
}


bool filter_ext(const Glib::RefPtr<Gio::FileInfo> & file, const std::string & ext)
{
    std::string file_ext = fwk::path_extension(file->get_name());
    boost::to_lower(file_ext);
    if(file_ext == ext) {
        return false;
    }
    return true;
}

bool filter_xmp_out(const Glib::RefPtr<Gio::FileInfo> & file)
{
    static const std::string ext(".xmp");
    return filter_ext(file, ext);
}

bool filter_only_media(const Glib::RefPtr<Gio::FileInfo> & file)
{
    return ffi::fwk_file_is_media(file->gobj());
}


FileListPtr wrapFileList(ffi::FileList* list)
{
    return FileListPtr(list, ffi::fwk_file_list_delete);
}

}

