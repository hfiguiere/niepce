/*
 * niepce - ui/moduleshell.hpp
 *
 * Copyright (C) 2007-2013 Hubert Figuiere
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

#include <vector>

#include "moduleshellwidget.hpp"
#include "libraryclient/libraryclient.hpp"
#include "fwk/toolkit/uicontroller.hpp"
#include "fwk/toolkit/notification.hpp"
#include "niepce/ui/gridviewmodule.hpp"
#include "modules/darkroom/darkroommodule.hpp"
#include "imageliststore.hpp"
#include "imoduleshell.hpp"

namespace Gtk {
class Widget;
}

namespace ui {

class ModuleShell
    : public fwk::UiController
    , public IModuleShell
{
public:
    typedef std::shared_ptr<ModuleShell> Ptr;
    typedef std::weak_ptr<ModuleShell> WeakPtr;
    
    ModuleShell(const libraryclient::LibraryClient::Ptr & libclient)
        : m_libraryclient(libclient)
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
    virtual const SelectionController::Ptr & get_selection_controller() const
        {
            return m_selection_controller;
        }
    virtual libraryclient::LibraryClient::Ptr getLibraryClient() const
        {
            return m_libraryclient;
        }

    /** called when something is selected by the shared selection */
    void on_selected(eng::library_id_t id);
    void on_image_activated(eng::library_id_t id);
    
    virtual Gtk::Widget * buildWidget(const Glib::RefPtr<Gtk::UIManager> & manager);

    void action_edit_delete();
protected:
    virtual void add_library_module(const ILibraryModule::Ptr & module,
                                    const std::string & label);
    virtual void on_ready();
    void on_module_deactivated(int idx);
    void on_module_activated(int idx);
private:
    libraryclient::LibraryClient::Ptr m_libraryclient;
    Glib::RefPtr<Gtk::ActionGroup> m_actionGroup;
    
    // managed widgets...
    ModuleShellWidget             m_shell;
    Glib::RefPtr<Gtk::UIManager>  m_ui_manager;
    
    ui::SelectionController::Ptr  m_selection_controller;
    std::vector<ILibraryModule::Ptr> m_modules;
    // these should be dynamic
    GridViewModule::Ptr           m_gridview;
    dr::DarkroomModule::Ptr       m_darkroom;
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
