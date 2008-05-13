/*
 * niepce - ui/selectioncontroller.cpp
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

#include <boost/bind.hpp>

#include <gtkmm/iconview.h>

#include "utils/autoflag.h"
#include "utils/debug.h"
#include "selectioncontroller.h"

namespace ui {


SelectionController::SelectionController()
	: m_in_handler(false)
{
}


void SelectionController::add_selectable(IImageSelectable * selectable)
{ 
	m_selectables.push_back(selectable);
	selectable->image_list()->signal_selection_changed().connect(
		boost::bind(&SelectionController::selected, 
					this, selectable));
	selectable->image_list()->signal_item_activated().connect(
		boost::bind(&SelectionController::activated, this, _1, selectable));
}


void SelectionController::activated(const Gtk::TreeModel::Path & /*path*/,
									IImageSelectable * selectable)
{
	utils::AutoFlag f(m_in_handler);
	int selection = selectable->get_selected();
	DBG_OUT("item activated %d", selection);
	signal_activated(selection);
}

void SelectionController::selected(IImageSelectable * selectable)
{
	if(m_in_handler) {
		DBG_OUT("%lx already in handler", this);
		return;
	}

	utils::AutoFlag f(m_in_handler);

	int selection = selectable->get_selected();
	std::vector<IImageSelectable *>::iterator iter;
	for(iter = m_selectables.begin(); iter != m_selectables.end(); iter++) {
		if(*iter != selectable) {
			(*iter)->select_image(selection);
		}
	}
	signal_selected(selection);
}

}
