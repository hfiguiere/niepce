/*
 * niepce - libraryclient/uidataprovider.hpp
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

#ifndef __LIBRARYCLIENT_UIDATAPROVIDER_HPP__
#define __LIBRARYCLIENT_UIDATAPROVIDER_HPP__

#include <stdint.h>

#include "fwk/base/option.hpp"
#include "fwk/base/colour.hpp"
#include "engine/db/label.hpp"

namespace libraryclient {

class UIDataProvider
{
public:
    // label management

    void updateLabel(const eng::Label &);
    void addLabel(const eng::Label & l);
    void deleteLabel(int id);
    fwk::Option<fwk::RgbColourPtr> colourForLabel(int id) const;
    const eng::LabelList & getLabels() const
        { return m_labels; }
private:
    eng::LabelList m_labels;
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

