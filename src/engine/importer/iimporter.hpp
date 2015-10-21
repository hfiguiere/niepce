/*
 * niepce - engine/importer/iimporter.hpp
 *
 * Copyright (C) 2014-2015 Hubert Figuière
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

  /** file importer callback */
  typedef std::function<void (const std::string&, bool)> file_importer;

  /** User visible importer name. */
  virtual std::string name() const = 0;

  /** list the source content and store it. */
  virtual bool listSourceContent(const std::string & source) = 0;

  /** get the source content. */
  virtual std::list<ImportedFile::Ptr> getSourceContent() = 0;

  /** perform import from source */
  virtual bool doImport(const std::string & source,
                        const file_importer & importer) = 0;

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
