/*
 * niepce - ui/librarymainviewcontroller.h
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


#ifndef __UI_LIBRARYMAINVIEWCONTROLLER_H__
#define __UI_LIBRARYMAINVIEWCONTROLLER_H__


#include <gtkmm/iconview.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treestore.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/paned.h>

#include "librarymainview.hpp"
#include "engine/db/libfile.hpp"
#include "libraryclient/libraryclient.hpp"
#include "fwk/toolkit/controller.hpp"
#include "fwk/toolkit/notification.hpp"
#include "metadatapanecontroller.hpp"
#include "selectioncontroller.hpp"
#include "modules/darkroom/darkroommodule.hpp"
#include "imageliststore.hpp"

namespace Gtk {
	class Widget;
}
namespace fwk {
class Dock;
}

namespace ui {

class LibraryMainViewController
		: public fwk::Controller,
		  public IImageSelectable
{
public:
		typedef std::tr1::shared_ptr<LibraryMainViewController> Ptr;
		typedef std::tr1::weak_ptr<LibraryMainViewController> WeakPtr;

		LibraryMainViewController(const Glib::RefPtr<Gtk::ActionGroup> & actions,
                              const Glib::RefPtr<ImageListStore> & store)
        : m_actionGroup(actions),
          m_model(store)
        {
        }

		void on_lib_notification(const fwk::Notification::Ptr &);

		/** called when somehing is selected by the shared selection */
		void on_selected(int id);
		void on_image_activated(int id);

		virtual Gtk::IconView * image_list();
		virtual int get_selected();
		virtual void select_image(int id);
protected:
		virtual Gtk::Widget * buildWidget();
		virtual void on_ready();
private:
		libraryclient::LibraryClient::Ptr getLibraryClient();
		Glib::RefPtr<Gtk::ActionGroup> m_actionGroup;

		// managed widgets...
		LibraryMainView              m_mainview;
		Gtk::IconView                m_librarylistview;
		Gtk::ScrolledWindow          m_scrollview;
		// library split view
		Gtk::HPaned                  m_lib_splitview;
    fwk::Dock                   *m_dock;
		MetaDataPaneController::Ptr  m_metapanecontroller;

    darkroom::DarkroomModule::Ptr m_darkroom;

		Glib::RefPtr<ImageListStore> m_model;
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
