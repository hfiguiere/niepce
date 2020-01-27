/* Eye Of Gnome - Thumbnail View
 *
 * Copyright (C) 2006 The Free Software Foundation
 * Copyright (C) 2007-2020 Hubert Figuière
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
enum class Target {
	PLAIN,
	PLAIN_UTF8,
	URILIST
};

#if 0
static GtkTargetEntry target_table[] = {
    { const_cast<gchar*>("text/uri-list"), 0, Target::URILIST },
};
#endif

class ThumbStripCell
    : public LibraryCellRenderer
{
public:
    ThumbStripCell(const GetColourFunc& get_colour);
};

ThumbStripCell::ThumbStripCell(const GetColourFunc& get_colour)
    : Glib::ObjectBase(typeid(ThumbStripCell))
    , LibraryCellRenderer(get_colour)
{
    set_pad(0);
    set_size(100);
    set_drawborder(false);
    set_drawemblem(false);
    set_drawrating(false);
    set_drawlabel(false);
    set_drawflag(false);
}

ThumbStripView::ThumbStripView(const Glib::RefPtr<ui::ImageListStore> & store,
                               const libraryclient::UIDataProviderWeakPtr& ui_data_provider)
    : Glib::ObjectBase(typeid(ThumbStripView))
    , Gtk::IconView(Glib::RefPtr<Gtk::TreeModel>::cast_dynamic(store))
    , property_item_height(*this, "item-height", 100)
    , m_store(store)
    , m_model_item_count(0)
{
    m_renderer = manage(
        new ThumbStripCell(
            [ui_data_provider] (int32_t label, ffi::RgbColour* out) {
                auto provider = ui_data_provider.lock();
                DBG_ASSERT(static_cast<bool>(provider), "couldn't lock UI provider");
                if (provider) {
                    auto c = provider->colourForLabel(label);
                    if (c.ok() && out) {
                        *out = c.unwrap();
                        return true;
                    }
                }
                return false;
            })
        );

    pack_start(*m_renderer, FALSE);
    connect_property_changed("item-height",
                             [this] () {
                                 m_renderer->property_height() = this->property_item_height;
                             });
    m_renderer->property_height() = 100;
    m_renderer->property_yalign() = 0.5;
    m_renderer->property_xalign() = 0.5;

    add_attribute(*m_renderer, "pixbuf",
                  ui::ImageListStore::Columns::STRIP_THUMB_INDEX);
    add_attribute(*m_renderer, "libfile",
                  ui::ImageListStore::Columns::FILE_INDEX);
    add_attribute(*m_renderer, "status",
                  ui::ImageListStore::Columns::FILE_STATUS_INDEX);
    set_selection_mode(Gtk::SELECTION_MULTIPLE);
    set_column_spacing(THUMB_STRIP_VIEW_SPACING);

    set_row_spacing (THUMB_STRIP_VIEW_SPACING);
    set_margin (0);

//  enable_model_drag_source (target_table, Gdk::MODIFIER_MASK, Gdk::ACTION_COPY);

//  signal_drag_data_get().connect(
//      sigc::mem_fun(*this, &ThumbStripView::on_drag_data_get));

    setup_model(store);
}

void ThumbStripView::set_model(const Glib::RefPtr<ui::ImageListStore> & store)
{
    m_store = store;
    setup_model(store);
    IconView::set_model(store);
}

void ThumbStripView::setup_model(const Glib::RefPtr<ui::ImageListStore> & store)
{
    m_model_add.disconnect();
    m_model_rm.disconnect();

    m_model_item_count = m_store->get_count();
    update_item_count();

    m_model_add = store->signal_row_inserted()
        .connect(sigc::mem_fun(*this, &ThumbStripView::row_added));
    m_model_rm = store->signal_row_deleted()
        .connect(sigc::mem_fun(*this, &ThumbStripView::row_deleted));
}

const Glib::RefPtr<ui::ImageListStore> & ThumbStripView::get_model() const
{
    return m_store;
}

void ThumbStripView::row_added(const Gtk::TreeModel::Path&,
                               const Gtk::TreeModel::iterator&)
{
    m_model_item_count++;
    update_item_count();
}

void ThumbStripView::row_deleted(const Gtk::TreeModel::Path&)
{
    if(m_model_item_count) {
        m_model_item_count--;
        update_item_count();
    }
}

void ThumbStripView::update_item_count()
{
    set_columns(m_model_item_count);
}

void ThumbStripView::on_drag_data_get(const Glib::RefPtr<Gdk::DragContext>&,
                                      Gtk::SelectionData& /*data*/,guint,guint)
{
#if 0
	GList *list;
	GList *node;
	eng::LibFile *image;
	const char *str;
	gchar *uris = nullptr;
	gchar *tmp_str;

	list = eog_thumb_view_get_selected_images (EOG_THUMB_VIEW (widget));

	for (node = list; node != nullptr; node = node->next) {
		image = static_cast<eng::LibFile*>(node->data);
		str = image->uri().c_str();
		
		/* build the "text/uri-list" string */
		if (uris) {
			tmp_str = g_strconcat (uris, str, "\r\n", nullptr);
			g_free (uris);
		} else {
			tmp_str = g_strconcat (str, "\r\n", nullptr);
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
    property_item_height = height;
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
