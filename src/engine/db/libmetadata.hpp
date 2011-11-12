/*
 * niepce - eng/db/libmetadata.hpp
 *
 * Copyright (C) 2008 Hubert Figuiere
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

#ifndef __DB_LIBMETADATA_H_
#define __DB_LIBMETADATA_H_

#include <vector>
#include <string>
#include <tr1/memory>
#include <boost/any.hpp>

#include "fwk/base/propertybag.hpp"
#include "fwk/utils/exempi.hpp"
#include "engine/db/librarytypes.hpp"
#include "engine/db/metadata.hpp"

namespace eng {

class LibMetadata
    : public fwk::XmpMeta
{
public:
    typedef std::tr1::shared_ptr<LibMetadata> Ptr;
    
    LibMetadata(library_id_t _id);
    
    library_id_t id() const
        { return m_id; }
    bool setMetaData(fwk::PropertyIndex meta, const fwk::PropertyValue & value);
    /** do like the unix "touch". Update the MetadataDate 
     * to the current time, in UTC.
     */
    bool touch();
private:
    LibMetadata(const LibMetadata &);
    library_id_t m_id;
};

}

#endif

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
