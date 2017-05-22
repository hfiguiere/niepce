

#include "cameraimporter.hpp"

namespace eng {

CameraImporter::CameraImporter()
{
}

CameraImporter::~CameraImporter()
{
}

const std::string& CameraImporter::id() const
{
  static std::string _id = "CameraImporter";
  return _id;
}

bool CameraImporter::list_source_content(const std::string & source,
                                         const SourceContentReady& callback)
{
}

bool CameraImporter::get_previews_for(const std::string& source,
                                      const std::list<std::string>& paths,
                                      const PreviewReady& callback)
{
}

bool CameraImporter::do_import(const std::string & source,
                               const FileImporter & importer)
{
}

}
