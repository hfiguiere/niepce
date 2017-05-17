

#pragma once

#include <memory>

#include "engine/importer/iimporter.hpp"

namespace fwk {
class Frame;
}

namespace ui {

class IImporterUI {
public:

  virtual std::shared_ptr<eng::IImporter> get_importer() = 0;
  virtual std::string select_source(const fwk::Frame&) = 0;

};

}
