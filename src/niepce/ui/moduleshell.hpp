/*
 * niepce - ui/moduleshell.hpp
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


#ifndef __UI_MODULESHELL_HPP__
#define __UI_MODULESHELL_HPP__



#include "moduleshellwidget.hpp"
#include "libraryclient/libraryclient.hpp"
#include "fwk/toolkit/uicontroller.hpp"
#include "fwk/toolkit/notification.hpp"
#include "niepce/ui/gridviewmodule.hpp"
#include "modules/darkroom/darkroommodule.hpp"
#include "imageliststore.hpp"

namespace Gtk {
	class Widget;
}

namespace ui {

class ModuleShell
		: public fwk::UiController
{
public:
		typedef std::tr1::shared_ptr<ModuleShell> Ptr;
		typedef std::tr1::weak_ptr<ModuleShell> WeakPtr;

		ModuleShell(const sigc::slot<libraryclient::LibraryClient::Ptr> get_client)
        : m_getclient(get_client)
        , m_actionGroup(Gtk::ActionGroup::create("ModuleShell"))
        {
        }


    const GridViewModule::Ptr & get_gridview() const
        {
            return m_gridview;
        }
    const Glib::RefPtr<ImageListStore> & get_list_store() const
        { 
            return m_selection_controller->get_list_store(); 
        }
    const SelectionController::Ptr & get_selection_controller() const
        {
            return m_selection_controller;
        }
    libraryclient::LibraryClient::Ptr getLibraryClient() const
        {
            return m_getclient();
        }

		/** called when somehing is selected by the shared selection */
		void on_selected(int id);
		void on_image_activated(int id);

		virtual Gtk::Widget * buildWidget(const Glib::RefPtr<Gtk::UIManager> & manager);
protected:
    virtual void add_library_module(const ILibraryModule::Ptr & module,
                                    const std::string & label);
		virtual void on_ready();
private:
		sigc::slot<libraryclient::LibraryClient::Ptr> m_getclient;
		Glib::RefPtr<Gtk::ActionGroup> m_actionGroup;

		// managed widgets...
		ModuleShellWidget             m_shell;
    Glib::RefPtr<Gtk::UIManager>  m_ui_manager;

    ui::SelectionController::Ptr  m_selection_controller;
    GridViewModule::Ptr           m_gridview;
    darkroom::DarkroomModule::Ptr m_darkroom;
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