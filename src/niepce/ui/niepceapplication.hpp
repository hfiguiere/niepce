#ifndef _UI_NIEPCEAPPLICATION_H_
#define _UI_NIEPCEAPPLICATION_H_

/*
 * niepce - ui/niepceapplication.h
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


#include "fwk/toolkit/application.hpp"

namespace ui {

	class NiepceApplication
		: public framework::Application
	{
	public:
		static framework::Application::Ptr create();

		virtual framework::Frame::Ptr makeMainFrame();
	protected:
		NiepceApplication();
		virtual void on_about();
	};

}

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/

#endif
