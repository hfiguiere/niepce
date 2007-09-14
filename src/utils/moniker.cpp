/*
 * niepce - utils/moniker.cpp
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

#include <iostream>
#include <string>
#include <boost/range.hpp>
#include <boost/algorithm/string/find.hpp>

#include "debug.h"
#include "moniker.h"

namespace utils {

	Moniker::Moniker(const std::string & s)
	{
		DBG_OUT("%s", s.c_str());
		try {
			boost::iterator_range<std::string::const_iterator> iter 
				= boost::find_first(s, ":");
			if(iter) {
				DBG_OUT("found : in %s", s.c_str());

				m_scheme.append(s.begin(), iter.begin());
				m_path.append(iter.end(), s.end());

				DBG_OUT("moniker scheme %s path %s", m_scheme.c_str(), 
								m_path.c_str());
			}
		}
		catch(...) {
			DBG_OUT("exception");
		}
	}

	Moniker::Moniker(const std::string & scheme, const std::string & path)
		: m_scheme(scheme),
			m_path(path)
	{
	}


	const char *Moniker::c_str() const
	{
		if(m_cachedvalue.empty()) {
			m_cachedvalue = m_scheme + ":" + m_path;
		}
		return m_cachedvalue.c_str();
	}


	std::ostream & operator<<(std::ostream & stream, const Moniker & m)
	{
		stream << "Moniker " << m.scheme() << " : " << m.path() << std::endl;
		return stream;
	}

}
