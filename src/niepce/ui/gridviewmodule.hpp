/*
 * niepce - ui/gridviewmodule.hpp
 *
 * Copyright (C) 2009-2014 Hubert Figuiere
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
#include "fwk/toolkit/widgets/imagegridview.hpp"
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
  typedef std::shared_ptr<GridViewModule> Ptr;

  GridViewModule(const IModuleShell & shell,
                 const Glib::RefPtr<ImageListStore> & store);
  virtual ~GridViewModule();

  void on_lib_notification(const eng::LibNotification &);
  void display_none();

  /* ILibraryModule */
  virtual void dispatch_action(const std::string & action_name) override;
  virtual void set_active(bool active) override;
  virtual Glib::RefPtr<Gio::MenuModel> getMenu() override
    { return Glib::RefPtr<Gio::MenuModel>(); }

  /* IImageSelectable */
  virtual Gtk::IconView * image_list() override;
  virtual eng::library_id_t get_selected() override;
  virtual void select_image(eng::library_id_t id) override;

protected:
  virtual Gtk::Widget * buildWidget() override;


private:
  void on_metadata_changed(const fwk::PropertyBag &, const fwk::PropertyBag & old);
  void on_rating_changed(int id, int rating);
  bool on_librarylistview_click(GdkEventButton *e);

  const IModuleShell &               m_shell;
  Glib::RefPtr<ImageListStore> m_model;

  // library split view
  fwk::ImageGridView*          m_librarylistview;
  Gtk::ScrolledWindow          m_scrollview;
  MetaDataPaneController::Ptr  m_metapanecontroller;
  Gtk::Paned                   m_lib_splitview;
  fwk::Dock                   *m_dock;
};


}
#endif
