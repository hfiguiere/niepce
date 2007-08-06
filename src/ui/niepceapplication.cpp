/*
 * niepce - ui/niepceapplication.cpp
 *
 * Copyright (C) 2007 Hubert Figuiere
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  
 * 02110-1301, USA
 */

#include "niepceapplication.h"
#include "niepcewindow.h"

using framework::Frame;

namespace ui {

	framework::Application *NiepceApplication::create()
	{
		if (m_application == NULL) {
			m_application = new NiepceApplication();
		}
		return m_application;
	}


	Frame *NiepceApplication::makeMainFrame()
	{
		return new NiepceWindow;
	}


}
