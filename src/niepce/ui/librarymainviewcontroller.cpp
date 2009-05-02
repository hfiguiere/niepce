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

#include "fwk/utils/debug.hpp"
#include "niepce/notifications.hpp"
#include "engine/db/library.hpp"
#include "fwk/toolkit/application.hpp"
#include "fwk/toolkit/widgets/dock.hpp"
#include "librarymainviewcontroller.hpp"
#include "niepcewindow.hpp"
#include "metadatapanecontroller.hpp"
#include "librarycellrenderer.hpp"

namespace ui {

void LibraryMainViewController::on_lib_notification(const fwk::Notification::Ptr &n)
{
    DBG_ASSERT(n->type() == niepce::NOTIFICATION_LIB, 
               "wrong notification type");
    if(n->type() == niepce::NOTIFICATION_LIB) {
        db::LibNotification ln = boost::any_cast<db::LibNotification>(n->data());
        switch(ln.type) {
        case db::Library::NOTIFY_METADATA_QUERIED:
        {
            db::LibMetadata::Ptr lm
                = boost::any_cast<db::LibMetadata::Ptr>(ln.param);
            DBG_OUT("received metadata");
            m_metapanecontroller->display(lm->id(), lm.get());
            break;
        }
        case db::Library::NOTIFY_METADATA_CHANGED:
        {
            DBG_OUT("metadata changed");
            std::tr1::array<int, 3> m = boost::any_cast<std::tr1::array<int, 3> >(ln.param);
            if(m[0] == m_metapanecontroller->displayed_file()) {
                // FIXME: actually just update the metadata
                getLibraryClient()->requestMetadata(m[0]);
            }
            break;
        }
        default:
            break;
        }
    }
}



Gtk::Widget * LibraryMainViewController::buildWidget()
{
    m_librarylistview.set_model(m_model);
    m_librarylistview.set_selection_mode(Gtk::SELECTION_SINGLE);
    m_librarylistview.property_row_spacing() = 0;
    m_librarylistview.property_column_spacing() = 0;
    m_librarylistview.property_spacing() = 0;

    // the main cell
    LibraryCellRenderer * libcell = Gtk::manage(new LibraryCellRenderer());

    GtkCellLayout *cl = GTK_CELL_LAYOUT(m_librarylistview.gobj());
    DBG_ASSERT(cl, "No cell layout");
    gtk_cell_layout_pack_start(cl, GTK_CELL_RENDERER(libcell->gobj()), 
                               FALSE);
    gtk_cell_layout_add_attribute(cl, 
                                  GTK_CELL_RENDERER(libcell->gobj()),
                                  "pixbuf", m_model->columns().m_pix.index());
    gtk_cell_layout_add_attribute(cl,
                                  GTK_CELL_RENDERER(libcell->gobj()),
                                  "libfile", m_model->columns().m_libfile.index());

    m_scrollview.add(m_librarylistview);
    m_scrollview.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    m_lib_splitview.pack1(m_scrollview);
    m_dock = new fwk::Dock();
    m_metapanecontroller = MetaDataPaneController::Ptr(new MetaDataPaneController(*m_dock));
    add(m_metapanecontroller);
    m_lib_splitview.pack2(m_dock->getWidget());
    (void)m_metapanecontroller->buildWidget();

    m_databinders.add_binder(new fwk::ConfigDataBinder<int>(
                                 m_lib_splitview.property_position(),
                                 fwk::Application::app()->config(),
                                 "meta_pane_splitter"));
		
    m_mainview.append_page(m_lib_splitview, _("Library"));


    m_darkroom = darkroom::DarkroomModule::Ptr(
        new darkroom::DarkroomModule(m_actionGroup, getLibraryClient()));
    add(m_darkroom);
    m_mainview.append_page(*m_darkroom->widget(), _("Darkroom"));

    // TODO PrintModuleController
    // m_mainview.append_page(, _("Print"));
    return &m_mainview;
}


void LibraryMainViewController::on_ready()
{
}


void LibraryMainViewController::on_selected(int id)
{
    DBG_OUT("selected callback %d", id);
    if(id > 0) {
        getLibraryClient()->requestMetadata(id);
    }		
    else  {
        m_metapanecontroller->display(0, NULL);
    }
}

void LibraryMainViewController::on_image_activated(int id)
{
    DBG_OUT("on image activated %d", id);
    Gtk::TreeIter iter = m_model->get_iter_from_id(id);
    if(iter) {
        db::LibFile::Ptr libfile = (*iter)[m_model->columns().m_libfile];
        m_darkroom->set_image(libfile);
        m_mainview.activate_page(1);
    }
}
	

libraryclient::LibraryClient::Ptr LibraryMainViewController::getLibraryClient()
{
    return	std::tr1::dynamic_pointer_cast<NiepceWindow>(m_parent.lock())->getLibraryClient();
}


Gtk::IconView * LibraryMainViewController::image_list()
{ 
    return & m_librarylistview; 
}

int LibraryMainViewController::get_selected()
{
    int id = 0;
    Glib::RefPtr<Gtk::TreeSelection> selection;

    Gtk::IconView::ArrayHandle_TreePaths paths = m_librarylistview.get_selected_items();
    if(!paths.empty()) {
        Gtk::TreePath path(*(paths.begin()));
        DBG_OUT("found path %s", path.to_string().c_str());
        Gtk::TreeRow row = *(m_model->get_iter(path));
        if(row) {
            DBG_OUT("found row");
            db::LibFile::Ptr libfile = row[m_model->columns().m_libfile];
            if(libfile) {
                id = libfile->id();
            }
        }
    }
    DBG_OUT("get_selected %d", id);
    return id;
}

void LibraryMainViewController::select_image(int id)
{
    DBG_OUT("library select %d", id);
    Gtk::TreePath path = m_model->get_path_from_id(id);
    m_librarylistview.select_path(path);
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
