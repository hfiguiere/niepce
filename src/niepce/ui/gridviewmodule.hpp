/*
 * niepce - ui/gridviewmodule.hpp
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



#ifndef __UI_GRIDVIEWMODULE_HPP_
#define __UI_GRIDVIEWMODULE_HPP_

#include <gtkmm/iconview.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/paned.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treestore.h>

#include "fwk/base/propertybag.hpp"
#include "engine/db/library.hpp"
#include "niepce/ui/ilibrarymodule.hpp"
#include "niepce/ui/imoduleshell.hpp"
#include "niepce/ui/metadatapanecontroller.hpp"
#include "niepce/ui/selectioncontroller.hpp"

namespace fwk {
class Dock;
}

namespace libraryclient {
class UIDataProvider;
}

namespace ui {

class ModuleShell;

class GridViewModule
    : public ILibraryModule
    , public IImageSelectable
{
public:
  typedef std::tr1::shared_ptr<GridViewModule> Ptr;

  GridViewModule(const IModuleShell & shell,
                 const Glib::RefPtr<ImageListStore> & store);


  void on_lib_notification(const eng::LibNotification &);
  void display_none();

  /* ILibraryModule */
  virtual void dispatch_action(const std::string & action_name);
  virtual void set_active(bool active);

  /* IImageSelectable */
  virtual Gtk::IconView * image_list();
  virtual eng::library_id_t get_selected();
  virtual void select_image(eng::library_id_t id);

protected:
  virtual Gtk::Widget * buildWidget(const Glib::RefPtr<Gtk::UIManager> & manager);


private:
  void on_metadata_changed(const fwk::PropertyBag &);
  void on_rating_changed(int id, int rating);

  const IModuleShell &               m_shell;
  Glib::RefPtr<ImageListStore> m_model;
  libraryclient::UIDataProvider *m_uidataprovider;

  // library split view
  Gtk::IconView                m_librarylistview;
  Gtk::ScrolledWindow          m_scrollview;
  MetaDataPaneController::Ptr  m_metapanecontroller;
  Gtk::HPaned                  m_lib_splitview;
  fwk::Dock                   *m_dock;
};


}
#endif
