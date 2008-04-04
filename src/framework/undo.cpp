/*
 * niepce - framework/undo.cpp
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


#include <boost/bind.hpp>
#include <boost/checked_delete.hpp>

#include "utils/debug.h"
#include "undo.h"

namespace framework {

	void UndoTransaction::undo()
	{
		DBG_OUT("undo transaction");
	}

	void UndoTransaction::redo()
	{
		DBG_OUT("redo transaction");
	}

	UndoHistory::~UndoHistory()
	{
		clear();
	}

	void UndoHistory::add(UndoTransaction* t)
	{
		m_undos.push_front(t);
		clear(m_redos);

		changed();
	}

	void UndoHistory::undo()
	{
		DBG_OUT("run undo history");
		if(!m_undos.empty()) {
			UndoTransaction * t = m_undos.front();
			if(t) {
				t->undo();
				m_undos.pop_front();
				m_redos.push_front(t);
				changed();
			}
		}
	}

	void UndoHistory::redo()
	{
		DBG_OUT("run redo history");
		if(!m_redos.empty()) {
			UndoTransaction * t = m_redos.front();
			if(t) {
				t->redo();
				m_redos.pop_front();
				m_undos.push_front(t);
				changed();
			}
		}
	}


	void UndoHistory::clear()
	{
		clear(m_undos);
		clear(m_redos);

		changed();
	}

	std::string UndoHistory::next_undo() const
	{
		if(!m_undos.empty()) {
			UndoTransaction * t = m_undos.front();
			if(t) {
				return t->name();
			}
		}
		return "";
	}

	std::string UndoHistory::next_redo() const
	{
		if(!m_redos.empty()) {
			UndoTransaction * t = m_redos.front();
			if(t) {
				return t->name();
			}
		}
		return "";
	}

	
	void UndoHistory::clear(std::list<UndoTransaction*> & l)
	{
		std::for_each(l.begin(), l.end(), 
					  boost::bind(&boost::checked_delete<UndoTransaction>, _1));
		l.clear();
	}

}
