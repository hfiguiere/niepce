/*
 * niepce - mgwindow.hpp
 *
 * Copyright (C) 2013 Hubert Figuiere
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


#ifndef __MG_WINDOW_HPP_
#define __MG_WINDOW_HPP_

#include <gtkmm/action.h>
#include <gtkmm/actiongroup.h>
#include <gtkmm/box.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/treeview.h>

#include "fwk/toolkit/frame.hpp"

namespace mg {

class MgWindow
  : public fwk::Frame
{
public:
  MgWindow();

protected:
  virtual Gtk::Widget * buildWidget(const Glib::RefPtr<Gtk::UIManager> & manager);

private:
  class CameraTreeRecord
    : public Gtk::TreeModelColumnRecord
  {
  public:
    CameraTreeRecord()
      {
        add(m_icon);
        add(m_label);
        add(m_persistent);
      }

    Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf> > m_icon;
    Gtk::TreeModelColumn<std::string>        m_label;
    Gtk::TreeModelColumn<bool>               m_persistent;
  };

  void init_ui(const Glib::RefPtr<Gtk::UIManager> & manager);
  void init_actions();
  void on_action_import();
  void on_preferences();

  void reload_camera_list();

  CameraTreeRecord               m_camera_tree_record;
  Glib::RefPtr<Gtk::ListStore>   m_camera_tree_model;
  Gtk::VBox                      m_vbox;
  Gtk::HBox                      m_hbox;
  Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;
};

}

#endif

