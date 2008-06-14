/*
 * niepce - framework/undo.h
 *
 * Copyright (C) 2008 Hubert Figuiere
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
#include <boost/signal.hpp>
#include <boost/noncopyable.hpp>

namespace framework {

	class Command;

	class UndoTransaction
	{
	public:
		UndoTransaction(const std::string & n);
        ~UndoTransaction();
        void add(Command *);
		void undo();
		void redo();
		const std::string & name() const
			{ return m_name; }
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
		boost::signal<void (void)> changed;
	private:
		void clear(std::list<UndoTransaction*> & l);

		std::list<UndoTransaction*> m_undos;
		std::list<UndoTransaction*> m_redos;
	};

	

}

#endif
