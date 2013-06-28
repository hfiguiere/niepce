
/*
 * niepce - framework/application.cpp
 *
 * Copyright (C) 2007-2013 Hubert Figuiere
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

#include <functional>
#include <boost/lexical_cast.hpp>

#include <glibmm/i18n.h>
#include <gtkmm/main.h>
#include <gtkmm/aboutdialog.h>
#include <gtkmm/settings.h>

#include "fwk/base/debug.hpp"
#include "fwk/utils/modulemanager.hpp"
#include "application.hpp"
#include "uicontroller.hpp"
#include "frame.hpp"


namespace fwk {

Application::Ptr Application::m_application;

Application::Application(int & argc, char** &argv, const char* app_id,
                         const char * name)
    : m_config(Configuration::make_config_path(name))
    , m_refUIManager(Gtk::UIManager::create())
    , m_module_manager(new ModuleManager())
    , m_gtkapp(Gtk::Application::create(argc, argv, app_id))
{
}


Application::~Application()
{
    delete m_module_manager;
}


Application::Ptr Application::app()
{
    return m_application;
}


Glib::RefPtr<Gtk::IconTheme> Application::getIconTheme() const
{
    return Gtk::IconTheme::get_default();
}

bool Application::get_use_dark_theme() const
{
    bool v;
    try {
        v = boost::lexical_cast<bool>(m_config.getValue("ui_dark_theme", "0"));
    }
    catch(...) {
        v = false;
    }
    return v;
}

void Application::set_use_dark_theme(bool value)
{
    m_config.setValue("ui_dark_theme",
                      boost::lexical_cast<Glib::ustring>(value));
}

/** Main loop.
 * @param constructor the Application object constructor
 * @param argc
 * @param argv
 * @return main return code
 */
int Application::main(const Application::Ptr & app,
                      int /*argc*/, char ** /*argv*/)
{
    bool use_dark = app->get_use_dark_theme();
    Glib::RefPtr<Gtk::Settings> settings = Gtk::Settings::get_default();
    settings->set_property("gtk-application-prefer-dark-theme", use_dark);

    Frame::Ptr window(app->makeMainFrame());
    app->_add(window, false);
    // signal_run() is gone in Gtkmm3. Call directly. Should work.
    app->_ready();
    app->m_gtkapp->run(window->gtkWindow());

    return 0;
}


void Application::terminate()
{
    using std::placeholders::_1;
    std::for_each(m_subs.begin(), m_subs.end(),
                  std::bind(&Controller::terminate, _1));
    std::for_each(m_subs.begin(), m_subs.end(),
                  std::bind(&Controller::clearParent, _1));
    m_subs.clear();
}


void Application::quit()
{
    terminate();
    m_gtkapp->quit();
}

void Application::about()
{
    on_about();
}

/** adding a controller to an application build said controller
 * widget
 */
void Application::add(const Controller::Ptr & sub)
{
    _add(sub, true);
}

void Application::_add(const Controller::Ptr & sub, bool attach)
{
    Controller::add(sub);
    UiController::Ptr uictrl = std::dynamic_pointer_cast<UiController>(sub);
    if(uictrl) {
        Gtk::Widget *w = uictrl->buildWidget(uiManager());
        Gtk::Window *win = nullptr;
        if(attach && m_gtkapp && (win = dynamic_cast<Gtk::Window*>(w))) {
            m_gtkapp->add_window(*win);
        }
    }
}

void Application::on_about()
{
    Gtk::AboutDialog dlg;
    dlg.run();
}

UndoTransaction * Application::begin_undo(const std::string & label)
{
    fwk::UndoTransaction *undo = new fwk::UndoTransaction(label);
    undo_history().add(undo);
    return undo;
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
