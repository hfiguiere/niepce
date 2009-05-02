/*
 * niepce - ui/selectioncontroller.h
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


#ifndef _UI_SELECTIONCONTROLLER_H__
#define _UI_SELECTIONCONTROLLER_H__

#include <gtk/gtk.h>

#include <sigc++/signal.h>

#include "fwk/toolkit/controller.hpp"
#include "ui/imageliststore.h"

namespace Gtk {
	class IconView;
	class Widget;
}

namespace ui {

/** interface for selectable image. Make the controller
 *  inherit/implement it.
 */
class IImageSelectable 
{
public:
	virtual ~IImageSelectable() {}
	virtual Gtk::IconView * image_list() = 0;
    /** Return the id of the selection. <= 0 is none. */
	virtual int get_selected() = 0;
    /** select the image a specific id 
     *  might emit the signals.
     */
	virtual void select_image(int id) = 0;
};


class SelectionController
	: public fwk::Controller
{
public:
	typedef boost::shared_ptr<SelectionController> Ptr;
	SelectionController();

	void add_selectable(IImageSelectable *);

	void activated(const Gtk::TreeModel::Path & /*path*/,
				   IImageSelectable * selectable);
	void selected(IImageSelectable *);


	const Glib::RefPtr<ImageListStore> & list_store() const
		{ return m_imageliststore; }

	// the signal to call when selection is changed.
    sigc::signal<void, int> signal_selected;

	// signal for when the item is activated (ie double-click)
    sigc::signal<void, int> signal_activated;

    /////////
    /** select the previous image. Emit the signal */
    void select_previous();
    /** select the next image. Emit the signal */
    void select_next();
    /** rotate the image in selection by %angle (trigonometric) */
    void rotate(int angle);
    /** set the rating of selection to %rating. */
    void set_rating(int rating);
    /** set the label of selection to the label with index %label. */
    void set_label(int label);
protected:
	virtual void _added();
	virtual Gtk::Widget * buildWidget()
		{ return NULL; }
private:
    int get_selection();
    libraryclient::LibraryClient::Ptr getLibraryClient();

    bool _set_metadata(const std::string & undo_label, 
                       const db::LibFile::Ptr & file,
                       int meta, int old_value, int new_value);
    /** move the selection and emit the signal 
     * @param backwards true if the move is backwards.
     */
    void _selection_move(bool backwards);

	Glib::RefPtr<ImageListStore>  m_imageliststore;
	bool m_in_handler;
	std::vector<IImageSelectable *> m_selectables;
};

}

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0))
  indent-tabs-mode:nil
  fill-column:80
  End:
*/

#endif
