/*
 * niepce - ui/librarymainviewcontroller.cpp
 *
 * Copyright (C) 2007 Hubert Figuiere
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

#include <gtkimageview/gtkimageview.h>
#include <gtkimageview/gtkimagescrollwin.h>

#include "librarymainviewcontroller.h"

namespace ui {


	Gtk::Widget * LibraryMainViewController::buildWidget()
	{
		m_mainview.append_page(m_librarylistview, _("Library"));
		
		GtkWidget *iv = gtk_image_view_new();
		GtkWidget *ivs = gtk_image_scroll_win_new(GTK_IMAGE_VIEW(iv));
		m_imageview = Gtk::manage(Glib::wrap(ivs));
		m_mainview.append_page(*m_imageview, _("Darkroom"));
		return &m_mainview;
	}

}
