/*
 * niepce - niepce/ui/dialogs/editlabels.hpp
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


#ifndef __UI_EDITLABELS_HPP__
#define __UI_EDITLABELS_HPP__

#include <gtkmm/label.h>

#include "engine/db/label.hpp"
#include "libraryclient/libraryclient.hpp"
#include "fwk/toolkit/dialog.hpp"

namespace ui {


class EditLabels
    : public fwk::Dialog
{
public:
    typedef boost::shared_ptr<EditLabels> Ptr;
    EditLabels(const eng::Label::List &, const libraryclient::LibraryClient::Ptr &);

    virtual void setup_widget();
private:
    void label_name_changed(Gtk::Entry *, size_t idx);
    eng::Label::List m_labels;
    libraryclient::LibraryClient::Ptr m_lib_client;
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
