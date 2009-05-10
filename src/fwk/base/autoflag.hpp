/*
 * niepce - fwk/base/autoflag.hpp
 *
 * Copyright (C) 2008-2009 Hubert Figuiere
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


#ifndef _FWK_BASE_AUTOFLAG_HPP_
#define _FWK_BASE_AUTOFLAG_HPP_

namespace fwk {

class AutoFlag
{
public:
	AutoFlag(bool & f)
		: m_flag(f)
		{
			m_flag = true;
		}
	~AutoFlag()
		{
			m_flag = false;
		}

private:
	bool & m_flag;
};

}

#endif
