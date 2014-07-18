/*
 * niepce - cwwindow.hpp
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


#ifndef __CW_WINDOW_HPP_
#define __CW_WINDOW_HPP_

#include <giomm/simpleactiongroup.h>
#include <gtkmm/box.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/treeview.h>

#include "fwk/utils/gphoto.hpp"
#include "fwk/toolkit/appframe.hpp"

namespace cw {

class CwWindow
  : public fwk::AppFrame
{
public:
  CwWindow();

protected:
  virtual Gtk::Widget * buildWidget();

private:
  class CameraTreeRecord
    : public Gtk::TreeModelColumnRecord
  {
  public:
    CameraTreeRecord()
      {
        add(m_icon);
        add(m_label);
        add(m_camera);
        add(m_persistent);
      }

    Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf> > m_icon;
    Gtk::TreeModelColumn<std::string>        m_label;
    Gtk::TreeModelColumn<fwk::GpDevice::Ptr> m_camera;
    Gtk::TreeModelColumn<bool>               m_persistent;
  };

  void init_ui();
  void init_actions();
  void on_action_import();

  void reload_camera_list();

  CameraTreeRecord               m_camera_tree_record;
  Glib::RefPtr<Gtk::ListStore>   m_camera_tree_model;
  Gtk::Box                       m_hbox;
  Glib::RefPtr<Gio::SimpleActionGroup> m_actionGroup;
};

}

#endif

