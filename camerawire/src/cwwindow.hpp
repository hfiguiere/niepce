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

#include <gtkmm/action.h>
#include <gtkmm/actiongroup.h>
#include <gtkmm/box.h>

#include "fwk/toolkit/frame.hpp"

namespace cw {

class CwWindow
  : public fwk::Frame
{
public:
  CwWindow();

protected:
  virtual Gtk::Widget * buildWidget();

private:
  void init_ui();
  void init_actions();
  void on_action_import();
  void on_preferences();

  Gtk::VBox                      m_vbox;
  Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;
};

}

#endif

