/*
 * niepce - niepce/ui/filmstripcontroller.h
 *
 * Copyright (C) 2008-2013 Hubert Figuiere
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

#include "fwk/toolkit/uicontroller.hpp"
#include "fwk/toolkit/notificationcenter.hpp"
#include "ui/selectioncontroller.hpp"

namespace Gtk {
	class IconView;
}

namespace ui {


class FilmStripController
	: public fwk::UiController,
	  public IImageSelectable
{
public:
	typedef std::shared_ptr<FilmStripController> Ptr;
	typedef std::weak_ptr<FilmStripController> WeakPtr;

	FilmStripController(const Glib::RefPtr<ImageListStore> & store);

	virtual Gtk::IconView * image_list();
	virtual eng::library_id_t get_selected();
	virtual void select_image(eng::library_id_t id);

	virtual Gtk::Widget * buildWidget();

private:
	Gtk::IconView * m_thumbview;
	Glib::RefPtr<ImageListStore> m_store;
};


}

#endif
