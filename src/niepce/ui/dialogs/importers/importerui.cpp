

#include "fwk/toolkit/frame.hpp"
#include "engine/importer/iimporter.hpp"
#include "importerui.hpp"

namespace ui {

ImporterUI::ImporterUI(std::shared_ptr<eng::IImporter>&& importer,
                       const std::string& name)
  : m_importer(importer)
  , m_name(name)
{
}

std::shared_ptr<eng::IImporter> ImporterUI::get_importer()
{
  return m_importer;
}

const std::string& ImporterUI::id() const
{
  return m_importer->id();
}


const std::string& ImporterUI::name() const
{
  return m_name;
}


void ImporterUI::set_source_selected_callback(const SourceSelected& cb)
{
    m_source_selected_cb = cb;
}

}
