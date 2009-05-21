

#include "config.h"

#include "cwapplication.hpp"


namespace cw {

  CwApplication::CwApplication()
    : Application(PACKAGE)
  {
  }


  fwk::Application::Ptr CwApplication::create()
  {
    if (!m_application) {
      m_application = fwk::Application::Ptr(new CwApplication());
    }
    return m_application;
  }


  fwk::Frame::Ptr CwApplication::makeMainFrame()
  {
    return fwk::Frame::Ptr();
  }

}
