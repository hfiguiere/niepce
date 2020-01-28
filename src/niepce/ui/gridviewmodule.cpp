/*
 * niepce - ui/gridviewmodule.cpp
 *
 * Copyright (C) 2009-2020 Hubert Figuière
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

#include <exempi/xmpconsts.h>

#include "rust_bindings.hpp"

#include "fwk/base/debug.hpp"
#include "fwk/toolkit/application.hpp"
#include "fwk/toolkit/configdatabinder.hpp"
#include "fwk/toolkit/widgets/dock.hpp"
#include "libraryclient/uidataprovider.hpp"
#include "gridviewmodule.hpp"
#include "moduleshell.hpp"

namespace ui {

GridViewModule::GridViewModule(const IModuleShell & shell,
                               const ImageListStorePtr& store)
  : m_shell(shell)
  , m_model(store)
  , m_librarylistview(nullptr)
  , m_lib_splitview(Gtk::ORIENTATION_HORIZONTAL)
  , m_dock(nullptr)
  , m_context_menu(nullptr)
{
}

GridViewModule::~GridViewModule()
{
    m_widget = nullptr;
}

void
GridViewModule::on_lib_notification(const eng::LibNotification &ln)
{
    switch (engine_library_notification_type(&ln)) {
    case eng::NotificationType::METADATA_QUERIED:
    {
        auto lm = engine_library_notification_get_libmetadata(&ln);
        DBG_OUT("received metadata");
        if (lm) {
            m_metapanecontroller->display(engine_libmetadata_get_id(lm), lm);
        } else {
            ERR_OUT("Invalid LibMetadata (nullptr)");
        }
        break;
    }
    case eng::NotificationType::METADATA_CHANGED:
    {
        DBG_OUT("metadata changed");
        auto id = engine_library_notification_get_id(&ln);
        if(id && id == m_metapanecontroller->displayed_file()) {
            // FIXME: actually just update the metadata
            ffi::libraryclient_request_metadata(m_shell.getLibraryClient()->client(), id);
        }
        break;
    }
    default:
        break;
    }
}

void GridViewModule::display_none()
{
    m_metapanecontroller->display(0, nullptr);
}

bool GridViewModule::get_colour_callback_c(int32_t label, ffi::RgbColour* out,
                                           const void* user_data)
{
    if (user_data == nullptr) {
        return false;
    }
    return static_cast<const GridViewModule*>(user_data)->get_colour_callback(label, out);
}

bool GridViewModule::get_colour_callback(int32_t label, ffi::RgbColour* out) const
{
    libraryclient::UIDataProviderWeakPtr ui_data_provider(m_shell.get_ui_data_provider());
    auto provider = ui_data_provider.lock();
    DBG_ASSERT(static_cast<bool>(provider), "couldn't lock UI provider");
    if (provider) {
        auto c = provider->colourForLabel(label);
        if (c.ok() && out) {
            *out = c.unwrap();
            return true;
        }
    }
    return false;
}

Gtk::Widget * GridViewModule::buildWidget()
{
  if(m_widget) {
    return m_widget;
  }
  m_widget = &m_lib_splitview;
  m_librarylistview = Gtk::manage(new fwk::ImageGridView(m_model->gobjmm()));
  m_librarylistview->set_selection_mode(Gtk::SELECTION_SINGLE);
  m_librarylistview->property_row_spacing() = 0;
  m_librarylistview->property_column_spacing() = 0;
  m_librarylistview->property_spacing() = 0;
  m_librarylistview->property_margin() = 0;

  auto shell_menu = m_shell.getMenu();
  m_context_menu = Gtk::manage(new Gtk::Menu(shell_menu));
  m_context_menu->attach_to_widget(*m_librarylistview);

  m_librarylistview->signal_button_press_event()
      .connect(sigc::mem_fun(*this, &GridViewModule::on_librarylistview_click));
  m_librarylistview->signal_popup_menu()
      .connect(sigc::mem_fun(*this, &GridViewModule::on_popup_menu));

  // the main cell
  Gtk::CellRenderer* libcell = manage(
      Glib::wrap(
          ffi::npc_library_cell_renderer_new(&get_colour_callback_c, this)));
  g_signal_connect(
      libcell->gobj(), "rating-changed", G_CALLBACK(GridViewModule::on_rating_changed), this);

  Glib::RefPtr<Gtk::CellArea> cell_area = m_librarylistview->property_cell_area();
  cell_area->pack_start(*libcell, FALSE);
  cell_area->add_attribute(*libcell, "pixbuf",
                           static_cast<gint>(ffi::ColIndex::Thumb));
  cell_area->add_attribute(*libcell, "libfile",
                           static_cast<gint>(ffi::ColIndex::File));
  cell_area->add_attribute(*libcell, "status",
                           static_cast<gint>(ffi::ColIndex::FileStatus));

  m_scrollview.add(*m_librarylistview);
  m_scrollview.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  m_lib_splitview.set_wide_handle(true);

  // build the toolbar
  auto box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));
  box->pack_start(m_scrollview);
  auto toolbar = ffi::image_toolbar_new();
  gtk_box_pack_end(box->gobj(), GTK_WIDGET(toolbar), false, false, 0);
  m_lib_splitview.pack1(*box);

  m_dock = new fwk::Dock();
  m_metapanecontroller = MetaDataPaneController::Ptr(new MetaDataPaneController);
  m_metapanecontroller->signal_metadata_changed.connect(
      sigc::mem_fun(*this, &GridViewModule::on_metadata_changed));
  add(m_metapanecontroller);
  m_lib_splitview.pack2(*m_dock);
  m_dock->vbox().pack_start(*m_metapanecontroller->buildWidget());

  m_databinders.add_binder(new fwk::ConfigDataBinder<int>(
                             m_lib_splitview.property_position(),
                             fwk::Application::app()->config(),
                             "meta_pane_splitter"));
  return m_widget;
}

void GridViewModule::dispatch_action(const std::string & /*action_name*/)
{
}

