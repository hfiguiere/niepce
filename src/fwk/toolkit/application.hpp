/*
 * niepce - framework/application.h
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


#ifndef _FRAMEWORK_APPLICATION_H_
#define _FRAMEWORK_APPLICATION_H_

#include <boost/function.hpp>

#include <glibmm/refptr.h>
#include <gtkmm/uimanager.h>
#include <gtkmm/icontheme.h>

#include "fwk/toolkit/configuration.hpp"
#include "fwk/toolkit/frame.hpp"
#include "fwk/toolkit/undo.hpp"


namespace fwk {

class Application 
    : public Controller
{
public:
    typedef boost::shared_ptr<Application> Ptr;
    typedef std::pair<std::string, std::string> ThemeDesc;

    virtual ~Application();

    virtual Glib::ustring get_rc_path();
    virtual int get_use_custom_theme() const;
    virtual void set_use_custom_theme(int theme_idx);
    const std::vector<ThemeDesc> & get_available_themes() const
        {
            return m_themes;
        }

    virtual Frame::Ptr makeMainFrame() = 0;

    Configuration & config()
        { return m_config; }
    Glib::RefPtr<Gtk::UIManager> uiManager()
        { 
            if(!m_refUIManager) {
                m_refUIManager = Gtk::UIManager::create();
            }
            return m_refUIManager; 
        }

    virtual Gtk::Widget * buildWidget();
    virtual void quit();
    void about();
    virtual void add(const Controller::Ptr & sub);
    virtual void terminate();

    Glib::RefPtr<Gtk::IconTheme> getIconTheme() const;

    static Application::Ptr app();
    static int main(boost::function<Application::Ptr (void)> constructor, 
                    int argc, char **argv);

    UndoHistory & undo_history()
        { return m_undo; }
protected:
    Application(const char *);
    static Application::Ptr m_application; 
    virtual void on_about();
    void register_theme(const Glib::ustring & label,
                        const std::string &path);
private:
    Configuration                m_config;
    Glib::RefPtr<Gtk::UIManager> m_refUIManager;
    UndoHistory                  m_undo;
    std::vector<ThemeDesc>       m_themes;
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
