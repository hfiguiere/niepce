/*
 * niepce - libraryclient/uidataprovider.cpp
 *
 * Copyright (C) 2011-2017 Hubert Figuière
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

#include <algorithm>
#include <boost/bind.hpp>

#include "fwk/base/debug.hpp"
#include "fwk/base/colour.hpp"
#include "uidataprovider.hpp"


namespace libraryclient {

void UIDataProvider::updateLabel(const eng::Label & l)
{
    // TODO: will work as long as we have 5 labels or something.
    for (auto & label : m_labels) {
        if (engine_db_label_id(label.get()) == engine_db_label_id(&l)) {
            label = eng::label_clone(&l);
        }
    }
}


void UIDataProvider::addLabel(const eng::Label & l)
{
    m_labels.push_back(eng::label_clone(&l));
}


void UIDataProvider::deleteLabel(int id)
{
    // TODO: will work as long as we have 5 labels or something.
    for(auto iter = m_labels.begin();
        iter != m_labels.end(); ++iter) {

        if (engine_db_label_id(iter->get()) == id) {
            DBG_OUT("remove label %d", id);
            iter = m_labels.erase(iter);
            break;
        }
    }
}

fwk::Option<fwk::RgbColourPtr> UIDataProvider::colourForLabel(int id) const
{
    for(auto label : m_labels) {
        if (engine_db_label_id(label.get()) == id) {
            return fwk::Option<fwk::RgbColourPtr>(
                fwk::rgbcolour_clone(engine_db_label_colour(label.get())));
        }
    }
    return fwk::Option<fwk::RgbColourPtr>();
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

