/*
 * niepce - utils/exempi.h
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

#ifndef __UTILS_EXEMPI_H__
#define __UTILS_EXEMPI_H__

#include <string>

#include <boost/filesystem/path.hpp>
#include <boost/noncopyable.hpp>

#include <exempi/xmp.h>

namespace utils {

	class ExempiManager
		: public boost::noncopyable
	{
	public:
		struct ns_defs_t {
			const char *ns;
			const char *prefix;
		};
		/** construct with namespaces to initialize */
		ExempiManager(const ns_defs_t * namespaces = 0);
		~ExempiManager();
	};

	/** a high-level wrapper for xmp */
	class XmpMeta 
	{
	public:
		XmpMeta();
		XmpMeta(const boost::filesystem::path & for_file);
		~XmpMeta();

		/** serialize the XMP inline */
		std::string serialize_inline() const;
		/** serialize the XMP (for the sidecar) */
		std::string serialize() const;

		int32_t orientation() const;
		std::string label() const;
		int32_t rating() const;
	private:
		XmpPtr m_xmp;
	};
}

#endif
