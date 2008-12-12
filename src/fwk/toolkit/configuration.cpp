/*
 * niepce - framework/configuration.cpp
 *
 * Copyright (C) 2007-2008 Hubert Figuiere
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


#include <memory>

#include "gconf_proxy_header.h"

#include "fwk/utils/debug.h"
#include "configuration.h"


namespace framework {

	Configuration::Configuration(const Glib::ustring & root)
		: m_gconf(Gnome::Conf::Client::get_default_client()),
		  m_root(root)
	{
	}


	Configuration::~Configuration()
	{
	}


	bool Configuration::hasKey(const Glib::ustring & key) const
	{
		//
		bool found = true;

		try {
			m_gconf->get(m_root + "/" + key);
		}
		catch(Gnome::Conf::Error & err) {
			DBG_OUT("key %s not found", key.c_str());
			DBG_OUT("exception is %s", err.what().c_str());
			found = false;
		}

		return found;
	}


	const Glib::ustring Configuration::getValue(const Glib::ustring & key,
												const Glib::ustring & def) const
	{
		Glib::ustring value;
		try {
			value = m_gconf->get_string(m_root + "/" + key);
		}
		catch(Gnome::Conf::Error &err) {
			value = def;
			DBG_OUT("Exception raised: %s", err.what().c_str());
		}

		return value;
	}

	void Configuration::setValue(const Glib::ustring & key, 
								 const Glib::ustring & value)
	{
		try {
			m_gconf->set(m_root + "/" + key, value);
		}
		catch(Gnome::Conf::Error & err) {
			DBG_OUT("Exception raised: %s", err.what().c_str());
		}
	}

}
