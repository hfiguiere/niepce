/*
 * niepce - ui/darkroommodule.cpp
 *
 * Copyright (C) 2008 Hubert Figuiere
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

#include <gtkimageview/gtkimageview.h>
#include <gtkimageview/gtkimagescrollwin.h>

#include <gdkmm/pixbuf.h>
#include <gtkmm/toolbar.h>
#include <gtkmm/stock.h>

#include "utils/debug.h"
#include "framework/imageloader.h"
#include "darkroommodule.h"

namespace ui {


void DarkroomModule::set_image(const db::LibFile::Ptr & file)
{
	try {
		framework::ImageLoader loader(file->path());

		Glib::RefPtr<Gdk::Pixbuf> pixbuf = loader.get_pixbuf();

		gtk_image_view_set_pixbuf(GTK_IMAGE_VIEW(m_imageview),
								  pixbuf->gobj(), true);
	}
	catch(const Glib::Error & e)
	{
		ERR_OUT("exception loading image: %s", e.what().c_str());
	}
}


Gtk::Widget * DarkroomModule::buildWidget()
{
    m_imageview = gtk_image_view_new();
	GtkWidget *ivs = gtk_image_scroll_win_new(GTK_IMAGE_VIEW(m_imageview));
	m_imageviewscroll = Gtk::manage(Glib::wrap(ivs));
	m_vbox.pack_start(*m_imageviewscroll, Gtk::PACK_EXPAND_WIDGET);

	// build the toolbar.
	Gtk::Toolbar * toolbar = Gtk::manage(new Gtk::Toolbar);
	// FIXME: bind the buttons
	Gtk::ToolButton *btn = Gtk::manage(new Gtk::ToolButton(Gtk::Stock::GO_BACK));
	toolbar->append(*btn);
	btn = Gtk::manage(new Gtk::ToolButton(Gtk::Stock::GO_FORWARD));
	toolbar->append(*btn);

	m_vbox.pack_start(*toolbar, Gtk::PACK_SHRINK);
	m_dr_splitview.pack1(m_vbox, Gtk::EXPAND);

	m_widget = &m_dr_splitview;
	return m_widget;
}


}
