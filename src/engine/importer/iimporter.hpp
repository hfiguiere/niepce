/*
 * niepce - engine/importer/iimporter.hpp
 *
 * Copyright (C) 2014 Hubert Figuiere
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

#include "engine/importer/importedfile.hpp"

namespace eng {

/**
 * Interface for importers.
 */
class IImporter {
public:
  virtual ~IImporter() {}

  /** User visible importer name. */
  virtual std::string name() const = 0;

  /** list the target content and store it. */
  virtual bool listTargetContent(const std::string & target) = 0;

  /** get the target content. */
  virtual std::list<ImportedFile::Ptr> getTargetContent() = 0;
};

}
/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  c-basic-offset:2
  indent-tabs-mode:nil
  tab-width:2
  fill-column:99
  End:
*/
