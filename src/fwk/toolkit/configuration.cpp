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

#include <gconf/gconf-client.h>

#include "fwk/utils/debug.hpp"
#include "configuration.hpp"


namespace fwk {

Configuration::Configuration(const Glib::ustring & root)
		: m_gconf(gconf_client_get_default()),
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

    GConfValue * v = gconf_client_get(m_gconf,
                                      Glib::ustring(m_root + "/" + key).c_str(),
                                      NULL);
    found = (v != NULL);
    if(v) {
        gconf_value_free(v);
    }
    else {
        DBG_OUT("key %s not found", key.c_str());
    }

		return found;
}


const Glib::ustring Configuration::getValue(const Glib::ustring & key,
                                            const Glib::ustring & def) const
{
		Glib::ustring value;
    GError *err = NULL;
    gchar* v = gconf_client_get_string(m_gconf,
                                       Glib::ustring(m_root + "/" + key).c_str(),
                                       &err);

    if(!v) {
        value = def;
        if(err) {
            DBG_OUT("Exception raised: %s", err->message);
            g_error_free(err);
        }
		}
    else {
        value = v;
        g_free(v);
    }

		return value;
}

void Configuration::setValue(const Glib::ustring & key, 
                             const Glib::ustring & value)
{
    GError *err = NULL;
    gboolean ret = gconf_client_set_string(m_gconf, 
                                           Glib::ustring(m_root + "/" + key).c_str(), 
                                           value.c_str(), &err);
        
    if(ret && err) {
        DBG_OUT("Exception raised: %s", err->message);
        g_error_free(err);
		}
}

}

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:80
  End:
*/
