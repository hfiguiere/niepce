/*
 * niepce - fwk/toolkit/configuration.hpp
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


#ifndef _FRAMEWORK_CONFIGURATION_H_
#define _FRAMEWORK_CONFIGURATION_H_

#include <gconf/gconf-client.h>

#include <glibmm/ustring.h>
#include <glibmm/refptr.h>


namespace fwk {

class Configuration
{
public:
		Configuration(const Glib::ustring & root);
		~Configuration();

		bool hasKey(const Glib::ustring & key) const;
		const Glib::ustring getValue(const Glib::ustring & key,
                                 const Glib::ustring & def) const;

		void setValue(const Glib::ustring & key, const Glib::ustring & value);
private:
    GConfClient *                       m_gconf;
		Glib::ustring                       m_root;
};

}

#endif
/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:80
  End:
*/
