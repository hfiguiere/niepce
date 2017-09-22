/*
 * niepce - libraryclient/locallibraryserver.hpp
 *
 * Copyright (C) 2007-2017 Hubert Figuière
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
 */

#ifndef _LIBRARYCLIENT_LOCALLIBRARYSERVER_H_
#define _LIBRARYCLIENT_LOCALLIBRARYSERVER_H_

#include "fwk/utils/worker.hpp"
#include "engine/library/op.hpp"
#include "engine/db/library.hpp"
namespace libraryclient {

class LocalLibraryServer : public fwk::Worker<eng::Op> {
public:
  /** create the local server for the library whose dir is specified */
  LocalLibraryServer(const std::string& dir, uint64_t notif_id)
    : fwk::Worker<eng::Op>()
    , m_library(eng::library_new(dir.c_str(), notif_id))
    {
    }
  bool ok() const { return m_library && engine_db_library_ok(m_library.get()); }

protected:
  virtual void execute(const ptr_t& _op) override;

private:
  eng::LibraryPtr m_library;
};

}

#endif
