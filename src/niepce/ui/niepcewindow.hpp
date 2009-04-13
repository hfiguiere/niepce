/*
 * niepce - ui/niepcewindow.h
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


#ifndef _UI_NIEPCEWINDOW_H_
#define _UI_NIEPCEWINDOW_H_

#include <boost/scoped_ptr.hpp>

#include <gtkmm/treemodel.h>
#include <gtkmm/box.h>
#include <gtkmm/menubar.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/paned.h>

#include "fwk/toolkit/frame.hpp"
#include "fwk/toolkit/notificationcenter.hpp"
#include "fwk/toolkit/configdatabinder.hpp"
#include "engine/db/label.hpp"
#include "libraryclient/libraryclient.hpp"
#include "ui/librarymainviewcontroller.hpp"
#include "ui/workspacecontroller.h"
#include "ui/selectioncontroller.hpp"
#include "ui/filmstripcontroller.hpp"

namespace fwk {
	class NotificatioCenter;
}

namespace ui {

class NiepceWindow
    : public fwk::Frame
{
public:
    NiepceWindow();
    virtual ~NiepceWindow();


    virtual void set_title(const std::string & title);

    libraryclient::LibraryClient::Ptr getLibraryClient()
        { return m_libClient; }
protected:
    virtual Gtk::Widget * buildWidget();

private:
    void undo_state();
    void redo_state();

    void on_action_file_import();

    void on_action_file_quit();
    void on_action_file_open();
    void on_open_library();
    void on_action_edit_labels();

    void preference_dialog_setup(const Glib::RefPtr<Gtk::Builder> &,
                                 Gtk::Dialog *);
    void on_preferences();

    void create_initial_labels();
    void on_lib_notification(const fwk::Notification::Ptr &n);

    void init_ui();
    void init_actions();

    void open_library(const std::string & libMoniker);
    eng::Label::List &   get_labels()
        { return m_labels; }
		
    fwk::NotificationCenter::Ptr  m_lib_notifcenter;

    Gtk::VBox                      m_vbox;
    Gtk::HPaned                    m_hbox;
    LibraryMainViewController::Ptr m_mainviewctrl; // the main views stacked.
    WorkspaceController::Ptr       m_workspacectrl;
    FilmStripController::Ptr       m_filmstrip;
    ui::SelectionController::Ptr   m_selection_controller;
    Gtk::Statusbar                 m_statusBar;
    Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;
    Glib::RefPtr<Gtk::Action>      m_undo_action;
    Glib::RefPtr<Gtk::Action>      m_redo_action;
    libraryclient::LibraryClient::Ptr m_libClient;
    eng::Label::List               m_labels;
};

}


#endif

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
