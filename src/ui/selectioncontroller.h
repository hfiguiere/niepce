/*
 * niepce - ui/selectioncontroller.h
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


#ifndef _UI_SELECTIONCONTROLLER_H__
#define _UI_SELECTIONCONTROLLER_H__

#include <gtk/gtkiconview.h>

#include <boost/signal.hpp>

#include "framework/controller.h"

namespace Gtk {
	class IconView;
	class Widget;
}

namespace ui {

/** interface for selectable image. Make the controller
 *  inherit/implement it.
 */
class IImageSelectable 
{
public:
	virtual ~IImageSelectable() {}
	virtual Gtk::IconView * image_list() = 0;
	virtual int get_selected() = 0;
	virtual void select_image(int id) = 0;
};


class SelectionController
	: public framework::Controller
{
public:
	typedef boost::shared_ptr<SelectionController> Ptr;
	SelectionController();

	void add_selectable(IImageSelectable *);

	void activated(const Gtk::TreeModel::Path & /*path*/,
				   IImageSelectable * selectable);
	void selected(IImageSelectable *);

	// the signal to call when selection is changed.
	boost::signal<void (int)> signal_selected;

	// signal for when the item is activated (ie double-click)
	boost::signal<void (int)> signal_activated;
protected:
	virtual Gtk::Widget * buildWidget()
		{ return NULL; }
private:
	bool m_in_handler;
	std::vector<IImageSelectable *> m_selectables;
};

}


#endif
