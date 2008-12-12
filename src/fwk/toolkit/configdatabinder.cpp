/*
 * niepce - framework/configdatabinder.cpp
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

#include "configdatabinder.h"

namespace framework {


ConfigDataBinderBase::ConfigDataBinderBase(const property_t & property,
										   Configuration & config, 
										   const std::string & key)
	: utils::DataBinderBase(),
	  m_property(property),
	  m_config_key(key),
	  m_config(config)
{
	m_conn = m_property.signal_changed().connect(
		sigc::mem_fun(*this, &ConfigDataBinderBase::on_changed));
}


}

