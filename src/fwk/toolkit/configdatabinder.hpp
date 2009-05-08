/*
 * niepce - fwk/toolkit/configdatabinder.h
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


#ifndef _FRAMEWORK_CONFIGDATABINDER_H_
#define _FRAMEWORK_CONFIGDATABINDER_H_

#include <exception>
#include <string>

#include <boost/lexical_cast.hpp>

#include <glibmm/propertyproxy.h>

#include "fwk/utils/debug.hpp"
#include "fwk/utils/databinder.hpp"
#include "fwk/toolkit/configuration.hpp"

namespace fwk {

class ConfigDataBinderBase
	: public DataBinderBase
{
public:
	typedef Glib::PropertyProxy_Base property_t;

	ConfigDataBinderBase(const property_t & property,
						 Configuration & config, const std::string & key);
	
	virtual void on_changed(void) = 0;
protected:
	property_t        m_property;
	std::string       m_config_key;
	Configuration   & m_config;
	sigc::connection  m_conn;
};

template <class T>
class ConfigDataBinder
	: public ConfigDataBinderBase
{
public:
	typedef Glib::PropertyProxy<T> property_t;

	ConfigDataBinder(const property_t & property,
					 Configuration & config, const std::string & key)
		: ConfigDataBinderBase(property, config, key)
		{
			Glib::ustring value;
			value = m_config.getValue(m_config_key, "");
			if(!value.empty()) {
				try {
					static_cast<property_t&>(m_property).set_value(
						boost::lexical_cast<T>(value));
				}
				catch(const boost::bad_lexical_cast &)
				{
					ERR_OUT("exception converting %s", value.c_str());
				}
			}
		}

	virtual ~ConfigDataBinder()
		{
			try {
				m_config.setValue(m_config_key, 
								  boost::lexical_cast<std::string>(m_value));
			}
			catch(const boost::bad_lexical_cast &)
			{
				ERR_OUT("exception");
			}
		}


	virtual void on_changed(void)
		{
			try {
				m_value = static_cast<property_t&>(m_property).get_value();
			}
			catch(const std::exception &)
			{
				ERR_OUT("exception");
			}
		}
private:
	T m_value;
};


}


#endif
