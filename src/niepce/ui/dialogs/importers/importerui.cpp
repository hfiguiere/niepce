/* -*- mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode:nil; -*- */
/*
 * niepce - ui/dialogs/importer/importerui.cpp
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
