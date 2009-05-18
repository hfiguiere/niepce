/*
 * niepce - niepce/ui/librarymainviewcontroller.cpp
 *
 * Copyright (C) 2007-2009 Hubert Figuiere
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


#include <gtk/gtk.h>

#include <glibmm/i18n.h>
#include <glibmm/ustring.h>

#include <gtkmm/celllayout.h>
#include <gtkmm/cellrenderer.h>

#include "fwk/base/debug.hpp"
#include "niepce/notifications.hpp"
#include "engine/db/library.hpp"
#include "engine/db/libfile.hpp"
#include "fwk/toolkit/application.hpp"
#include "librarymainviewcontroller.hpp"
#include "niepcewindow.hpp"
#include "metadatapanecontroller.hpp"

namespace ui {




Gtk::Widget * LibraryMainViewController::buildWidget()
{
    m_gridview = GridViewModule::Ptr(new GridViewModule(m_getclient, m_model));
    add_library_module(m_gridview, _("Library"));


    m_darkroom = darkroom::DarkroomModule::Ptr(
        new darkroom::DarkroomModule(m_actionGroup, m_getclient));
    add_library_module(m_darkroom, _("Darkroom"));

    // TODO PrintModuleController
    // add_library_module(, _("Print"));
    return &m_mainview;
}


void LibraryMainViewController::add_library_module(const ILibraryModule::Ptr & module,
                                                   const std::string & label)
{
    add(module);
    m_mainview.append_page(*module->widget(), label);
}

void LibraryMainViewController::on_ready()
{
}


void LibraryMainViewController::on_selected(int id)
{
    DBG_OUT("selected callback %d", id);
    if(id > 0) {
        m_getclient()->requestMetadata(id);
    }		
    else  {
        m_gridview->display_none();
    }
}

void LibraryMainViewController::on_image_activated(int id)
{
    DBG_OUT("on image activated %d", id);
    Gtk::TreeIter iter = m_model->get_iter_from_id(id);
    if(iter) {
        eng::LibFile::Ptr libfile = (*iter)[m_model->columns().m_libfile];
        m_darkroom->set_image(libfile);
        m_mainview.activate_page(1);
    }
}


}

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:80
  End:
*/
