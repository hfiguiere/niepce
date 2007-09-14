/*
 * niepce - framework/configuration.h
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


#ifndef _FRAMEWORK_CONFIGURATION_H_
#define _FRAMEWORK_CONFIGURATION_H_

#include <glibmm/ustring.h>
#include <glibmm/refptr.h>

#include "framework/gconf_proxy_header.h"


namespace framework {

	class Configuration
	{
	public:
		Configuration();
		~Configuration();

		bool hasKey(const Glib::ustring & key) const;
		const Glib::ustring getValue(const Glib::ustring & key,
																 const Glib::ustring & def) const;

		void setValue(const Glib::ustring & key, const Glib::ustring & value);

	private:
		Glib::RefPtr< Gnome::Conf::Client > m_gconf;
	};

}

#endif
