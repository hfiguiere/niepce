/*
 * niepce - niepce/stock.cpp
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

#include <glibmm/i18n.h>
#include <gdkmm/pixbuf.h>
#include <gtkmm/iconfactory.h>
#include <gtkmm/stock.h>
#include <gtkmm/stockitem.h>

#include "fwk/base/debug.hpp"
#include "stock.hpp"

#ifndef DATADIR
#error DATADIR is not defined
#endif

namespace niepce {
namespace Stock {

const Gtk::StockID ROTATE_LEFT = Gtk::StockID("rotate-left");
const Gtk::StockID ROTATE_RIGHT = Gtk::StockID("rotate-right");


void registerStockItems()
{
    Glib::RefPtr<Gtk::IconFactory> icon_factory = Gtk::IconFactory::create();
    icon_factory->add_default();

    try {
        Gtk::Stock::add(Gtk::StockItem(ROTATE_LEFT, _("Rotate L_eft"), Gdk::ModifierType(0), '['));
        icon_factory->add(ROTATE_LEFT, Gtk::IconSet::create(
                              Gdk::Pixbuf::create_from_file(DATADIR"/niepce/pixmaps/niepce-rotate-left.png")));
        Gtk::Stock::add(Gtk::StockItem(ROTATE_RIGHT, _("Rotate R_ight"), Gdk::ModifierType(0), ']'));
        icon_factory->add(ROTATE_RIGHT, Gtk::IconSet::create(
                              Gdk::Pixbuf::create_from_file(DATADIR"/niepce/pixmaps/niepce-rotate-right.png")));
    }
    catch(const Glib::Exception &e)
    {
        ERR_OUT("Exception %s", e.what().c_str());
    }
}


}
}

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
