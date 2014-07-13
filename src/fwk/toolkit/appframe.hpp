

#pragma once

#include <gtkmm/applicationwindow.h>

#include "fwk/toolkit/frame.hpp"

namespace fwk {

class AppFrame
  : public Frame
{
public:
  typedef std::shared_ptr<AppFrame> Ptr;

  AppFrame(const std::string & layout_cfg_key = "")
    : Frame(new Gtk::ApplicationWindow(), layout_cfg_key)
    {
    }

  Glib::RefPtr<Gio::Menu> get_menu() const
    { return m_menu; }

protected:
  Glib::RefPtr<Gio::Menu> m_menu;
};

}
