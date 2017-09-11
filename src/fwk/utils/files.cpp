/* -*- mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode:nil; -*- */
/*
 * niepce - fwk/utils/files.cpp
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

#include <boost/algorithm/string.hpp>

#include <giomm/file.h>
#include <glibmm/miscutils.h>

#include "fwk/base/debug.hpp"
#include "files.hpp"
#include "pathutils.hpp"

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


FileList::FileList( const _impltype_t & v )
    : _impltype_t( v )
{
}

const FileList::value_type::value_type*
FileList::at_cstr(size_type index) const
{
    return _impltype_t::at(index).c_str();
}

FileList::size_type FileList::size() const
{
    return _impltype_t::size();
}

void FileList::sort()
{
    std::sort(_impltype_t::begin(), _impltype_t::end());
}

FileList::Ptr FileList::getFilesFromDirectory(const FileList::value_type& p, std::function<bool (const Glib::RefPtr<Gio::FileInfo>&)> filter)
{
//		if(!exists( p ) ) {
//			DBG_OUT( "directory %s do not exist", p.c_str() );
//			return Ptr();
//		}
    try {
        FileList::Ptr l(new FileList());

        Glib::RefPtr<Gio::File> dir = Gio::File::create_for_path(p);
        Glib::RefPtr<Gio::FileEnumerator> enumerator = dir->enumerate_children();

        for (Glib::RefPtr<Gio::FileInfo> itr = enumerator->next_file();
             itr ; itr = enumerator->next_file()) {
            Gio::FileType ftype = itr->get_file_type();
            if ((ftype == Gio::FILE_TYPE_REGULAR)  || (ftype == Gio::FILE_TYPE_SYMBOLIC_LINK)) {
                if (filter(itr)) {
                    std::string fullname =
                        Glib::build_filename(dir->get_path(), itr->get_name());
                    l->push_back(fullname);
                    DBG_OUT("found file %s", fullname.c_str());
                }
            }
        }
        enumerator->close();
        l->sort();
        return l;
    } catch(const Glib::Error & e) {
        ERR_OUT("Exception: %s", e.what().c_str());
    } catch(const std::exception & e) {
        ERR_OUT("Exception: %s", e.what());
    }

    return Ptr();
}


}