void GridViewModule::set_active(bool /*active*/)
{
}

Gtk::IconView * GridViewModule::image_list()
{
    return m_librarylistview;
}

eng::library_id_t GridViewModule::get_selected()
{
    eng::library_id_t id = 0;
    Glib::RefPtr<Gtk::TreeSelection> selection;

    std::vector<Gtk::TreePath> paths = m_librarylistview->get_selected_items();
    if(!paths.empty()) {
        Gtk::TreePath path(*(paths.begin()));
        DBG_OUT("found path %s", path.to_string().c_str());
        id = m_model->get_libfile_id_at_path(path);
    }
    DBG_OUT("get_selected %Ld", (long long)id);
    return id;
}

void GridViewModule::select_image(eng::library_id_t id)
{
    DBG_OUT("library select %Ld", (long long)id);
    Gtk::TreePath path = m_model->get_path_from_id(id);
    if(path) {
        m_librarylistview->scroll_to_path(path, false, 0, 0);
        m_librarylistview->select_path(path);
    }
    else {
        m_librarylistview->unselect_all();
    }
}

void GridViewModule::on_metadata_changed(const fwk::PropertyBagPtr & props,
                                         const fwk::PropertyBagPtr & old)
{
    // TODO this MUST be more generic
    DBG_OUT("on_metadata_changed()");
    m_shell.get_selection_controller()->set_properties(props, old);
}

void GridViewModule::on_rating_changed(GtkCellRenderer*, eng::library_id_t /*id*/,
                                       int32_t rating, gpointer user_data)
{
    auto self = static_cast<GridViewModule*>(user_data);
    self->m_shell.get_selection_controller()->set_rating(rating);
}

bool GridViewModule::on_popup_menu()
{
    if (m_context_menu && !m_librarylistview->get_selected_items().empty()) {
        m_context_menu->popup_at_widget(m_librarylistview, Gdk::GRAVITY_CENTER, Gdk::GRAVITY_NORTH, nullptr);
        return true;
    }
    return false;
}

bool GridViewModule::on_librarylistview_click(GdkEventButton *e)
{
    GdkEvent* event = (GdkEvent*)e;
    if (gdk_event_triggers_context_menu(event)
        && gdk_event_get_event_type(event) == GDK_BUTTON_PRESS
        && !m_librarylistview->get_selected_items().empty()) {
        m_context_menu->popup_at_pointer(event);
    }
    double x, y;
    int bx, by;
    bx = by = 0;
    x = e->x;
    y = e->y;
    Gtk::TreeModel::Path path;
    Gtk::CellRenderer * renderer = nullptr;
    DBG_OUT("click (%f, %f)", x, y);
    m_librarylistview->convert_widget_to_bin_window_coords(x, y, bx, by);
    if (m_librarylistview->get_item_at_pos(bx, by, path, renderer)){
        DBG_OUT("found an item");

        return true;
    }
    return false;
}

}
/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  c-basic-offset:4
  tab-width:4
  indent-tabs-mode:nil
  fill-column:80
  End:
*/
