/*
 * niepce - ui/filmstripcontroller.h
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



#ifndef __UI_FILMSTRIPCONTROLLER_H_
#define __UI_FILMSTRIPCONTROLLER_H_

#include "fwk/toolkit/controller.hpp"
#include "fwk/toolkit/notificationcenter.hpp"
#include "ui/selectioncontroller.hpp"

namespace Gtk {
	class IconView;
}

namespace ui {


class FilmStripController
	: public fwk::Controller,
	  public IImageSelectable
{
public:
	typedef std::tr1::shared_ptr<FilmStripController> Ptr;
	typedef std::tr1::weak_ptr<FilmStripController> WeakPtr;

	FilmStripController(const Glib::RefPtr<ImageListStore> & store);

	virtual Gtk::IconView * image_list();
	virtual int get_selected();
	virtual void select_image(int id);

//	void on_tnail_notification(const framework::Notification::Ptr &);
//	void on_lib_notification(const framework::Notification::Ptr &);
	virtual Gtk::Widget * buildWidget(const Glib::RefPtr<Gtk::UIManager> &);

private:
	Gtk::IconView * m_thumbview;
	Glib::RefPtr<ImageListStore> m_store;
};


}

#endif
