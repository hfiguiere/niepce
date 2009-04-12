/*
 * niepce - fwk/toolkit/dialog.hpp
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


#ifndef __FWK_DIALOG_H__
#define __FWK_DIALOG_H__

#include <gtkmm/dialog.h>

#include "fwk/toolkit/frame.hpp"

namespace fwk {

/** 
 * Implement a dialog.
 */ 
class Dialog
	: public fwk::Frame
{
public:
    typedef boost::shared_ptr<Dialog> Ptr;

		Dialog(const std::string & gladeFile, const Glib::ustring & widgetName,
          const std::string & layout_cfg_key = "")
        : Frame(gladeFile, widgetName, layout_cfg_key)
        , m_is_setup(false)
        { 
        }

    /** this is called prior to show the dialog 
     * subclass must implement and set %m_is_setup to true
     */
    virtual void setup_widget() = 0;
    Gtk::Dialog & gtkDialog()
        { return static_cast<Gtk::Dialog&>(gtkWindow()); }

    /** add a header widget at the top of the "dialog-vbox1"
     */
    void add_header(const Glib::ustring & label);

    int run_modal();
    int run_modal(const Frame::Ptr & parent);
protected:
    bool m_is_setup;
};

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

#endif
