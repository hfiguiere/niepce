/* -*- mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode:nil; -*- */
/*
 * niepce - ui/dialogs/importers/iimporterui.hpp
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
