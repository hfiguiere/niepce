/*
 * niepce - ui/niepcewindow.cpp
 *
 * Copyright (C) 2007-2008 Hubert Figuiere
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

#include <iostream>
#include <string>
#include <boost/bind.hpp>

#include <glibmm/i18n.h>
#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/stock.h>
#include <gtkmm/separator.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/combobox.h>

#include "niepce/notifications.h"
#include "utils/debug.h"
#include "utils/moniker.h"
#include "db/library.h"
#include "library/thumbnailnotification.h"
#include "libraryclient/libraryclient.h"
#include "framework/application.h"
#include "framework/configuration.h"
#include "framework/notificationcenter.h"
#include "framework/gdkutils.h"
#include "framework/configdatabinder.h"

#include "eog-thumb-nav.h"
#include "eog-thumb-view.h"
#include "niepcewindow.h"
#include "librarymainviewcontroller.h"

using libraryclient::LibraryClient;
using framework::Application;
using framework::Configuration;
using framework::NotificationCenter;

namespace ui {


	NiepceWindow::NiepceWindow()
		: framework::Frame("mainWindow-frame")
	{
	}


	NiepceWindow::~NiepceWindow()
	{
		Application::Ptr pApp = Application::app();
		pApp->uiManager()->remove_action_group(m_refActionGroup);
	}

 	Gtk::Widget * 
	NiepceWindow::buildWidget()
	{
		Gtk::Window & win(gtkWindow());

		Application::Ptr pApp = Application::app();

		init_actions();
		init_ui();

		m_lib_notifcenter.reset(new NotificationCenter());
		m_lib_notifcenter->subscribe(niepce::NOTIFICATION_LIB, 
									 boost::bind(&NiepceWindow::on_lib_notification, 
												 this, _1));
		m_lib_notifcenter->subscribe(niepce::NOTIFICATION_THUMBNAIL,
									 boost::bind(&NiepceWindow::on_tnail_notification, 
												 this, _1));

		Glib::ustring name("camera");
		set_icon_from_theme(name);		

		// main view
		m_mainviewctrl = LibraryMainViewController::Ptr(new LibraryMainViewController());
		m_lib_notifcenter->subscribe(niepce::NOTIFICATION_LIB,
									 boost::bind(&LibraryMainViewController::on_lib_notification, 
												 m_mainviewctrl, _1));
		m_lib_notifcenter->subscribe(niepce::NOTIFICATION_THUMBNAIL,
									 boost::bind(&LibraryMainViewController::on_tnail_notification, 
												 m_mainviewctrl, _1));
		add(m_mainviewctrl);
		// workspace treeview
		m_workspacectrl = WorkspaceController::Ptr( new WorkspaceController() );
		m_lib_notifcenter->subscribe(niepce::NOTIFICATION_LIB,
									 boost::bind(&WorkspaceController::on_lib_notification, 
												 m_workspacectrl, _1));
		m_lib_notifcenter->subscribe(niepce::NOTIFICATION_COUNT,
									 boost::bind(&WorkspaceController::on_count_notification,
												 m_workspacectrl, _1));
		add(m_workspacectrl);

		m_hbox.set_border_width(4);
		m_hbox.pack1(*(m_workspacectrl->widget()), Gtk::EXPAND);
		m_hbox.pack2(*(m_mainviewctrl->widget()), Gtk::EXPAND);
		m_databinders.add_binder(new framework::ConfigDataBinder<int>(m_hbox.property_position(),
																	  Application::app()->config(),
																	  "workspace_splitter"));

		win.add(m_vbox);

		Gtk::Widget* pMenuBar = pApp->uiManager()->get_widget("/MenuBar");
		m_vbox.pack_start(*pMenuBar, Gtk::PACK_SHRINK);
		m_vbox.pack_start(m_hbox);


		// ribbon FIXME Move to its own controller
		m_thumbview = eog_thumb_view_new();
		GtkWidget *thn = eog_thumb_nav_new(m_thumbview, EOG_THUMB_NAV_MODE_ONE_ROW, true);
		Gtk::Widget *w = Glib::wrap(thn);
		m_vbox.pack_start(*w, Gtk::PACK_SHRINK);

		// status bar
		m_vbox.pack_start(m_statusBar, Gtk::PACK_SHRINK);
		m_statusBar.push(Glib::ustring(_("Ready")));

		win.set_size_request(600, 400);
		win.show_all_children();
		on_open_library();
		return &win;
	}


	void NiepceWindow::init_actions()
	{
		m_refActionGroup = Gtk::ActionGroup::create();
		
		m_refActionGroup->add(Gtk::Action::create("MenuFile", _("_File")));
		m_refActionGroup->add(Gtk::Action::create("Import", _("_Import...")),
							  sigc::mem_fun(this, 
											&NiepceWindow::on_action_file_import));
		m_refActionGroup->add(Gtk::Action::create("Close", Gtk::Stock::CLOSE),
							  sigc::mem_fun(gtkWindow(), 
											&Gtk::Window::hide));			
		m_refActionGroup->add(Gtk::Action::create("Quit", Gtk::Stock::QUIT),
							  sigc::mem_fun(*Application::app(), 
											&Application::quit));	

		m_refActionGroup->add(Gtk::Action::create("MenuEdit", _("_Edit")));
		// TODO link to action
		m_refActionGroup->add(Gtk::Action::create("Preferences", 
												  Gtk::Stock::PREFERENCES),
							  sigc::mem_fun(this,
											&NiepceWindow::on_preferences));

		m_refActionGroup->add(Gtk::Action::create("MenuHelp", _("_Help")));
		m_refActionGroup->add(Gtk::Action::create("Help", Gtk::Stock::HELP));
		m_refActionGroup->add(Gtk::Action::create("About", Gtk::Stock::ABOUT),
							  sigc::mem_fun(*Application::app(),
											&Application::about));

		Application::app()->uiManager()->insert_action_group(m_refActionGroup);		
		
		gtkWindow().add_accel_group(Application::app()
									->uiManager()->get_accel_group());
	}
	
	void NiepceWindow::on_action_file_import()
	{
		Gtk::FileChooserDialog dialog(gtkWindow(), _("Import picture folder"),
									  Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);

		dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		dialog.add_button(_("Import"), Gtk::RESPONSE_OK);

		Configuration & cfg = Application::app()->config();
		std::string last_import_location;
		last_import_location = cfg.getValue("lastImportLocation", "");
		if(!last_import_location.empty()) {
			dialog.set_filename(last_import_location);
		}

		int result = dialog.run();
		Glib::ustring to_import;
		switch(result)
		{
		case Gtk::RESPONSE_OK:
			to_import = dialog.get_filename();
			cfg.setValue("lastImportLocation", to_import);
			
			DBG_OUT("%s", to_import.c_str());
			m_libClient->importFromDirectory(to_import, false);
			break;
		default:
			break;
		}
	}

	void NiepceWindow::on_lib_notification(const framework::Notification::Ptr &n)
	{
		DBG_ASSERT(n->type() == niepce::NOTIFICATION_LIB, "wrong notification type");
		if(n->type() == niepce::NOTIFICATION_LIB) {
			db::LibNotification ln = boost::any_cast<db::LibNotification>(n->data());
			switch(ln.type) {
			case db::Library::NOTIFY_FOLDER_CONTENT_QUERIED:
			case db::Library::NOTIFY_KEYWORD_CONTENT_QUERIED:
			{
				db::LibFile::ListPtr l 
					= boost::any_cast<db::LibFile::ListPtr>(ln.param);
				DBG_OUT("received folder content file # %d", l->size());

				Glib::RefPtr<EogListStore> store(new EogListStore( *l ));
				eog_thumb_view_set_model((EogThumbView*)m_thumbview, 
										 store);
				break;
			}
			default:
				break;
			}
		}
	}

	void NiepceWindow::on_tnail_notification(const framework::Notification::Ptr &n)
	{
		DBG_ASSERT(n->type() == niepce::NOTIFICATION_THUMBNAIL, "wrong notification type");
		if(n->type() == niepce::NOTIFICATION_THUMBNAIL)	{
			Glib::RefPtr<EogListStore> store 
				= eog_thumb_view_get_model((EogThumbView*)m_thumbview);
			library::ThumbnailNotification tn 
				= boost::any_cast<library::ThumbnailNotification>(n->data());
			Gtk::TreeRow row;
			bool found = store->find_by_id(tn.id, row);
			if(found) {
				// FIXME parametrize
				row[store->m_columns.m_thumbnail] = framework::gdkpixbuf_scale_to_fit(tn.pixmap, 100);
			}
			else {
				DBG_OUT("row %d not found", tn.id);
			}
		}
	}



	void NiepceWindow::on_action_file_quit()
	{
		Application::app()->quit();
	}


	void NiepceWindow::on_open_library()
	{
		Configuration & cfg = Application::app()->config();
		std::string libMoniker;
		int reopen = 0;
		try {
			reopen = boost::lexical_cast<int>(cfg.getValue("reopen_last_library", "0"));
		}
		catch(...)
		{
		}
		if(reopen) {
			libMoniker = cfg.getValue("lastOpenLibrary", "");
		}
		if(libMoniker.empty()) {
			Gtk::FileChooserDialog dialog(gtkWindow(), _("Create library"),
										  Gtk::FILE_CHOOSER_ACTION_CREATE_FOLDER);
			
			dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
			dialog.add_button(_("Create"), Gtk::RESPONSE_OK);

			int result = dialog.run();
			Glib::ustring libraryToCreate;
			switch(result)
			{
			case Gtk::RESPONSE_OK:
				libraryToCreate = dialog.get_filename();
				// pass it to the library
				libMoniker = "local:";
				libMoniker += libraryToCreate.c_str();
				cfg.setValue("lastOpenLibrary", libMoniker);
				DBG_OUT("created library %s", libMoniker.c_str());
				break;
			default:
				break;
			}
			
		}
		else {
			DBG_OUT("last library is %s", libMoniker.c_str());
		}
		open_library(libMoniker);
	}


	void NiepceWindow::preference_dialog_setup(const Glib::RefPtr<Gnome::Glade::Xml> & xml, Gtk::Dialog * dialog)
	{
		Gtk::ComboBox * theme_combo = NULL;
		Gtk::CheckButton * reopen_checkbutton = NULL;
		utils::DataBinderPool * binder_pool = new utils::DataBinderPool();

		dialog->signal_hide().connect(boost::bind(&utils::DataBinderPool::destroy, 
												  binder_pool));
		
		theme_combo = xml->get_widget("theme_combo", theme_combo);
		binder_pool->add_binder(new framework::ConfigDataBinder<int>(
									theme_combo->property_active(),
									framework::Application::app()->config(),
									"ui_theme_set"));
		
		reopen_checkbutton = xml->get_widget("reopen_checkbutton", reopen_checkbutton);
		binder_pool->add_binder(new framework::ConfigDataBinder<bool>(
									reopen_checkbutton->property_active(),
									framework::Application::app()->config(),
									"reopen_last_library"));
	}


	void NiepceWindow::on_preferences()
	{
		DBG_OUT("on_preferences");
		show_modal_dialog(GLADEDIR"preferences.glade", "preferences",
						  boost::bind(&NiepceWindow::preference_dialog_setup,
									  this, _1, _2));
		DBG_OUT("end on_preferences");
	}

	void NiepceWindow::init_ui()
	{
		Application::Ptr pApp = Application::app();
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
//			"      <menuitem action='Cut'/>"
//			"      <menuitem action='Copy'/>"
//			"      <menuitem action='Paste'/>"
//			"      <separator/>"
			"      <menuitem action='Preferences'/>"
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
		pApp->uiManager()->add_ui_from_string(ui_info);
	} 


	void NiepceWindow::open_library(const std::string & libMoniker)
	{
		m_libClient = LibraryClient::Ptr(new LibraryClient(utils::Moniker(libMoniker),
														   m_lib_notifcenter));
		set_title(libMoniker);
	}


	void NiepceWindow::set_title(const std::string & title)
	{
		Frame::set_title(std::string(_("Niepce Digital - ")) + title);
	}


}
