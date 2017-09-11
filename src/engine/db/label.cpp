/*
 * niepce - engine/db/label.cpp
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

#include "label.hpp"

extern "C" {
void engine_db_label_delete(eng::Label*);
eng::Label* engine_db_label_clone(const eng::Label*);
}

namespace eng {

LabelPtr label_wrap(Label* l)
{
    return LabelPtr(l, &engine_db_label_delete);
}

LabelPtr label_clone(const Label* l)
{
    return label_wrap(engine_db_label_clone(l));
}
}
