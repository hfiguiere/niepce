/*
 * niepce - fwk/toolkit/undo.hpp
 *
 * Copyright (C) 2008-2009 Hubert Figuiere
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


#ifndef _FRAMEWORK_UNDO_H_
#define _FRAMEWORK_UNDO_H_

#include <list>
#include <stack>
#include <string>
#include <boost/noncopyable.hpp>

#include <sigc++/signal.h>

#include "fwk/toolkit/command.hpp"

namespace fwk {


class UndoTransaction
{
public:
    UndoTransaction(const std::string & n);
    ~UndoTransaction();
    Command *new_command(const Command::Function &, const Command::Function &);
    void undo();
    void redo();
    /** execute the transaction after adding it. (calls %undo) */
    void execute()
        { redo(); }
    const std::string & name() const
        { return m_name; }
protected:
    /** add the command. Use %new_command instead */
    void add(Command *);
private:
    std::list<Command *> m_operations;
    std::string m_name;
};

class UndoHistory
    : public boost::noncopyable
{
public:
    ~UndoHistory();
		
    /** the history becomes owner */
    void add(UndoTransaction*);
    void undo();
    void redo();
    void clear();
    bool has_undo() const
        { return !m_undos.empty(); }
    bool has_redo() const
        { return !m_redos.empty(); }
    std::string next_undo() const;
    std::string next_redo() const;

    // called when the undo history change.
    sigc::signal<void> signal_changed;
private:
    void clear(std::list<UndoTransaction*> & l);

    std::list<UndoTransaction*> m_undos;
    std::list<UndoTransaction*> m_redos;
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
