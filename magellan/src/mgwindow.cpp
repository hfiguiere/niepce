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

#include <gtkmm/stock.h>
#include <gtkmm/window.h>

#include "fwk/base/debug.hpp"
#include "fwk/toolkit/application.hpp"
#include "fwk/toolkit/undo.hpp"
#include "mgwindow.hpp"


using fwk::Application;
using fwk::UndoHistory;

namespace mg {


MgWindow::MgWindow()
    : fwk::Frame("mg-window-frame")
{
}


Gtk::Widget * MgWindow::buildWidget(const Glib::RefPtr<Gtk::UIManager> & manager)
{
  if(m_widget) {
    return m_widget;
  }

  Gtk::Window & win(gtkWindow());
  m_widget = &win;

  Application::Ptr pApp = Application::app();

  init_actions();
  init_ui(manager);

  win.add(m_vbox);

  Gtk::Widget* pMenuBar = pApp->uiManager()->get_widget("/MenuBar");
  m_vbox.pack_start(*pMenuBar, Gtk::PACK_SHRINK);

  Glib::RefPtr<Gtk::Builder> bldr
	  = Gtk::Builder::create_from_file(GLADEDIR"mgwindow.ui",
					   "content_box");
  Gtk::Box* content;
  bldr->get_widget("content_box", content);
  m_vbox.pack_start(*content, true, true, 8);

  Gtk::Button* dload_btn;
  bldr->get_widget("download_btn", dload_btn);
  dload_btn->set_related_action(m_importAction);

  win.set_size_request(600, 400);
  win.show_all_children();
  return m_widget;
}


void MgWindow::init_actions()
{
    Glib::RefPtr<Gtk::Action> an_action;

    m_refActionGroup = Gtk::ActionGroup::create();

    m_refActionGroup->add(Gtk::Action::create("MenuFile", _("_File")));
    m_importAction = Gtk::Action::create("Import", _("_Import"));
    m_refActionGroup->add(m_importAction,
			  sigc::mem_fun(*this,
					&MgWindow::on_action_import));
    m_refActionGroup->add(Gtk::Action::create("Close", Gtk::Stock::CLOSE),
                          sigc::mem_fun(gtkWindow(),
                                        &Gtk::Window::hide));
    m_refActionGroup->add(Gtk::Action::create("Quit", Gtk::Stock::QUIT),
                          sigc::mem_fun(*Application::app(),
                                        &Application::quit));

    m_refActionGroup->add(Gtk::Action::create("MenuEdit", _("_Edit")));

    create_undo_action(m_refActionGroup);
    create_redo_action(m_refActionGroup);

    // FIXME: bind
    m_refActionGroup->add(Gtk::Action::create("Cut", Gtk::Stock::CUT));
    m_refActionGroup->add(Gtk::Action::create("Copy", Gtk::Stock::COPY));
    m_refActionGroup->add(Gtk::Action::create("Paste", Gtk::Stock::PASTE));
    m_refActionGroup->add(Gtk::Action::create("Delete", Gtk::Stock::DELETE));

    m_refActionGroup->add(Gtk::Action::create("Preferences",
                                              Gtk::Stock::PREFERENCES),
                          sigc::mem_fun(*this,
                                        &MgWindow::on_preferences));

    m_refActionGroup->add(Gtk::Action::create("MenuTools", _("_Tools")));
    m_refActionGroup->add(Gtk::Action::create("RedetectDevices",
                                              Gtk::Stock::REFRESH),
                          Gtk::AccelKey("F5"),
                          sigc::mem_fun(*this, &MgWindow::detect_devices));
    m_hide_tools_action = Gtk::ToggleAction::create("ToggleToolsVisible",
                                                    _("_Hide tools"));
    m_refActionGroup->add(m_hide_tools_action,
                          sigc::mem_fun(*this, &Frame::toggle_tools_visible));

    m_refActionGroup->add(Gtk::Action::create("MenuHelp", _("_Help")));
    m_refActionGroup->add(Gtk::Action::create("Help", Gtk::Stock::HELP));
    m_refActionGroup->add(Gtk::Action::create("About", Gtk::Stock::ABOUT),
                          sigc::mem_fun(*Application::app(),
                                        &Application::about));

    Application::app()->uiManager()->insert_action_group(m_refActionGroup);

    gtkWindow().add_accel_group(Application::app()
                                ->uiManager()->get_accel_group());
}


void MgWindow::init_ui(const Glib::RefPtr<Gtk::UIManager> & manager)
{
  Glib::ustring ui_info =
    "<ui>"
    "  <menubar name='MenuBar'>"
    "    <menu action='MenuFile'>"
    "      <menuitem action='Import'/>"
    "      <separator/>"
    "      <menuitem action='Close'/>"
    "      <menuitem action='Quit'/>"
    "    </menu>"
    "    <menu action='MenuEdit'>"
    "      <menuitem action='Undo'/>"
    "      <menuitem action='Redo'/>"
    "      <separator/>"
    "      <menuitem action='Cut'/>"
    "      <menuitem action='Copy'/>"
    "      <menuitem action='Paste'/>"
    "      <menuitem action='Delete'/>"
    "      <separator/>"
    "      <menuitem action='Preferences'/>"
    "    </menu>"
    "    <menu action='MenuTools'>"
    "      <menuitem action='RedetectDevices' />"
    "      <separator/>"
    "      <menuitem action='ToggleToolsVisible'/>"
    "      <separator/>"
    "    </menu>"
    "    <menu action='MenuHelp'>"
    "      <menuitem action='Help'/>"
    "      <menuitem action='About'/>"
    "    </menu>"
    "  </menubar>"
    "  <toolbar  name='ToolBar'>"
    "    <toolitem action='Import'/>"
    "    <toolitem action='Quit'/>"
    "  </toolbar>"
    "</ui>";
  manager->add_ui_from_string(ui_info);
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
