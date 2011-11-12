/* Eye Of Gnome - Thumbnail View
 *
 * Copyright (C) 2006 The Free Software Foundation
 * Copyright (C) 2007-2009 Hubert Figuiere
 *
 * C++-ization: Hubert Figuiere <hub@figuiere.net>
 * Original Author: Claudio Saavedra <csaavedra@alumnos.utalca.cl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <string>

#include <glibmm/i18n.h>
#include <gdkmm.h>
#include <gtkmm/adjustment.h>
#include <gtkmm/scrolledwindow.h>

#include "fwk/base/debug.hpp"
#include "fwk/utils/boost.hpp"
#include "engine/db/libfile.hpp"
#include "thumbstripview.hpp"
#include "librarycellrenderer.hpp"

#include <gdkmm/general.h>

struct EogThumbView;

namespace ui {

#define THUMB_STRIP_VIEW_SPACING 0

/* Drag 'n Drop */
enum {
	TARGET_PLAIN,
	TARGET_PLAIN_UTF8,
	TARGET_URILIST
};

#if 0
static GtkTargetEntry target_table[] = {
	{ const_cast<gchar*>("text/uri-list"), 0, TARGET_URILIST },
};
#endif

class ThumbStripCell
    : public LibraryCellRenderer
{
public:
    ThumbStripCell();
};


ThumbStripCell::ThumbStripCell()
    : Glib::ObjectBase(typeid(ThumbStripCell))
    , LibraryCellRenderer(NULL)
{
    set_pad(0);
    set_size(100);
    set_drawborder(false);
    set_drawemblem(false);
    set_drawrating(false);
    set_drawlabel(false);
    set_drawflag(false);
}



ThumbStripView::ThumbStripView(const Glib::RefPtr<ui::ImageListStore> & store)
    : m_start_thumb(0)
    , m_end_thumb(0)
    , m_store(store)
{
    IconView::set_model(store);

    m_renderer = new ThumbStripCell();

    pack_start(*m_renderer, FALSE);
    m_renderer->property_follow_state() = true;
    m_renderer->property_height() = 100;
    m_renderer->property_yalign() = 0.5;
    m_renderer->property_xalign() = 0.5;
	
    add_attribute(*m_renderer, "pixbuf",
                  ui::ImageListStore::Columns::STRIP_THUMB_INDEX);
    add_attribute(*m_renderer, "libfile",
                  ui::ImageListStore::Columns::FILE_INDEX);
    set_selection_mode(Gtk::SELECTION_MULTIPLE);
    set_column_spacing(THUMB_STRIP_VIEW_SPACING);

    set_row_spacing (THUMB_STRIP_VIEW_SPACING);
    set_margin (0);

    signal_parent_changed().connect(
        sigc::mem_fun(*this, &ThumbStripView::on_parent_set));

//  enable_model_drag_source (target_table, Gdk::MODIFIER_MASK, Gdk::ACTION_COPY);

//  signal_drag_data_get().connect(
//      sigc::mem_fun(*this, &ThumbStripView::on_drag_data_get));
}

void ThumbStripView::set_model(const Glib::RefPtr<ui::ImageListStore> & store)
{
    m_store = store;
    IconView::set_model(store);
}

const Glib::RefPtr<ui::ImageListStore> & ThumbStripView::get_model() const
{
    return m_store;
}

void
ThumbStripView::clear_range (int start_thumb, int end_thumb)
{
//    Gtk::TreeIter iter;
//    int thumb = start_thumb;
	
    g_assert (start_thumb <= end_thumb);
	
//    for (iter = m_store->children().begin() + start_thumb;
//         iter && thumb <= end_thumb;
//         ++iter, thumb++) {
//		eog_list_store_thumbnail_unset (get_pointer(store), &iter);
//    }
}

void
ThumbStripView::add_range (int start_thumb, int end_thumb)
{
//    Gtk::TreeIter iter;
//    gint thumb = start_thumb;
	
    g_assert (start_thumb <= end_thumb);
    
//    for (iter = m_store->children().begin() + start_thumb;
//         iter  && thumb <= end_thumb;
//         ++iter, thumb++) {
//		eog_list_store_thumbnail_set (get_pointer(store), &iter);
//    }
}

