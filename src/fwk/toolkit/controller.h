/*
 * niepce - framework/controller.h
 *
 * Copyright (C) 2007 Hubert Figuiere
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



#ifndef __FRAMEWORK_CONTROLLER_H__
#define __FRAMEWORK_CONTROLLER_H__


#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "fwk/utils/databinder.h"

namespace Gtk {
	class Widget;
}

namespace framework {

	/** Generic controller class
	 */
	class Controller
		: public boost::enable_shared_from_this<Controller>
	{
	public:
		typedef boost::shared_ptr<Controller> Ptr;
		typedef boost::weak_ptr<Controller> WeakPtr;

		Controller();
		virtual ~Controller();

		/** add a subcontroller to this one */
		void add(const Ptr & sub);
		/** clear the parent. Usually called by the parent when unparenting */
		void clearParent()
			{ m_parent.reset(); }
		void remove(const Ptr & sub);
		
		virtual bool canTerminate();
		/** signal that the controller needs to terminate */
		virtual void terminate();

		/** return the widget controlled (construct it if needed) */
		virtual Gtk::Widget * buildWidget() = 0;
		Gtk::Widget * widget();

		/** called when everything is ready 
		 * subclasses should reimplement if needed
		 */
		virtual void on_ready();
	protected:
		/** called when the controller has been added to a parent. */
		virtual void _added();

		void _ready();
		Gtk::Widget* m_widget;

		WeakPtr          m_parent;
		std::list<Ptr> m_subs; /**< sub controllers */

		utils::DataBinderPool m_databinders;
	};

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

#endif
