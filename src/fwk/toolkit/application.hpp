/*
 * niepce - framework/application.h
 *
 * Copyright (C) 2007-2008, 2013 Hubert Figuiere
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


#ifndef _FRAMEWORK_APPLICATION_H_
#define _FRAMEWORK_APPLICATION_H_

#include <functional>

#include <glibmm/refptr.h>
#include <gtkmm/application.h>
#include <gtkmm/uimanager.h>
#include <gtkmm/icontheme.h>

#include "fwk/toolkit/configuration.hpp"
#include "fwk/toolkit/frame.hpp"
#include "fwk/toolkit/undo.hpp"


namespace fwk {

class ModuleManager;

class Application 
    : public Controller
{
public:
    typedef std::shared_ptr<Application> Ptr;
    typedef std::pair<std::string, std::string> ThemeDesc;

    virtual ~Application();

    virtual bool get_use_dark_theme() const;
    virtual void set_use_dark_theme(bool value);

    virtual Frame::Ptr makeMainFrame() = 0;

    Configuration & config()
        { return m_config; }
    Glib::RefPtr<Gtk::UIManager> uiManager()
        { 
            return m_refUIManager; 
        }


    virtual void quit();
    void about();
    virtual void add(const Controller::Ptr & sub);
    virtual void terminate();

    Glib::RefPtr<Gtk::IconTheme> getIconTheme() const;

    static Application::Ptr app();
    static int main(const Application::Ptr & app,
                    int argc, char **argv);

    UndoHistory & undo_history()
        { return m_undo; }
    UndoTransaction * begin_undo(const std::string & label);

    // Module management
    /** @return the module manager 
     *  It is guaranted to be safe to call from the constructor
     */
    ModuleManager * module_manager() const
        { return m_module_manager; }
protected:
    Application(int & argc, char** &argv, const char* app_id, const char *name);
    static Application::Ptr m_application;
    void _add(const Controller::Ptr & sub, bool attach = true);
    virtual void on_about();
private:
    Configuration                m_config;
    Glib::RefPtr<Gtk::UIManager> m_refUIManager;
    UndoHistory                  m_undo;
    ModuleManager               *m_module_manager;
    Glib::RefPtr<Gtk::Application> m_gtkapp;
};

}

#endif
/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:80
  End:
*/
