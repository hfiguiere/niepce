

#pragma once

#include <memory>
#include <string>
#include <functional>

namespace Gtk {
class Widget;
}
namespace eng {
class IImporter;
}
namespace fwk {
class Frame;
}

namespace ui {

class IImporterUI {
public:

  virtual ~IImporterUI() {}

  virtual std::shared_ptr<eng::IImporter> get_importer() = 0;

  /** User visible importer name. */
  virtual const std::string& name() const = 0;
  virtual const std::string& id() const = 0;
  virtual Gtk::Widget* setup_widget(const fwk::Frame::Ptr&) = 0;

  typedef std::function<void (const std::string&)> SourceSelected;
  virtual void set_source_selected_callback(const SourceSelected&) = 0;
};

}
