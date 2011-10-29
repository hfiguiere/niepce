/*
 * niepce - fwk/toolkit/widgets/ratingaction.hpp
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

#ifndef _FWK_TOOLKIT_RATINGACTION_HPP_
#define _FWK_TOOLKIT_RATINGACTION_HPP_

#include <gtkmm/action.h>

namespace fwk {

class RatingAction
  : public Gtk::Action
{
public:

  static Glib::RefPtr<RatingAction> create (const Glib::ustring& name, 
					    int rating);
protected:
  RatingAction(const Glib::ustring& name, int rating)
    : Gtk::Action(name)
    , m_rating(rating)
  {
  }

  virtual Gtk::Widget* create_menu_item_vfunc();
private:
  int m_rating;
};

}

#endif
