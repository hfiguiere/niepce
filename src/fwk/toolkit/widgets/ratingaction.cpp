/*
 * niepce - fwk/toolkit/widgets/ratingaction.cpp
 *
 * Copyright (C) 2011 Hubert Figuiere
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

#include "fwk/base/debug.hpp"

#include "ratingaction.hpp"
#include "ratingmenuitem.hpp"

namespace fwk  {

  Glib::RefPtr<RatingAction> RatingAction::create (const Glib::ustring& name, 
						   int rating)
  {
    return Glib::RefPtr<RatingAction>(new RatingAction(name, rating));
  }

  Gtk::Widget* RatingAction::create_menu_item_vfunc()
  {
    DBG_OUT("created menu item");
    return new RatingMenuItem(m_rating);
  }

}
