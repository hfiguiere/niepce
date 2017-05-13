/*
 * niepce - framework/undo.cpp
 *
 * Copyright (C) 2008-2017 Hubert Figuière
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


#include <algorithm>

#include <boost/checked_delete.hpp>

#include "fwk/base/debug.hpp"
#include "command.hpp"
#include "undo.hpp"

namespace fwk {

UndoTransaction::UndoTransaction(const std::string & n)
    : m_name(n)
{
}

UndoTransaction::~UndoTransaction()
{
}


void UndoTransaction::add(const std::shared_ptr<Command>& cmd)
{
    m_operations.push_back(cmd);
}

void UndoTransaction::undo()
{
    DBG_OUT("undo transaction %lu cmd", (unsigned long)m_operations.size());
    std::for_each(m_operations.rbegin(), m_operations.rend(),
                  [] (auto cmd) {
                      cmd->undo();
                  });
}

void UndoTransaction::redo()
{
    DBG_OUT("redo transaction %lu cmd", (unsigned long)m_operations.size());
    std::for_each(m_operations.begin(), m_operations.end(),
                  [] (auto cmd) {
                      cmd->redo();
                  });
}

UndoHistory::~UndoHistory()
{
    // DO NOT CALL UndoHistory::clear() !!!
}

void UndoHistory::add(const std::shared_ptr<UndoTransaction>& t)
{
    m_undos.push_front(t);
    m_redos.clear();

    signal_changed();
}

void UndoHistory::undo()
{
    DBG_OUT("run undo history");
    if(!m_undos.empty()) {
        std::shared_ptr<UndoTransaction> t = m_undos.front();
        if(t) {
            t->undo();
            m_undos.pop_front();
            m_redos.push_front(t);
            signal_changed();
        }
    }
}

void UndoHistory::redo()
{
    DBG_OUT("run redo history");
    if(!m_redos.empty()) {
        std::shared_ptr<UndoTransaction> t = m_redos.front();
        if(t) {
            t->redo();
            m_redos.pop_front();
            m_undos.push_front(t);
            signal_changed();
        }
    }
}


void UndoHistory::clear()
{

    signal_changed();
}

std::string UndoHistory::next_undo() const
{
    if(!m_undos.empty()) {
        auto t = m_undos.front();
        if(t) {
            return t->name();
        }
    }
    return "";
}

std::string UndoHistory::next_redo() const
{
    if(!m_redos.empty()) {
        auto t = m_redos.front();
        if(t) {
            return t->name();
        }
    }
    return "";
}

}

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
