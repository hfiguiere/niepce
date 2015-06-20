/*
 * niepce - fwk/command.h
 *
 * Copyright (C) 2008-2013 Hubert Figuiere
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


#ifndef __FWK_COMMAND_H_
#define __FWK_COMMAND_H_

#include <functional>

namespace fwk {

class Command
{
public:
    virtual ~Command() {}
    typedef std::function<void (void)> Function;
    virtual void undo() = 0;
    virtual void redo() = 0;
};

template <typename _ArgType>
class CommandWithArg
    : public Command
{
public:
    typedef std::function<void (_ArgType)> UndoFunction;
    typedef std::function<_ArgType (void)> RedoFunction;
    CommandWithArg(const RedoFunction & _redo,
                   const UndoFunction & _undo)
        : m_redo(_redo)
        , m_undo(_undo)
        {
        }
        
    virtual void undo() override
        {
            m_undo(m_argstorage);
        }
    virtual void redo() override
        {
            m_argstorage = m_redo();
        }
private:
    RedoFunction m_redo;
    UndoFunction m_undo;
    _ArgType     m_argstorage;
};

template <>
class CommandWithArg<void>
    : public Command
{
public:
    typedef std::function<void (void)> UndoFunction;
    typedef std::function<void (void)> RedoFunction;
    CommandWithArg(const RedoFunction & _redo,
                   const UndoFunction & _undo)
        : m_redo(_redo)
        , m_undo(_undo)
        {
        }
        
    virtual void undo() override
        {
            m_undo();
        }
    virtual void redo() override
        {
            m_redo();
        }
private:
    RedoFunction m_redo;
    UndoFunction m_undo;
};

}
#endif

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
