/*
 * niepce - ui/niepcewindow.cpp
 *
 * Copyright (C) 2007 Hubert Figuiere
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  
 * 02110-1301, USA
 */



#include <glibmm/i18n.h>

#include <gtkmm/window.h>
#include <gtkmm/treestore.h>
#include <gtkmm/box.h>
#include <gtkmm/stock.h>
#include <gtkmm/separator.h>
#include <gtkmm/treeview.h>
#include <gtkmm/iconview.h>


#include "eog-thumb-nav.h"
#include "eog-thumb-view.h"
#include "framework/application.h"
#include "niepcewindow.h"

#include <gtkimageview/gtkimageview.h>
#include <gtkimageview/gtkimagescrollwin.h>


using framework::Application;

namespace ui {


	NiepceWindow::NiepceWindow()
		: framework::Frame()//GLADEDIR "mainwindow.glade", "mainwindow")
	{
//		Glib::RefPtr<Gnome::Glade::Xml> & g(glade());
		Gtk::Window & win(gtkWindow());

		Application *pApp = Application::app();

		init_actions();
		init_ui();

		win.add(m_vbox);

		Gtk::Widget* pMenuBar = pApp->uiManager()->get_widget("/MenuBar");
		m_vbox.pack_start(*pMenuBar, Gtk::PACK_SHRINK);
		
		m_vbox.pack_start(m_hbox);


		Glib::RefPtr<Gtk::TreeStore> treestore = Gtk::TreeStore::create(m_librarycolumns);
		m_librarytree = Gtk::manage(new Gtk::TreeView());
		m_librarytree->set_model(treestore);
		Gtk::TreeModel::iterator iter = treestore->append();
		Gtk::TreeModel::Row row = *iter;
		row[m_librarycolumns.m_col_text] = "foo";
		row[m_librarycolumns.m_col_number] = 42;

		m_hbox.pack1(*m_librarytree, Gtk::EXPAND);
	 

		// this should gets its own widget...
		m_mainbar.set_layout(Gtk::BUTTONBOX_START);
		Gtk::Button *button = Gtk::manage(new Gtk::Button(_("Library")));
		m_mainbar.pack_start(*button);
		button = Gtk::manage(new Gtk::Button(_("Darkroom")));
		m_mainbar.pack_start(*button);
		m_vbox2.pack_start(m_mainbar, Gtk::PACK_SHRINK);
		m_mainview = Gtk::manage(new Gtk::Notebook());
		m_mainview->set_show_tabs(false);
		m_vbox2.pack_start(*m_mainview);
		m_hbox.pack2(m_vbox2, Gtk::EXPAND);


		// TODO instanciate the iconview...
		m_imageview = Gtk::manage(new Gtk::IconView());
		m_mainview->append_page(*m_imageview, _("Library"));

		GtkWidget *iv = gtk_image_view_new();
		GtkWidget *ivs = gtk_image_scroll_win_new(GTK_IMAGE_VIEW(iv));
		Gtk::Widget *w = Glib::wrap(ivs);
		m_mainview->append_page(*w, _("Darkroom"));


		// ribbon
		GtkWidget *thv = eog_thumb_view_new();
		GtkWidget *thn = eog_thumb_nav_new(thv, EOG_THUMB_NAV_MODE_ONE_ROW, true);
		w = Glib::wrap(thn);
		m_vbox.pack_start(*w, Gtk::PACK_SHRINK);

		// status bar
		m_vbox.pack_start(m_statusBar, Gtk::PACK_SHRINK);
		m_statusBar.push(Glib::ustring("Ready"));

		win.set_size_request(600, 400);
		win.show_all_children();
	}

	NiepceWindow::~NiepceWindow()
	{
		Application *pApp = Application::app();
		pApp->uiManager()->remove_action_group(m_refActionGroup);		
	}

	void NiepceWindow::init_actions()
	{
		m_refActionGroup = Gtk::ActionGroup::create();
		
		m_refActionGroup->add(Gtk::Action::create("MenuFile", "_File") );
		m_refActionGroup->add(Gtk::Action::create("Import", "_Import..."),
													sigc::mem_fun(*this, 
																				&NiepceWindow::on_action_file_import));
		m_refActionGroup->add(Gtk::Action::create("Close", Gtk::Stock::CLOSE),
													sigc::mem_fun(gtkWindow(), 
																				&Gtk::Window::hide));			
		m_refActionGroup->add(Gtk::Action::create("Quit", Gtk::Stock::QUIT),
													sigc::mem_fun(Application::app(), 
																				&Application::quit));	
		Application::app()->uiManager()->insert_action_group(m_refActionGroup);		

		gtkWindow().add_accel_group(Application::app()->uiManager()->get_accel_group());
	}

	void NiepceWindow::on_action_file_import()
	{
	}


	void NiepceWindow::on_action_file_quit()
	{
		Application::app()->quit();
	}

	void NiepceWindow::init_ui()
	{
		Application *pApp = Application::app();
		Glib::ustring ui_info =
			"<ui>"
			"  <menubar name='MenuBar'>"
			"    <menu action='MenuFile'>"
			"      <menuitem action='Import'/>"
			"      <separator/>"
			"      <menuitem action='Close'/>"
			"      <menuitem action='Quit'/>"
			"    </menu>"
//			"    <menu action='MenuEdit'>"
//			"      <menuitem action='Cut'/>"
//			"      <menuitem action='Copy'/>"
//			"      <menuitem action='Paste'/>"
//			"    </menu>"
			"  </menubar>"
			"  <toolbar  name='ToolBar'>"
			"    <toolitem action='Import'/>"
			"    <toolitem action='Quit'/>"
			"  </toolbar>"
			"</ui>";
		pApp->uiManager()->add_ui_from_string(ui_info);
	} 
}
