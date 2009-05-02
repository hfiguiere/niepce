/*
 * niepce - ui/dialogs/preferencesdialog.cpp
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

#include <boost/bind.hpp>

#include <glibmm/i18n.h>
#include <gtkmm/combobox.h>
#include <gtkmm/liststore.h>
#include <gtkmm/checkbutton.h>

#include "fwk/utils/boost.hpp"
#include "fwk/toolkit/configdatabinder.hpp"
#include "fwk/toolkit/application.hpp"
#include "fwk/toolkit/gtkutils.hpp"


#include "preferencesdialog.hpp"

namespace ui {

void PreferencesDialog::setup_widget()
{
    if(m_is_setup) {
        return;
    }

    add_header(_("Preferences"));

    Gtk::ComboBox * theme_combo = NULL;
    Gtk::CheckButton * reopen_checkbutton = NULL;
    utils::DataBinderPool * binder_pool = new utils::DataBinderPool();

    gtkDialog().signal_hide().connect(boost::bind(&utils::DataBinderPool::destroy, 
                                              binder_pool));
		
    builder()->get_widget("theme_combo", theme_combo);
 
		Glib::RefPtr<Gtk::ListStore> model = m_theme_combo_model.inject(*theme_combo);
		
		const std::vector<fwk::Application::ThemeDesc> & themes 
			= fwk::Application::app()->get_available_themes();
    std::vector<fwk::Application::ThemeDesc>::const_iterator i;
    for(i = themes.begin(); i != themes.end(); ++i) {
        DBG_OUT("adding %s", i->first.c_str());
        Gtk::TreeIter iter = model->append();
        iter->set_value(m_theme_combo_model.m_col1, i->first); 
        iter->set_value(m_theme_combo_model.m_col2, i->second); 
    }

    theme_combo->set_active(fwk::Application::app()
                            ->get_use_custom_theme());
    theme_combo->signal_changed().connect(
			boost::bind(&fwk::Application::set_use_custom_theme,
									fwk::Application::app(),
									theme_combo->property_active()));

    builder()->get_widget("reopen_checkbutton", reopen_checkbutton);
    binder_pool->add_binder(new fwk::ConfigDataBinder<bool>(
															reopen_checkbutton->property_active(),
															fwk::Application::app()->config(),
															"reopen_last_library"));
		m_is_setup = true;
}


}