void
ThumbStripView::update_visible_range (int start_thumb, int end_thumb)
{
    int old_start_thumb, old_end_thumb;

    old_start_thumb = m_start_thumb;
    old_end_thumb = m_end_thumb;
	
    if (start_thumb == old_start_thumb &&
        end_thumb == old_end_thumb) {
        return;
    }
	
    if (old_start_thumb < start_thumb)
        clear_range (old_start_thumb, MIN (start_thumb - 1, old_end_thumb));

    if (old_end_thumb > end_thumb)
        clear_range (MAX (end_thumb + 1, old_start_thumb), old_end_thumb);

    add_range (start_thumb, end_thumb);
	
    m_start_thumb = start_thumb;
    m_end_thumb = end_thumb;
}

void
ThumbStripView::on_visible_range_changed()
{
    Gtk::TreePath path1, path2;

    if (!get_visible_range (path1, path2)) {
        return;
    }
		
    update_visible_range (path1[0], path2[0]);
}

void
ThumbStripView::on_adjustment_changed ()
{
    Gtk::TreePath path1, path2;
    int start_thumb, end_thumb;
	
    if (!get_visible_range (path1, path2)) {
        return;
    }

    start_thumb = path1 [0];
    end_thumb = path2 [0];

    add_range (start_thumb, end_thumb);	

    /* case we added an image, we need to make sure that the shifted thumbnail is cleared */
    clear_range (end_thumb + 1, end_thumb + 1);

    m_start_thumb = start_thumb;
    m_end_thumb = end_thumb;
}

void
ThumbStripView::on_parent_set (Gtk::Widget */*old_parent*/)
{
    Gtk::ScrolledWindow *sw;
    Glib::RefPtr<Gtk::Adjustment> hadjustment;
    Glib::RefPtr<Gtk::Adjustment> vadjustment;

    Gtk::Widget *parent = get_parent ();
    sw = dynamic_cast<Gtk::ScrolledWindow*>(parent);
    if (!sw) {
        return;
    }
	
    /* if we have been set to a ScrolledWindow, we connect to the callback
       to set and unset thumbnails. */
    hadjustment = sw->get_hadjustment ();
    vadjustment = sw->get_vadjustment ();
	
    /* when scrolling */
    hadjustment->signal_value_changed().connect(
        sigc::mem_fun(*this, &ThumbStripView::on_visible_range_changed));
    vadjustment->signal_value_changed().connect(
        sigc::mem_fun(*this, &ThumbStripView::on_visible_range_changed));
	
    /* when the adjustment is changed, ie. probably we have new images added. */
    hadjustment->signal_changed().connect(
        sigc::mem_fun(*this, &ThumbStripView::on_adjustment_changed));
    vadjustment->signal_changed().connect(
        sigc::mem_fun(*this, &ThumbStripView::on_adjustment_changed));

    /* when resizing the scrolled window */
    sw->signal_size_allocate().connect(
        sigc::hide<0>(sigc::mem_fun(*this, 
                                    &ThumbStripView::on_visible_range_changed)));
}


void ThumbStripView::on_drag_data_get(const Glib::RefPtr<Gdk::DragContext>&,
                                      Gtk::SelectionData& /*data*/,guint,guint)
{
#if 0
	GList *list;
	GList *node;
	eng::LibFile *image;
	const char *str;
	gchar *uris = NULL;
	gchar *tmp_str;

	list = eog_thumb_view_get_selected_images (EOG_THUMB_VIEW (widget));

	for (node = list; node != NULL; node = node->next) {
		image = static_cast<eng::LibFile*>(node->data);
		str = image->uri().c_str();
		
		/* build the "text/uri-list" string */
		if (uris) {
			tmp_str = g_strconcat (uris, str, "\r\n", NULL);
			g_free (uris);
		} else {
			tmp_str = g_strconcat (str, "\r\n", NULL);
		}
		uris = tmp_str;
	}
	gtk_selection_data_set (data, data->target, 8, 
				(guchar*) uris, strlen (uris));
	g_free (uris);
	g_list_free (list);
#endif
}


void
ThumbStripView::set_item_height (int height)
{
    m_renderer->property_height() = height;
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
