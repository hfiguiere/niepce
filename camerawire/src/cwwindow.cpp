/*
 * niepce - cwwindow.cpp
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


#include <glibmm/i18n.h>

#include <gtkmm/stock.h>
#include <gtkmm/window.h>

#include "fwk/toolkit/application.hpp"
#include "fwk/toolkit/undo.hpp"
#include "fwk/toolkit/gtkutils.hpp"
#include "cwwindow.hpp"


using fwk::Application;
using fwk::UndoHistory;

namespace cw {


CwWindow::CwWindow()
  : fwk::AppFrame("cw-window-frame")
  , m_hbox(Gtk::ORIENTATION_HORIZONTAL)
{
}


Gtk::Widget * CwWindow::buildWidget()
{
  if(m_widget) {
    return m_widget;
  }

  Gtk::Window & win(gtkWindow());
  m_widget = &win;

  auto pApp = Application::app();

  init_actions();

  win.add(m_hbox);

  m_camera_tree_model = Gtk::ListStore::create(m_camera_tree_record);
  auto treeview = manage(new Gtk::TreeView(m_camera_tree_model));
  auto column = manage(new Gtk::TreeViewColumn(_("Camera")));
  auto cell = manage(new Gtk::CellRendererToggle);
  column->pack_start(*cell, false);
  column->add_attribute(cell->property_active(),
                        m_camera_tree_record.m_persistent);
  column->pack_start(m_camera_tree_record.m_icon, false);
  column->pack_start(m_camera_tree_record.m_label);
  treeview->append_column(*column);
  m_hbox.pack_start(*treeview, false, true, 4);

  reload_camera_list();

  win.set_size_request(600, 400);
  win.show_all_children();
  return m_widget;
}


void CwWindow::init_actions()
{
    m_actionGroup = Gio::SimpleActionGroup::create();
    gtkWindow().insert_action_group("win", m_actionGroup);

    m_menu = Gio::Menu::create();
    auto submenu = Gio::Menu::create();
    m_menu->append_submenu(_("File"), submenu);

    fwk::add_action(m_actionGroup, "Import",
                    sigc::mem_fun(*this,
                                  &CwWindow::on_action_import),
                    submenu, _("_Import..."), "win", nullptr);
    fwk::add_action(m_actionGroup, "Close",
                    sigc::mem_fun(gtkWindow(),
                                  &Gtk::Window::hide),
                    submenu, _("Close"), "win", "<Primary>w");

    submenu = Gio::Menu::create();
    m_menu->append_submenu(_("Edit"), submenu);
    auto section = Gio::Menu::create();
    submenu->append_section(section);

    create_undo_action(m_actionGroup, section);
    create_redo_action(m_actionGroup, section);

    // FIXME: bind
    section = Gio::Menu::create();
    submenu->append_section(section);

    fwk::add_action(m_actionGroup,
                    "Cut",
                    Gio::ActionMap::ActivateSlot(), section,
                    _("Cut"), "win", "<control>x");
    fwk::add_action(m_actionGroup,
                    "Copy",
                    Gio::ActionMap::ActivateSlot(), section,
                    _("Copy"), "win", "<control>c");
    fwk::add_action(m_actionGroup,
                    "Paste",
                    Gio::ActionMap::ActivateSlot(), section,
                    _("Paste"), "win" "<control>v");

    submenu = Gio::Menu::create();
    m_menu->append_submenu(_("Tools"), submenu);

    fwk::add_action(m_actionGroup,
                    "ReloadCameras",
                    sigc::mem_fun(*this, &CwWindow::reload_camera_list),
                    submenu, _("Refresh"), "win", "F5");


    m_hide_tools_action
        = fwk::add_action(m_actionGroup, "ToggleToolsVisible",
                          sigc::mem_fun(*this, &Frame::toggle_tools_visible),
                          submenu, _("Hide tools"), "win",
                          nullptr);
}


void CwWindow::on_action_import()
{
}

void CwWindow::reload_camera_list()
{
  using fwk::GpDeviceList;

  m_camera_tree_model->clear();
  
  GpDeviceList::obj().detect();
  for(GpDeviceList::const_iterator iter = GpDeviceList::obj().begin();
      iter != GpDeviceList::obj().end(); ++iter) {

    Gtk::TreeIter treeiter = m_camera_tree_model->append();
    treeiter->set_value(m_camera_tree_record.m_label, (*iter)->get_model());
    treeiter->set_value(m_camera_tree_record.m_camera, *iter);
    treeiter->set_value(m_camera_tree_record.m_persistent, false);
  }
}


}
