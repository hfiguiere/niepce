/*
 * niepce - ui/dialogs/importer/cameraimporterui.cpp
 *
 * Copyright (C) 2017 Hubert Figuière
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
#include <gtkmm/label.h>

#include "fwk/toolkit/frame.hpp"
#include "engine/importer/cameraimporter.hpp"
#include "cameraimporterui.hpp"

namespace ui {

CameraImporterUI::CameraImporterUI()
  : ImporterUI(std::make_shared<eng::CameraImporter>(), _("Camera"))
{
}

Gtk::Widget* CameraImporterUI::setup_widget(const fwk::Frame::Ptr&)
{
  Gtk::Label* label = Gtk::manage(new Gtk::Label("camera! camera! camera!"));
  return label;
}

}
