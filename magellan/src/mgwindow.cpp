/*
 * niepce - mgwindow.cpp
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


#include <glibmm/i18n.h>

#include <gtkmm/window.h>

#include "fwk/base/debug.hpp"
#include "fwk/toolkit/application.hpp"
#include "fwk/toolkit/undo.hpp"
#include "fwk/toolkit/gtkutils.hpp"
#include "mgwindow.hpp"


using fwk::Application;
using fwk::UndoHistory;

namespace mg {


MgWindow::MgWindow()
    : fwk::AppFrame("mg-window-frame")
{
}


Gtk::Widget * MgWindow::buildWidget()
{
  if(m_widget) {
    return m_widget;
  }

  Gtk::Window & win(gtkWindow());
  m_widget = &win;

  Application::Ptr pApp = Application::app();

  init_actions();

  win.add(m_vbox);

  Glib::RefPtr<Gtk::Builder> bldr
	  = Gtk::Builder::create_from_file(GLADEDIR"mgwindow.ui",
					   "content_box");
  Gtk::Box* content;
  bldr->get_widget("content_box", content);
  m_vbox.pack_start(*content, true, true, 8);

  Gtk::Button* dload_btn;
  bldr->get_widget("download_btn", dload_btn);
  gtk_actionable_set_action_name(GTK_ACTIONABLE(dload_btn->gobj()), "win.Import");

  win.set_size_request(600, 400);
  win.show_all_children();
  return m_widget;
}


void MgWindow::init_actions()
{
    m_actionGroup = Gio::SimpleActionGroup::create();
    gtkWindow().insert_action_group("win", m_actionGroup);

    m_menu = Gio::Menu::create();
    auto submenu = Gio::Menu::create();
    m_menu->append_submenu(_("File"), submenu);

    fwk::add_action(m_actionGroup, "Import",
                    sigc::mem_fun(*this,
                                  &MgWindow::on_action_import),
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

    section = Gio::Menu::create();
    submenu->append_section(section);
    fwk::add_action(m_actionGroup,
                    "Preferences",
                    sigc::mem_fun(*this,
                                  &MgWindow::on_preferences),
                    section, _("Preferences"), "win", nullptr);

    submenu = Gio::Menu::create();
    m_menu->append_submenu(_("Tools"), submenu);

    fwk::add_action(m_actionGroup,
                    "RedetectDevices",
                    sigc::mem_fun(*this, &MgWindow::detect_devices),
                    submenu, _("Refresh"), "win", "F5");


    m_hide_tools_action
        = fwk::add_action(m_actionGroup, "ToggleToolsVisible",
                          sigc::mem_fun(*this, &Frame::toggle_tools_visible),
                          submenu, _("Hide tools"), "win",
                          nullptr);
}


void MgWindow::on_action_import()
{
  DBG_OUT("import");
}


void MgWindow::on_preferences()
{
  DBG_OUT("prefs");
}


void MgWindow::detect_devices()
{
  DBG_OUT("detect");
}


}
