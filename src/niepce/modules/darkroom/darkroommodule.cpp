/*
 * niepce - ui/darkroommodule.cpp
 *
 * Copyright (C) 2008-2014 Hubert Figuiere
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

#include <gdkmm/pixbuf.h>
#include <gtkmm/toolbar.h>

#include "fwk/base/debug.hpp"
#include "fwk/toolkit/application.hpp"
#include "fwk/toolkit/configdatabinder.hpp"
#include "fwk/toolkit/widgets/dock.hpp"
#include "ncr/init.hpp"
#include "darkroommodule.hpp"

namespace dr {

DarkroomModule::DarkroomModule(const ui::IModuleShell & shell)
    : m_shell(shell)
    , m_dr_splitview(Gtk::ORIENTATION_HORIZONTAL)
    , m_vbox(Gtk::ORIENTATION_VERTICAL)
    , m_image(new ncr::Image)
    , m_active(false)
    , m_need_reload(true)
{
    m_shell.get_selection_controller()->signal_selected.connect(
        sigc::mem_fun(*this, &DarkroomModule::on_selected));
}

void DarkroomModule::reload_image()
{
    if(!m_need_reload) {
        return;
    }
    eng::LibFilePtr file = m_imagefile.lock();
    if(file) {
        // currently we treat RAW + JPEG as RAW.
        // TODO: have a way to actually choose the JPEG.
        auto file_type = engine_db_libfile_file_type(file.get());
        bool isRaw = (file_type == eng::LibFileType::RAW)
            || (file_type == eng::LibFileType::RAW_JPEG);
        std::string path = engine_db_libfile_path(file.get());
        m_image->reload(path, isRaw, engine_db_libfile_orientation(file.get()));
    }
    else {
        // reset
        Glib::RefPtr<Gdk::Pixbuf> p = Gdk::Pixbuf::create_from_file(
            DATADIR"/niepce/pixmaps/niepce-image-generic.png");
        m_image->reload(p);
    }
    m_need_reload = false;
}

void DarkroomModule::set_image(const eng::LibFilePtr & file)
{
    if(m_imagefile.expired() || (file != m_imagefile.lock())) {
        m_imagefile = eng::LibFileWeakPtr(file);
        m_need_reload = true;
    }
    else if(!static_cast<bool>(file)) {
        m_imagefile.reset();
        m_need_reload = true;
    }

    if(m_need_reload && m_active) {
        reload_image();
    }
}

void DarkroomModule::dispatch_action(const std::string & /*action_name*/)
{
}


void DarkroomModule::set_active(bool active)
{
    m_active = active;
    if(active) {
        // if activated, force the refresh of the image.
        reload_image();
    }
}


Gtk::Widget * DarkroomModule::buildWidget()
{
    if(m_widget) {
        return m_widget;
    }
    ncr::init();
    m_widget = &m_dr_splitview;
    m_imagecanvas = Gtk::manage(new ImageCanvas());
// TODO set a proper canvas size
//    m_canvas_scroll.add(*m_imagecanvas);
    m_vbox.pack_start(*m_imagecanvas, Gtk::PACK_EXPAND_WIDGET);

    m_imagecanvas->set_image(m_image);

    // build the toolbar.
    auto toolbar = Gtk::manage(new Gtk::Toolbar);

    Glib::RefPtr<Gio::Action> an_action;
    auto tool_item = new Gtk::ToolButton();
    gtk_actionable_set_action_name(GTK_ACTIONABLE(tool_item->gobj()),
                                   "shell.PrevImage");
    tool_item->set_icon_name("go-previous");
    toolbar->append(*manage(tool_item));

    tool_item = new Gtk::ToolButton();
    gtk_actionable_set_action_name(GTK_ACTIONABLE(tool_item->gobj()),
                                   "shell.NextImage");
    tool_item->set_icon_name("go-next");
    toolbar->append(*manage(tool_item));

    tool_item = new Gtk::ToolButton();
    gtk_actionable_set_action_name(GTK_ACTIONABLE(tool_item->gobj()),
                                   "shell.RotateLeft");
    tool_item->set_icon_name("object-rotate-left");
    toolbar->append(*manage(tool_item));

    tool_item = new Gtk::ToolButton();
    gtk_actionable_set_action_name(GTK_ACTIONABLE(tool_item->gobj()),
                                   "shell.RotateRight");
    tool_item->set_icon_name("object-rotate-right");
    toolbar->append(*manage(tool_item));

    m_vbox.pack_start(*toolbar, Gtk::PACK_SHRINK);
    m_dr_splitview.pack1(m_vbox, Gtk::EXPAND);
    m_dock = Gtk::manage(new fwk::Dock());
    m_dr_splitview.pack2(*m_dock, Gtk::SHRINK);

    m_databinders.add_binder(new fwk::ConfigDataBinder<int>(
                                 m_dr_splitview.property_position(),
                                 fwk::Application::app()->config(),
                                 "dr_toolbox_pane_splitter"));

    m_toolbox_ctrl = ToolboxController::Ptr(new ToolboxController);
    add(m_toolbox_ctrl);
    m_dock->vbox().pack_start(*m_toolbox_ctrl->buildWidget());

    return m_widget;
}

void DarkroomModule::on_selected(eng::library_id_t id)
{
    auto file = m_shell.get_selection_controller()->get_file(id);
    DBG_OUT("selection is %ld", id);
    set_image(file);
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
