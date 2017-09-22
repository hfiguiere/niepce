/* -*- mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode:nil; -*- */
/*
 * niepce - fwk/utils/files.hpp
 *
 * Copyright (C) 2007-2017 Hubert Figuiere
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

#ifndef __UTILS_FILES_H__
#define __UTILS_FILES_H__

#include <algorithm>
#include <vector>
#include <string>
#include <memory>

#include <functional>

#if !RUST_BINDGEN
#include <giomm/fileinfo.h>
#endif

namespace fwk {

#if !RUST_BINDGEN
/** wrapper around g_dir_make_tmp() */
std::string make_tmp_dir(const std::string& base);

bool filter_none(const Glib::RefPtr<Gio::FileInfo> & file);
bool filter_ext(const Glib::RefPtr<Gio::FileInfo> & file,
                const std::string & ext);
bool filter_xmp_out(const Glib::RefPtr<Gio::FileInfo> & file);
#endif

class FileList;
typedef std::shared_ptr<FileList> FileListPtr;

class FileList
	: private std::vector<std::string>
{
public:
    typedef FileListPtr Ptr;

    typedef std::vector<std::string>    _impltype_t;
    typedef _impltype_t::value_type       value_type;
    typedef _impltype_t::iterator         iterator;
    typedef _impltype_t::const_iterator   const_iterator;
    typedef _impltype_t::size_type        size_type;

    FileList()
        {}
    FileList(const _impltype_t&);

#if !RUST_BINDGEN
    static Ptr getFilesFromDirectory(const value_type& dir,
                                     std::function<bool (const Glib::RefPtr<Gio::FileInfo>&)> filter);
#endif

    value_type at(size_type index) const
        { return _impltype_t::at(index); }
    const value_type::value_type* at_cstr(size_type index) const;
    const_iterator begin() const
        { return _impltype_t::cbegin(); }
    const_iterator end() const
        { return _impltype_t::cend(); }
    const_iterator cbegin() const
        { return _impltype_t::cbegin(); }
    const_iterator cend() const
        { return _impltype_t::cend(); }
    size_type size() const;
    void sort();
    void push_back(const value_type & v)
        { _impltype_t::push_back(v); }
};

}

#endif
