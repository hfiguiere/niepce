/*
 * niepce - libraryclient/libraryclient.hpp
 *
 * Copyright (C) 2007-2020 Hubert Figuière
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

#pragma once

#include <string>
#include <memory>

#include "uidataprovider.hpp"

#include "rust_bindings.hpp"

namespace fwk {
class Moniker;
}

namespace libraryclient {

class LibraryClient
{
public:
    LibraryClient() = delete;
    LibraryClient& operator=(const LibraryClient&) = delete;

    LibraryClient(const fwk::Moniker & moniker, const std::shared_ptr<ffi::LcChannel>& channel);
    virtual ~LibraryClient();

    ffi::ThumbnailCache* thumbnailCache()
        { return m_thumbnailCache; }

    const UIDataProviderPtr& getDataProvider() const
        { return m_uidataprovider; }

    ffi::LibraryClientWrapper* client() const {
        return m_client.get();
    }
private:
    std::shared_ptr<ffi::LibraryClientWrapper> m_client;

    ffi::ThumbnailCache* m_thumbnailCache;
    UIDataProviderPtr m_uidataprovider;
};

typedef std::shared_ptr<LibraryClient> LibraryClientPtr;

}
/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  c-basic-offset:4
  tab-width:4
  indent-tabs-mode:nil
  fill-column:80
  End:
*/
