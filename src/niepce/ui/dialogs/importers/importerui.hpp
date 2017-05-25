/* -*- mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode:nil; -*- */
/*
 * niepce - ui/dialogs/importer/importerui.hpp
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

#include "iimporterui.hpp"

namespace fwk {
class Frame;
}

namespace ui {

class ImporterUI
    : public IImporterUI
    , public sigc::trackable
{
public:
    ImporterUI(std::shared_ptr<eng::IImporter>&& importer,
               const std::string& name);

    std::shared_ptr<eng::IImporter> get_importer() override;

    const std::string& name() const override;
    const std::string& id() const override;

    void set_source_selected_callback(const SourceSelected&) override;

protected:
    std::shared_ptr<eng::IImporter> m_importer;
    std::string m_name;
    std::weak_ptr<fwk::Frame> m_frame;
    Glib::RefPtr<Gtk::Builder> m_builder;
    SourceSelected m_source_selected_cb;
};

}
