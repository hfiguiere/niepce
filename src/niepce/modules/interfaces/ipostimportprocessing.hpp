

#ifndef __MODULES_INTF_POSTIMPORTPROCESSING_HPP_
#define __MODULES_INTF_POSTIMPORTPROCESSING_HPP_

#include <sigc++/trackable.h>

#include "engine/db/libfile.hpp"
#include "fwk/utils/modulefactory.hpp"

namespace niepce {

class IPostImportProcessing
  : public fwk::IInterface
  , public sigc::trackable
{
public:
  static const char * IFACE_NAME;

  virtual void post_process_image(const eng::LibFile::Ptr & file) = 0;
};

}

#endif
