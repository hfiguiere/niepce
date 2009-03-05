/*
 * niepce - framework/application.cpp
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

#include <list>
#include <vector>
#include <boost/bind.hpp>

#include <gtkmm/dialog.h>

#include "fwk/utils/debug.h"
#include "fwk/utils/boost.h"
#include "fwk/utils/geometry.h"
#include "frame.hpp"
#include "application.hpp"



namespace framework {

Frame::Frame(const std::string & layout_cfg_key)
		: m_window(new Gtk::Window()),
		  m_builder(NULL),
		  m_layout_cfg_key(layout_cfg_key)
{
		connectSignals();
		frameRectFromConfig();
}


Frame::Frame(const std::string & gladeFile, 
             const Glib::ustring & widgetName,
             const std::string & layout_cfg_key)
		: m_window(NULL),
		  m_builder(Gtk::Builder::create_from_file(gladeFile)),
		  m_layout_cfg_key(layout_cfg_key)
{
		if (m_builder) {
        m_builder->get_widget(widgetName, m_window);
        connectSignals();
        frameRectFromConfig();
		}
}


void Frame::connectSignals()
{
		m_window->signal_delete_event().connect(
        sigc::hide(sigc::mem_fun(this, &Frame::_close)));
		m_window->signal_hide().connect(
        sigc::retype_return<void>(sigc::mem_fun(this, &Frame::_close)));
}

Frame::~Frame()
{
		frameRectToConfig();
    DBG_OUT("destroy Frame");
    delete m_window;
}


void Frame::set_icon_from_theme(const Glib::ustring & name)
{
		using Glib::RefPtr;
		using Gtk::IconTheme;
		using std::vector;
		using std::list;
		
		RefPtr< IconTheme > icon_theme(Application::app()->getIconTheme());
		vector<int> icon_sizes(icon_theme->get_icon_sizes(name));
		
		list< RefPtr <Gdk::Pixbuf> > icons;

		for_each(icon_sizes.begin(), icon_sizes.end(),
             // store the icon
             bind(&std::list< RefPtr<Gdk::Pixbuf> >::push_back, 
                  boost::ref(icons), 
                  // load the icon
                  bind( &IconTheme::load_icon, 
                        boost::ref(icon_theme), 
                        boost::ref(name), _1, 
                        Gtk::ICON_LOOKUP_USE_BUILTIN)));
		gtkWindow().set_icon_list(icons);
}

void Frame::set_title(const std::string & title)
{
		gtkWindow().set_title(Glib::ustring(title));
}

int Frame::show_modal_dialog(Gtk::Dialog & dlg)
{
		int result;
		dlg.set_transient_for(*m_window);
		dlg.set_default_response(Gtk::RESPONSE_CLOSE);
		result = dlg.run();
		dlg.hide();
		return result;
}

int Frame::show_modal_dialog(const char *gladefile,
                             const char *widgetname,
                             boost::function<void (const Glib::RefPtr<Gtk::Builder> &, 
                                                   Gtk::Dialog *)> setup)
{
		Glib::RefPtr<Gtk::Builder> dialogBuilder 
        = Gtk::Builder::create_from_file(gladefile);
		Gtk::Dialog *dlg = NULL;

		dialogBuilder->get_widget(widgetname, dlg);
		if(setup) {
        setup(dialogBuilder, dlg);
		}
		return show_modal_dialog(*dlg);
}

void Frame::toggle_tools_visible()
{
    if(m_hide_tools_action->get_active()) {
        signal_hide_tools.emit();   
    }
    else {
        signal_show_tools.emit();
    }
}


bool Frame::_close()
{
		if(Controller::Ptr parent = m_parent.lock()) {
        parent->remove(shared_from_this());
		}
		return false;
}

void Frame::frameRectFromConfig()
{
		DBG_OUT("loading frame rect (%s)", m_layout_cfg_key.c_str());
		if(!m_layout_cfg_key.empty()) {
        Configuration & cfg = Application::app()->config();
        std::string val;
        val = cfg.getValue(m_layout_cfg_key, "");
        if(!val.empty()) {
            try {
                utils::Rect r(val);
                m_window->move(r.x(), r.y());
                m_window->resize(r.w(), r.h());				
            }
            catch(std::bad_cast)
            {
                ERR_OUT("wrong value in configuration: %s", val.c_str());
            }
        }
		}
}


void Frame::frameRectToConfig()
{
		DBG_OUT("saving frame rect (%s)", m_layout_cfg_key.c_str());
		if(!m_layout_cfg_key.empty()) {
        Configuration & cfg = Application::app()->config();
        int x, y, w, h;
        x = y = w = h = 0;
        m_window->get_position(x, y);
        m_window->get_size(w, h);
        utils::Rect r(x, y, w, h);
        cfg.setValue(m_layout_cfg_key, r.to_string());
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
}
