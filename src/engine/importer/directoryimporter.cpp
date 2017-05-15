/*
 * niepce - engine/importer/directoryimporter.cpp
 *
 * Copyright (C) 2014-2017 Hubert Figuière
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

#include <glibmm/i18n.h>

#include "fwk/base/debug.hpp"
#include "fwk/utils/pathutils.hpp"
#include "engine/importer/directoryimporter.hpp"
#include "engine/importer/importedfile.hpp"

namespace eng {

class DirectoryImportedFile
  : public ImportedFile
{
public:
  DirectoryImportedFile(const std::string & path)
    : m_path(path)
    {
      m_name = fwk::path_basename(path);
    }
  const std::string & name() override
    {
      return m_name;
    }
private:
  std::string m_name;
  std::string m_path;
};


DirectoryImporter::DirectoryImporter()
{
}

DirectoryImporter::~DirectoryImporter()
{
}

std::string DirectoryImporter::name() const
{
  return _("Directory");
}

bool DirectoryImporter::listSourceContent(const std::string & source,
                                          const SourceContentReady& callback)
{
  auto files =
    fwk::FileList::getFilesFromDirectory(source,
                                         &fwk::filter_xmp_out);

  std::list<ImportedFile::Ptr> content;
  for(const auto & entry : *files)
  {
    content.push_back(ImportedFile::Ptr(new DirectoryImportedFile(entry)));
  }
  callback(std::move(content));

  return true;
}

bool DirectoryImporter::doImport(const std::string& source,
                                 const FileImporter& callback)
{
  // pretty trivial, we have the source path.
  callback(source, false);

  // XXX return a real error
  return true;
}

}
