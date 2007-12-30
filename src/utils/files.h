/*
 * niepce - utils/files.h
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




#ifndef __UTILS_FILES_H__
#define __UTILS_FILES_H__

#include <list>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/filesystem/path.hpp>

namespace utils {

	bool filter_none(const boost::filesystem::path & file);
	bool filter_xmp_out(const boost::filesystem::path & file);

	class FileList 
		: private std::list< boost::filesystem::path >
	{
	public:
		typedef boost::shared_ptr< FileList > Ptr;

		typedef std::list< boost::filesystem::path >    _impltype_t;
		typedef _impltype_t::value_type       value_type;
		typedef _impltype_t::iterator         iterator;
		typedef _impltype_t::const_iterator   const_iterator;
		typedef _impltype_t::size_type        size_type;

		FileList( )
			{}
		FileList( const _impltype_t & );

		static Ptr getFilesFromDirectory(const value_type & dir,
										 boost::function<bool (const value_type &)> filter);

		const_iterator begin() const
			{ return _impltype_t::begin(); }
		const_iterator end() const
			{ return _impltype_t::end(); }
		size_type size() const
			{ return _impltype_t::size(); }
	};
}


#endif
