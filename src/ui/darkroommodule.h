/*
 * niepce - ui/darkroommodule.h
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




#ifndef _UI_DARKROOMMODULE_H__
#define _UI_DARKROOMMODULE_H__

#include <gtkmm/widget.h>
#include <gtkmm/paned.h>
#include <gtkmm/box.h>


#include "framework/controller.h"
#include "db/libfile.h"
	
namespace ui {

class DarkroomModule
	: public framework::Controller
{
public:
	typedef boost::shared_ptr<DarkroomModule> Ptr;

	void set_image(const db::LibFile::Ptr & file);

protected:
	virtual Gtk::Widget * buildWidget();

private:
	// darkroom split view
	Gtk::HPaned                  m_dr_splitview;
	Gtk::VBox                    m_vbox;
	GtkWidget*                   m_imageview;
	Gtk::Widget*                 m_imageviewscroll;
};


}

#endif
