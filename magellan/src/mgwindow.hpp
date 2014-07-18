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

#include "fwk/toolkit/appframe.hpp"

namespace mg {

class MgWindow
  : public fwk::AppFrame
{
public:
  MgWindow();

protected:
  virtual Gtk::Widget * buildWidget();

private:

  void init_ui(const Glib::RefPtr<Gtk::UIManager> & manager);
  void init_actions();
  void on_action_import();
  void on_preferences();

  void detect_devices();

  Gtk::VBox                      m_vbox;
  Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;
	Glib::RefPtr<Gtk::Action>      m_importAction;
};

}

#endif

