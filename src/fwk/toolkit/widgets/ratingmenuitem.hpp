/*
 * niepce - fwk/toolkit/widgets/ratingmenuitem.hpp
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

#ifndef __FWK_TOOLKIT_RATINGMENUITEM__
#define __FWK_TOOLKIT_RATINGMENUITEM__

#include <gtkmm/menuitem.h>
#include "fwk/base/debug.hpp"
#include "fwk/toolkit/widgets/ratinglabel.hpp"

namespace fwk {

class RatingMenuItem
  : public Gtk::MenuItem
{
public:
  RatingMenuItem(int rating)
    : Gtk::MenuItem()
    , m_label(rating)
  {
    DBG_OUT("ctor");
    m_label.show();
    add(m_label);
  }

private:
  RatingLabel m_label;
};

}

#endif
