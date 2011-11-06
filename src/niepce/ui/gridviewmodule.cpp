/*
 * niepce - ui/gridviewmodule.hpp
 *
 * Copyright (C) 2009 Hubert Figuiere
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


#include <gtkmm/liststore.h>
#include <gtkmm/treestore.h>
#include <gtkmm/treeselection.h>

#include "fwk/base/debug.hpp"
#include "fwk/toolkit/application.hpp"
#include "fwk/toolkit/configdatabinder.hpp"
#include "fwk/toolkit/widgets/dock.hpp"
#include "gridviewmodule.hpp"
#include "moduleshell.hpp"
#include "librarycellrenderer.hpp"


namespace ui {


GridViewModule::GridViewModule(ModuleShell* shell,
                               const Glib::RefPtr<ImageListStore> & store)
  : m_shell(shell)
  , m_model(store)
  , m_uidataprovider(NULL)
{
    m_uidataprovider = m_shell->getLibraryClient()->getDataProvider();
    DBG_ASSERT(m_uidataprovider, "provider is NULL");
}

void 
GridViewModule::on_lib_notification(const eng::LibNotification &ln)
{
    switch(ln.type) {
    case eng::Library::NOTIFY_METADATA_QUERIED:
    {
        eng::LibMetadata::Ptr lm
            = boost::any_cast<eng::LibMetadata::Ptr>(ln.param);
        DBG_OUT("received metadata");
        m_metapanecontroller->display(lm->id(), lm.get());
        break;
    }
    case eng::Library::NOTIFY_METADATA_CHANGED:
    {
        DBG_OUT("metadata changed");
        std::tr1::array<int, 3> m = boost::any_cast<std::tr1::array<int, 3> >(ln.param);
        if(m[0] == m_metapanecontroller->displayed_file()) {
            // FIXME: actually just update the metadata
          m_shell->getLibraryClient()->requestMetadata(m[0]);
        }
        break;
    }
    default:
        break;
    }
}


void GridViewModule::display_none()
{
  m_metapanecontroller->display(0, NULL);
}


Gtk::Widget * GridViewModule::buildWidget(const Glib::RefPtr<Gtk::UIManager> & manager)
{
  if(m_widget) {
    return m_widget;
  }
  m_widget = &m_lib_splitview;
  m_librarylistview.set_model(m_model);
  m_librarylistview.set_selection_mode(Gtk::SELECTION_SINGLE);
  m_librarylistview.property_row_spacing() = 0;
  m_librarylistview.property_column_spacing() = 0;
  m_librarylistview.property_spacing() = 0;
  m_librarylistview.property_margin() = 0;

  // the main cell
  LibraryCellRenderer * libcell = Gtk::manage(new LibraryCellRenderer(m_uidataprovider));
  libcell->signal_rating_changed.connect(sigc::mem_fun(*this, &GridViewModule::on_rating_changed));

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
  m_metapanecontroller = MetaDataPaneController::Ptr(new MetaDataPaneController);
  add(m_metapanecontroller);
  m_lib_splitview.pack2(*m_dock);
  m_dock->vbox().pack_start(*m_metapanecontroller->buildWidget(manager));

  m_databinders.add_binder(new fwk::ConfigDataBinder<int>(
                             m_lib_splitview.property_position(),
                             fwk::Application::app()->config(),
                             "meta_pane_splitter"));
  return m_widget;
}


void GridViewModule::dispatch_action(const std::string & /*action_name*/)
{
}


Gtk::IconView * GridViewModule::image_list()
{ 
    return & m_librarylistview; 
}

int GridViewModule::get_selected()
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
            eng::LibFile::Ptr libfile = row[m_model->columns().m_libfile];
            if(libfile) {
                id = libfile->id();
            }
        }
    }
    DBG_OUT("get_selected %d", id);
    return id;
}

void GridViewModule::select_image(int id)
{
    DBG_OUT("library select %d", id);
    Gtk::TreePath path = m_model->get_path_from_id(id);
    if(path) {
      m_librarylistview.select_path(path);
    }
    else {
      m_librarylistview.unselect_all();
    }
}

void GridViewModule::on_rating_changed(int /*id*/, int rating)
{
    m_shell->get_selection_controller()->set_rating(rating);
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
