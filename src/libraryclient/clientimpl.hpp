/*
 * niepce - libraryclient/clientimpl.hpp
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


#ifndef _LIBRARYCLIENT_CLIENTIMPL_H_
#define _LIBRARYCLIENT_CLIENTIMPL_H_

#include <string>

#include "fwk/base/moniker.hpp"
#include "engine/library/clienttypes.hpp"


namespace libraryclient {

class LocalLibraryServer;

class ClientImpl
{
public:
    static ClientImpl *makeClientImpl(const fwk::Moniker & moniker, 
                                      const fwk::NotificationCenter::Ptr & nc);
		
		ClientImpl(const fwk::Moniker & moniker, const fwk::NotificationCenter::Ptr & nc);
		virtual ~ClientImpl();

		eng::tid_t getAllKeywords();
		eng::tid_t queryKeywordContent(int id);
		eng::tid_t getAllFolders();
		eng::tid_t queryFolderContent(int id);
		eng::tid_t countFolder(int id);
		eng::tid_t requestMetadata(int id);
    eng::tid_t setMetadata(int id, int meta, int value);

    eng::tid_t getAllLabels();
    eng::tid_t createLabel(const std::string & s, const std::string & color);
    eng::tid_t deleteLabel(int id);
    eng::tid_t updateLabel(int id, const std::string & new_name,
                               const std::string & new_color);

    eng::tid_t processXmpUpdateQueue();

		eng::tid_t importFromDirectory(const std::string & dir, bool manage);

protected:
		const fwk::Moniker m_moniker;
		LocalLibraryServer *m_localLibrary;
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