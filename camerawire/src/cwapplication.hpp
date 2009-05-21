


#ifndef __CW_APPLICATION_HPP_
#define __CW_APPLICATION_HPP_


#include "fwk/toolkit/application.hpp"



namespace cw {

class CwApplication
  : public fwk::Application
{
public:
  static fwk::Application::Ptr create();

  virtual fwk::Frame::Ptr makeMainFrame();
protected:
  CwApplication();

};

}


#endif
