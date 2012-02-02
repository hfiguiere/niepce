/*
 * niepce - ui/niepceapplication.cpp
 *
 * Copyright (C) 2007-2008 Hubert Figuiere
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

#include "config.h"

#include <glibmm/i18n.h>
#include <gtkmm/aboutdialog.h>

#include "fwk/utils/modulemanager.hpp"
#include "niepce/stock.hpp"
#include "niepceapplication.hpp"
#include "niepcewindow.hpp"

using fwk::Frame;
using fwk::Application;

namespace ui {

NiepceApplication::NiepceApplication()
    : Application(PACKAGE)
{
    niepce::Stock::registerStockItems();

    fwk::ModuleManager * modmgr = module_manager();
    DBG_ASSERT(modmgr != NULL, "module manager is NULL.");
    // path for modules is $PREFIX/share/niepce/modules/$VERSION
    modmgr->add_path(DATADIR"/"PACKAGE"/modules/"VERSION);
}

Application::Ptr NiepceApplication::create()
{
    if (!m_application) {
        m_application = Application::Ptr(new NiepceApplication());
    }
    return m_application;
}


Frame::Ptr NiepceApplication::makeMainFrame()
{
    return Frame::Ptr(new NiepceWindow);
}

void NiepceApplication::on_about()
{
    Gtk::AboutDialog dlg;
//    dlg.set_name("Niepce");
    dlg.set_program_name("Niepce Digital");
    dlg.set_version(VERSION);
    dlg.set_comments(Glib::ustring(_("A digital photo application.\n\n"
                                     "Build options: ")) + 
                     NIEPCE_BUILD_CONFIG);
    dlg.run();
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
