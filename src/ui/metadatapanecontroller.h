/*
 * niepce - ui/metadatapanecontroller.h
 *
 * Copyright (C) 2008 Hubert Figuiere
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

#ifndef __UI_METADATAPANECONTROLLER_H__
#define __UI_METADATAPANECONTROLLER_H__

#include <gtkmm/box.h>

#include "utils/exempi.h"
#include "framework/controller.h"
#include "framework/widgets/dock-item.h"

namespace xmp {
	struct MetaDataSectionFormat;
}
namespace framework {
	class MetaDataWidget;
    class Dock;
}

namespace ui {
	
	class MetaDataPaneController
		: public framework::Controller
	{
	public:
		typedef boost::shared_ptr<MetaDataPaneController> Ptr;
		MetaDataPaneController(framework::Dock &);
        ~MetaDataPaneController();
		virtual Gtk::Widget * buildWidget();
		void display(int file_id, const utils::XmpMeta * meta);
        int displayed_file() const 
            { return m_fileid; }
	private:
		std::vector<framework::MetaDataWidget *> m_widgets;

		static const xmp::MetaDataSectionFormat * get_format();

        int m_fileid;
        framework::DockItem *m_dockitem;
	};

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
#endif
