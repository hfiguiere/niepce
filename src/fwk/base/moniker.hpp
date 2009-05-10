/*
 * niepce - fwk/base/moniker.hpp
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


#ifndef __FWK_BASE_MONIKER_H__
#define __FWK_BASE_MONIKER_H__

#include <ostream>
#include <string>


namespace fwk {

	/** Moniker are URL like specifiers
			In the form of scheme:path.

			@todo implement nested monikers
	 */
	class Moniker
	{
	public:
		explicit Moniker(const std::string & s);
		Moniker(const std::string & scheme, const std::string & path);

		const std::string & scheme() const
			{ return m_scheme; }
		const std::string & path() const
			{ return m_path; }
		const char *c_str() const;
	private:
		std::string m_scheme;
		std::string m_path;

		mutable std::string m_cachedvalue;
	};

	std::ostream & operator<<(std::ostream & stream, const Moniker & m);

}


#endif
