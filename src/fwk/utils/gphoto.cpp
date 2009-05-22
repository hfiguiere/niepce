/*
 * niepce - fwk/utils/gphoto.cpp
 *
 * Copyright (C) 2009 Hubert Figuiere
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

#include <string.h>

extern "C" {
#include <gphoto2-port-result.h>
}

#include "fwk/base/debug.hpp"
#include "gphoto.hpp"


namespace fwk {


#define GP_CHECK(x,op)   if (x < GP_OK) {         \
    DBG_OUT("%s failed with %d",#op,x); }


GpDeviceList::GpDeviceList()
  : m_abilities(NULL)
  , m_ports(NULL)
{
  reload();
}

GpDeviceList::~GpDeviceList()
{
  _gp_cleanup();
}


void GpDeviceList::_gp_cleanup()
{
  if (m_abilities) {
    ::gp_abilities_list_free(m_abilities);
    m_abilities = NULL;
  }
  if (m_ports) {
    ::gp_port_info_list_free(m_ports);
    m_ports = NULL;
  }
}

void GpDeviceList::reload()
{
  _gp_cleanup();

  int ret;
  ret = ::gp_port_info_list_new(&m_ports);
  GP_CHECK(ret, gp_port_info_list_new);
  ret = ::gp_port_info_list_load(m_ports);
  GP_CHECK(ret, gp_port_list_load);
	ret = ::gp_abilities_list_new(&m_abilities);
  GP_CHECK(ret, gp_abilities_list_new);
  ret = ::gp_abilities_list_load(m_abilities, NULL);
  GP_CHECK(ret, gp_abilities_list_load);
}


void GpDeviceList::detect()
{
  if((!m_ports) || (!m_abilities)) {
    reload();
  }
  ::CameraList *cameraList;
  int ret;

  ret = ::gp_list_new(&cameraList);
  GP_CHECK(ret, gp_list_new);
  ret = ::gp_abilities_list_detect(m_abilities, m_ports, cameraList, NULL);
  GP_CHECK(ret, gp_abilities_list_detect);
	
  int count = ::gp_list_count(cameraList);
  for (int i = 0; i < count; i++)	{
    const char * name;
    const char * value;
    ret = ::gp_list_get_name(cameraList, i, &name);
    GP_CHECK(ret, gp_list_get_name);
    ret = ::gp_list_get_value(cameraList, i, &value);
    GP_CHECK(ret, gp_list_get_value);
    if ((count > 1) && (strcmp(value, "usb:") == 0)) {
      continue;
    }
    DBG_OUT("found %s %s", name, value);
    push_back(GpDevice::Ptr(new GpDevice(name,value)));
  }
  ::gp_list_free(cameraList);
}


}
