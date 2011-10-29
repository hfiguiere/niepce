/*
 * niepce - libraryclient/uidataprovider.cpp
 *
 * Copyright (C) 2011 Hub Figuiere
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
#include "fwk/base/color.hpp"
#include "engine/db/label.hpp"
#include "uidataprovider.hpp"


namespace libraryclient {

void UIDataProvider::updateLabel(const eng::Label::Ptr & l)
{
    // TODO: will work as long as we have 5 labels or something.
    for(eng::Label::List::iterator iter = m_labels.begin();
        iter != m_labels.end(); ++iter) {
        
        if((*iter)->id() == l->id()) {
            (*iter)->set_label(l->label());
            (*iter)->set_color(l->color());
        }
    }
}


void UIDataProvider::addLabels(const eng::Label::ListPtr & l)
{
    for(eng::Label::List::const_iterator iter = l->begin();
        iter != l->end(); ++iter) {
        m_labels.push_back(eng::Label::Ptr(new eng::Label(*(iter->get()))));
    }
}


void UIDataProvider::deleteLabel(int id)
{
    // TODO: will work as long as we have 5 labels or something.
    for(eng::Label::List::iterator iter = m_labels.begin();
        iter != m_labels.end(); ++iter) {
        
        if((*iter)->id() == id) {
            DBG_OUT("remove label %d", id);
            iter = m_labels.erase(iter);
            break;
        }
    }
}

const fwk::RgbColor * UIDataProvider::colorForLabel(int id) const
{
    for(eng::Label::List::const_iterator iter = m_labels.begin();
        iter != m_labels.end(); ++iter) {
        if((*iter)->id() == id) {
            return &((*iter)->color());
        }
    }
    return NULL;
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
