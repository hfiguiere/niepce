/* Eye Of Gnome - Thumbnail View
 *
 * Copyright (C) 2006 The Free Software Foundation
 * Copyright (C) 2007 Hubert Figuiere
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "eog-thumb-view.h"
//#include "eog-list-store.h"

#ifdef HAVE_EXIF
#include "eog-exif-util.h"
#include <libexif/exif-data.h>
#endif

#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <string.h>
#include <libgnomevfs/gnome-vfs-mime-utils.h>
#include <libgnomevfs/gnome-vfs-mime-handlers.h>

#include "utils/boost.h"
#include "db/libfile.h"

#define EOG_THUMB_VIEW_SPACING 0

#define EOG_THUMB_VIEW_GET_PRIVATE(object) \
	(G_TYPE_INSTANCE_GET_PRIVATE ((object), EOG_TYPE_THUMB_VIEW, EogThumbViewPrivate))

G_DEFINE_TYPE (EogThumbView, eog_thumb_view, GTK_TYPE_ICON_VIEW)

//static EogImage* eog_thumb_view_get_image_from_path (EogThumbView      *tb,
//						     GtkTreePath       *path);

static void      eog_thumb_view_popup_menu          (EogThumbView      *widget, 
						     GdkEventButton    *event);

struct _EogThumbViewPrivate {
	gint start_thumb; /* the first visible thumbnail */
	gint end_thumb;   /* the last visible thumbnail  */
	GtkWidget *menu;  /* a contextual menu for thumbnails */
	GtkCellRenderer *pixbuf_cell;
	Glib::RefPtr<ui::ImageListStore> store;
};

/* Drag 'n Drop */
enum {
	TARGET_PLAIN,
	TARGET_PLAIN_UTF8,
	TARGET_URILIST
};

static GtkTargetEntry target_table[] = {
	{ const_cast<gchar*>("text/uri-list"), 0, TARGET_URILIST },
};


static void
eog_thumb_view_finalize (GObject *object)
{
	EogThumbView *tb;
	g_return_if_fail (EOG_IS_THUMB_VIEW (object));
	tb = EOG_THUMB_VIEW (object);
	
	G_OBJECT_CLASS (eog_thumb_view_parent_class)->finalize (object);
}

static void
eog_thumb_view_destroy (GtkObject *object)
{
	EogThumbView *tb;
	g_return_if_fail (EOG_IS_THUMB_VIEW (object));
	tb = EOG_THUMB_VIEW (object);

	GTK_OBJECT_CLASS (eog_thumb_view_parent_class)->destroy (object);
}

static void
eog_thumb_view_class_init (EogThumbViewClass *klass)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
	GtkObjectClass *object_class = GTK_OBJECT_CLASS (klass);
	
	gobject_class->finalize = eog_thumb_view_finalize;
	object_class->destroy = eog_thumb_view_destroy;

	g_type_class_add_private (klass, sizeof (EogThumbViewPrivate));
}

static void
eog_thumb_view_clear_range (EogThumbView *tb, 
			    const gint start_thumb, 
			    const gint end_thumb)
{
	GtkTreePath *path;
	GtkTreeIter iter;
	Glib::RefPtr<ui::ImageListStore> store = eog_thumb_view_get_model(tb);
	gint thumb = start_thumb;
	gboolean result;
	
	g_assert (start_thumb <= end_thumb);
	
	path = gtk_tree_path_new_from_indices (start_thumb, -1);
	for (result = gtk_tree_model_get_iter (GTK_TREE_MODEL (store->gobj()), 
										   &iter, path);
	     result && thumb <= end_thumb;
	     result = gtk_tree_model_iter_next (GTK_TREE_MODEL (store->gobj()), 
											&iter), thumb++) {
//		eog_list_store_thumbnail_unset (get_pointer(store), &iter);
	}
	gtk_tree_path_free (path);
}

static void
eog_thumb_view_add_range (EogThumbView *tb, 
			  const gint start_thumb, 
			  const gint end_thumb)
{
	GtkTreePath *path;
	GtkTreeIter iter;
	Glib::RefPtr<ui::ImageListStore> store = eog_thumb_view_get_model(tb);
	gint thumb = start_thumb;
	gboolean result;
	
	g_assert (start_thumb <= end_thumb);
	
	path = gtk_tree_path_new_from_indices (start_thumb, -1);
	for (result = gtk_tree_model_get_iter (GTK_TREE_MODEL (store->gobj()), &iter, path);
	     result && thumb <= end_thumb;
	     result = gtk_tree_model_iter_next (GTK_TREE_MODEL (store->gobj()), &iter), thumb++) {
//		eog_list_store_thumbnail_set (get_pointer(store), &iter);
	}
	gtk_tree_path_free (path);
}

static void
eog_thumb_view_update_visible_range (EogThumbView *tb, 
				     const gint start_thumb, 
				     const gint end_thumb)
{
	EogThumbViewPrivate *priv = tb->priv;
	int old_start_thumb, old_end_thumb;

	old_start_thumb= priv->start_thumb;
	old_end_thumb = priv->end_thumb;
	
	if (start_thumb == old_start_thumb &&
	    end_thumb == old_end_thumb) {
		return;
	}
	
	if (old_start_thumb < start_thumb)
		eog_thumb_view_clear_range (tb, old_start_thumb, MIN (start_thumb - 1, old_end_thumb));

	if (old_end_thumb > end_thumb)
		eog_thumb_view_clear_range (tb, MAX (end_thumb + 1, old_start_thumb), old_end_thumb);

	eog_thumb_view_add_range (tb, start_thumb, end_thumb);
	
	priv->start_thumb = start_thumb;
	priv->end_thumb = end_thumb;
}

static void
tb_on_visible_range_changed_cb (EogThumbView *tb,
                                gpointer /*user_data*/)
{
	GtkTreePath *path1, *path2;

	if (!gtk_icon_view_get_visible_range (GTK_ICON_VIEW (tb), &path1, &path2)) {
		return;
	}
	
	if (path1 == NULL) {
		path1 = gtk_tree_path_new_first ();
	}
	if (path2 == NULL) {
		gint n_items = gtk_tree_model_iter_n_children (gtk_icon_view_get_model (GTK_ICON_VIEW (tb)), NULL);
		path2 = gtk_tree_path_new_from_indices (n_items - 1 , -1);
	}
	
	eog_thumb_view_update_visible_range (tb, gtk_tree_path_get_indices (path1) [0],
					     gtk_tree_path_get_indices (path2) [0]);
	
	gtk_tree_path_free (path1);
	gtk_tree_path_free (path2);
}

static void
tb_on_adjustment_changed_cb (EogThumbView *tb,
							 gpointer /*user_data*/)
{
	GtkTreePath *path1, *path2;
	gint start_thumb, end_thumb;
	
	if (!gtk_icon_view_get_visible_range (GTK_ICON_VIEW (tb), &path1, &path2)) {
		return;
	}
	
	if (path1 == NULL) {
		path1 = gtk_tree_path_new_first ();
	}
	if (path2 == NULL) {
		gint n_items = gtk_tree_model_iter_n_children (gtk_icon_view_get_model (GTK_ICON_VIEW (tb)), NULL);
		path2 = gtk_tree_path_new_from_indices (n_items - 1 , -1);
	}

	start_thumb = gtk_tree_path_get_indices (path1) [0];
	end_thumb = gtk_tree_path_get_indices (path2) [0];

	eog_thumb_view_add_range (tb, start_thumb, end_thumb);	

	/* case we added an image, we need to make sure that the shifted thumbnail is cleared */
	eog_thumb_view_clear_range (tb, end_thumb + 1, end_thumb + 1);

	tb->priv->start_thumb = start_thumb;
	tb->priv->end_thumb = end_thumb;

	gtk_tree_path_free (path1);
	gtk_tree_path_free (path2);
}

static void
tb_on_parent_set_cb (GtkWidget *widget,
					 GtkObject * /*old_parent*/,
					 gpointer  /*user_data*/)
{
	EogThumbView *tb = EOG_THUMB_VIEW (widget);
	GtkScrolledWindow *sw;
	GtkAdjustment *hadjustment;
	GtkAdjustment *vadjustment;

	GtkWidget *parent = gtk_widget_get_parent (GTK_WIDGET (tb));
	if (!GTK_IS_SCROLLED_WINDOW (parent)) {
		return;
	}
	
	/* if we have been set to a ScrolledWindow, we connect to the callback
	to set and unset thumbnails. */
	sw = GTK_SCROLLED_WINDOW (parent);
	hadjustment = gtk_scrolled_window_get_hadjustment (GTK_SCROLLED_WINDOW (sw));
	vadjustment = gtk_scrolled_window_get_vadjustment (GTK_SCROLLED_WINDOW (sw));
	
	/* when scrolling */
	g_signal_connect_data (G_OBJECT (hadjustment), "value-changed",
			       G_CALLBACK (tb_on_visible_range_changed_cb),
			       tb, NULL, (GConnectFlags)(G_CONNECT_SWAPPED | G_CONNECT_AFTER));
	g_signal_connect_data (G_OBJECT (vadjustment), "value-changed",
			       G_CALLBACK (tb_on_visible_range_changed_cb),
			       tb, NULL, (GConnectFlags)(G_CONNECT_SWAPPED | G_CONNECT_AFTER));
	
	/* when the adjustment is changed, ie. probably we have new images added. */
	g_signal_connect_data (G_OBJECT (hadjustment), "changed",
			       G_CALLBACK (tb_on_adjustment_changed_cb),
			       tb, NULL, (GConnectFlags)(G_CONNECT_SWAPPED | G_CONNECT_AFTER));
	g_signal_connect_data (G_OBJECT (vadjustment), "changed",
			       G_CALLBACK (tb_on_adjustment_changed_cb),
			       tb, NULL, (GConnectFlags)(G_CONNECT_SWAPPED | G_CONNECT_AFTER));

	/* when resizing the scrolled window */
	g_signal_connect_swapped (G_OBJECT (sw), "size-allocate",
				  G_CALLBACK (tb_on_visible_range_changed_cb),
				  tb);
}

#if 0
static gboolean
tb_on_button_press_event_cb (GtkWidget *tb, GdkEventButton *event, 
                             gpointer /*user_data*/)
{
    GtkTreePath *path;
    
    /* Ignore double-clicks and triple-clicks */
    if (event->button == 3 && event->type == GDK_BUTTON_PRESS)
    {
	    path = gtk_icon_view_get_path_at_pos (GTK_ICON_VIEW (tb), 
						  (gint) event->x, (gint) event->y);
	    if (path == NULL) {
		    return FALSE;
	    }

	    gtk_icon_view_unselect_all (GTK_ICON_VIEW (tb));
	    gtk_icon_view_select_path (GTK_ICON_VIEW (tb), path);

	    eog_thumb_view_popup_menu (EOG_THUMB_VIEW (tb), event);
	    
	    gtk_tree_path_free (path);

	    return TRUE;
    }
    
    return FALSE;
}
#endif

static void
tb_on_drag_data_get_cb (GtkWidget        *widget,
						GdkDragContext   */*drag_context*/,
						GtkSelectionData *data,
						guint             /*info*/,
						guint             /*time*/,
						gpointer          /*user_data*/) 
{
	GList *list;
	GList *node;
	db::LibFile *image;
	const char *str;
	gchar *uris = NULL;
	gchar *tmp_str;

	list = eog_thumb_view_get_selected_images (EOG_THUMB_VIEW (widget));

	for (node = list; node != NULL; node = node->next) {
		image = static_cast<db::LibFile*>(node->data);
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
	
}

#ifdef HAVE_GTK_TOOLTIP
static gboolean 
tb_on_query_tooltip_cb (GtkWidget  *widget,
			gint        x,
			gint        y,
			gboolean    keyboard_mode,
			GtkTooltip *tooltip,
			gpointer    user_data)
{
	GtkTreePath *path;
	EogImage *image;
	gchar *tooltip_string;
	gchar *bytes;
	gint width, height;
	gchar *uri_str, *mime_str;
	const gchar *type_str;
	GError *error = NULL;
#ifdef HAVE_EXIF
	ExifData *exif_data;
#endif	

	if (!gtk_icon_view_get_tooltip_context (GTK_ICON_VIEW (widget), 
						&x, &y, keyboard_mode,
						NULL, &path, NULL)) {
		return FALSE;
	}
	
	image = eog_thumb_view_get_image_from_path (EOG_THUMB_VIEW (widget),
						    path);
	gtk_tree_path_free (path);

	if (image == NULL) {
		return FALSE;
	}

	if (!eog_image_has_data (image, EOG_IMAGE_DATA_EXIF)) {
		eog_image_load (image, EOG_IMAGE_DATA_EXIF, NULL, &error);

		if (error) {
			/* Here, error typically means no exif data found */
			g_error_free (error);
			error = NULL;
		}
	}

#ifdef HAVE_EXIF
	exif_data = (ExifData *) eog_image_get_exif_info (image);		
#endif

	if (!eog_image_has_data (image, EOG_IMAGE_DATA_DIMENSION)) {
		eog_image_load (image,
				EOG_IMAGE_DATA_DIMENSION,
				NULL, 
				&error);
	}

	bytes = gnome_vfs_format_file_size_for_display (eog_image_get_bytes (image));

	eog_image_get_size (image, &width, &height);
	
	uri_str = eog_image_get_uri_for_display (image);

	mime_str = gnome_vfs_get_mime_type (uri_str);
	type_str = gnome_vfs_mime_get_description (mime_str);

	tooltip_string = g_strdup_printf ("<b><big>%s</big></b>\n"
					  "%i x %i %s\n"
					  "%s\n"
					  "%s",
					  eog_image_get_caption (image),
					  width, 
					  height, 
					  ngettext ("pixel", "pixels", height), 
					  bytes,
					  type_str);

#ifdef HAVE_EXIF
	if (exif_data) {
		gchar *extra_info, *tmp, *date;
		
		date = eog_exif_util_format_date (
				eog_exif_util_get_value (exif_data, EXIF_TAG_DATE_TIME));

		extra_info = g_strdup_printf ("\n%s %s", _("Taken on"), date);

		tmp = g_strconcat (tooltip_string, extra_info, NULL);

		g_free (date);
		g_free (extra_info);
		g_free (tooltip_string);

		tooltip_string = tmp;
	}
#endif

	gtk_tooltip_set_markup (tooltip, tooltip_string);
	
	g_free (uri_str);
	g_free (mime_str);
	g_free (bytes);
 	g_free (tooltip_string);
	g_object_unref (image);
	
	return TRUE;
}
#endif /* HAVE_GTK_TOOLTIP */

static void
eog_thumb_view_init (EogThumbView *tb)
{
	tb->priv = EOG_THUMB_VIEW_GET_PRIVATE (tb);
	
	tb->priv->pixbuf_cell = gtk_cell_renderer_pixbuf_new ();

	gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (tb), 
	      	  		    tb->priv->pixbuf_cell, 
	      			    FALSE);
	
	g_object_set (tb->priv->pixbuf_cell, 
	              "follow-state", TRUE, 
	              "height", 100, 
	              "yalign", 0.5, 
	              "xalign", 0.5, 
	              NULL);
	
	gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT (tb),
	                                tb->priv->pixbuf_cell, 
									"pixbuf", ui::ImageListStore::Columns::STRIP_THUMB_INDEX,
	                                NULL);

	gtk_icon_view_set_selection_mode (GTK_ICON_VIEW (tb),
 					  GTK_SELECTION_MULTIPLE);
	
	gtk_icon_view_set_column_spacing (GTK_ICON_VIEW (tb),
					  EOG_THUMB_VIEW_SPACING);

	gtk_icon_view_set_row_spacing (GTK_ICON_VIEW (tb),
				       EOG_THUMB_VIEW_SPACING);

#ifdef HAVE_GTK_TOOLTIP
	g_object_set (tb, "has-tooltip", TRUE, NULL);

	g_signal_connect (tb, 
			  "query-tooltip",
			  G_CALLBACK (tb_on_query_tooltip_cb), 
			  NULL);
#endif /* HAVE_GTK_TOOLTIP */	
	
	tb->priv->start_thumb = 0;
	tb->priv->end_thumb = 0;
	tb->priv->menu = NULL;
	
	g_signal_connect (G_OBJECT (tb), "parent-set", 
			  G_CALLBACK (tb_on_parent_set_cb), NULL);

	gtk_icon_view_enable_model_drag_source (GTK_ICON_VIEW (tb), (GdkModifierType)0,
						target_table, G_N_ELEMENTS (target_table), 
						(GdkDragAction)GDK_ACTION_COPY);

	g_signal_connect (G_OBJECT (tb), "drag-data-get", 
			  G_CALLBACK (tb_on_drag_data_get_cb), NULL);
}

GtkWidget *
eog_thumb_view_new (const Glib::RefPtr<ui::ImageListStore> & store)
{
	EogThumbView *tb;	

	tb = (EogThumbView *)g_object_new (EOG_TYPE_THUMB_VIEW, NULL);
	gtk_icon_view_set_model (GTK_ICON_VIEW (tb), GTK_TREE_MODEL(store->gobj()));
	tb->priv->store = store;

	return GTK_WIDGET (tb);
}

void
eog_thumb_view_set_model (EogThumbView * tb, 
						  const Glib::RefPtr<ui::ImageListStore> & store)
{
//	gint index;
	g_return_if_fail (EOG_IS_THUMB_VIEW (tb));
	
	tb->priv->store = store;

//	index = eog_list_store_get_initial_pos (get_pointer(store));

	gtk_icon_view_set_model (GTK_ICON_VIEW (tb), GTK_TREE_MODEL(store->gobj()));

//	if (index >= 0) {
//		GtkTreePath *path = gtk_tree_path_new_from_indices (index, -1);
//		gtk_icon_view_select_path (GTK_ICON_VIEW (tb), path);
//		gtk_icon_view_scroll_to_path (GTK_ICON_VIEW (tb), path, FALSE, 0, 0);
//		gtk_tree_path_free (path);
//	}
}

Glib::RefPtr<ui::ImageListStore> eog_thumb_view_get_model    (EogThumbView *view)
{
	g_return_val_if_fail (EOG_IS_THUMB_VIEW (view), 
						  Glib::RefPtr<ui::ImageListStore>());
	return view->priv->store;
}

void
eog_thumb_view_set_item_height (EogThumbView *tb, gint height)
{
	g_return_if_fail (EOG_IS_THUMB_VIEW (tb));

	g_object_set (tb->priv->pixbuf_cell, 
	              "height", height, 
	              NULL);
}

static void
eog_thumb_view_get_n_selected_helper (GtkIconView * /*tb*/,
									  GtkTreePath * /*path*/,
				      gpointer data)
{
	/* data is of type (guint *) */
	(*(guint *) data) ++;
}

guint
eog_thumb_view_get_n_selected (EogThumbView *tb)
{
	guint count = 0;
	gtk_icon_view_selected_foreach (GTK_ICON_VIEW (tb),
					eog_thumb_view_get_n_selected_helper,
					(&count));
	return count;
}

#if 0
static EogImage *
eog_thumb_view_get_image_from_path (EogThumbView *tb, GtkTreePath *path)
{
	GtkTreeModel *model;
	GtkTreeIter iter;
	EogImage *image;

	model = gtk_icon_view_get_model (GTK_ICON_VIEW (tb));
	gtk_tree_model_get_iter (model, &iter, path);

	gtk_tree_model_get (model, &iter,
			    EOG_LIST_STORE_EOG_IMAGE, &image,
			    -1);
			    
	return image;
}

EogImage *
eog_thumb_view_get_first_selected_image (EogThumbView *tb)
{
	/* The returned list is not sorted! We need to find the 
	   smaller tree path value => tricky and expensive. Do we really need this?
	*/
	GtkTreePath *path;
	EogImage *image;
	gchar *text_path;
	GList *list = gtk_icon_view_get_selected_items (GTK_ICON_VIEW (tb));

	if (list == NULL) {
		return NULL;
	}

	path = (GtkTreePath *) (list->data);

	/* debugging purposes */
	image = eog_thumb_view_get_image_from_path (tb, path);
	text_path = gtk_tree_path_to_string (path);
	g_free (text_path);

	g_list_foreach (list, (GFunc) gtk_tree_path_free , NULL);
	g_list_free (list);

	return image;
}
#endif

GList *
eog_thumb_view_get_selected_images (EogThumbView *tb)
{
	GList *l, *item;
	GList *list = NULL;

	GtkTreePath *path;

	l = gtk_icon_view_get_selected_items (GTK_ICON_VIEW (tb));

	for (item = l; item != NULL; item = item->next) {
		path = (GtkTreePath *) item->data;
// FIXME
//		list = g_list_prepend (list, eog_thumb_view_get_image_from_path (tb, path));
		gtk_tree_path_free (path);
	}

	g_list_free (l);
	list = g_list_reverse (list);

	return list;
}

#if 0
void
eog_thumb_view_set_current_image (EogThumbView *tb, 
								  const db::LibFile::Ptr &image,
								  gboolean deselect_other)
{
	GtkTreePath *path;
	Glib::RefPtr<ui::ImageListStore> store;
	gint pos;

	store = eog_thumb_view_get_model(tb);
	pos = eog_list_store_get_pos_by_image (get_pointer(store), image);
	path = gtk_tree_path_new_from_indices (pos, -1);

	if (path == NULL) {
		return;
	}

	if (deselect_other) {
		gtk_icon_view_unselect_all (GTK_ICON_VIEW (tb));
	}
	
	gtk_icon_view_select_path (GTK_ICON_VIEW (tb), path);
	gtk_icon_view_scroll_to_path (GTK_ICON_VIEW (tb), path, FALSE, 0, 0);

	gtk_tree_path_free (path);
}

void
eog_thumb_view_select_single (EogThumbView *tb, 
			      EogThumbViewSelectionChange change)
{
	GtkTreePath *path = NULL;
	GList *list;
	gint n_items;

	g_return_if_fail (EOG_IS_THUMB_VIEW (tb));
	Glib::RefPtr<ui::ImageListStore> store = eog_thumb_view_get_model (tb);

	n_items = eog_list_store_length (get_pointer(store));

	if (n_items == 0) {
		return;
	}
	
	if (eog_thumb_view_get_n_selected (tb) == 0) {
		switch (change) {
		case EOG_THUMB_VIEW_SELECT_RIGHT:
		case EOG_THUMB_VIEW_SELECT_FIRST:
			path = gtk_tree_path_new_first ();
			break;
		case EOG_THUMB_VIEW_SELECT_LEFT:
		case EOG_THUMB_VIEW_SELECT_LAST:
			path = gtk_tree_path_new_from_indices (n_items - 1, -1);
		}
	} else {
		list = gtk_icon_view_get_selected_items (GTK_ICON_VIEW (tb));
		path = gtk_tree_path_copy ((GtkTreePath *) list->data);
		g_list_foreach (list, (GFunc) gtk_tree_path_free , NULL);
		g_list_free (list);
		
		gtk_icon_view_unselect_all (GTK_ICON_VIEW (tb));
		
		switch (change) {
		case EOG_THUMB_VIEW_SELECT_LEFT:
			if (!gtk_tree_path_prev (path)) {
				gtk_tree_path_free (path);
				path = gtk_tree_path_new_from_indices (n_items - 1, -1);
			}
			break;
		case EOG_THUMB_VIEW_SELECT_RIGHT:
			if (gtk_tree_path_get_indices (path) [0] == n_items - 1) {
				gtk_tree_path_free (path);
				path = gtk_tree_path_new_first ();
			} else {
				gtk_tree_path_next (path);
			}
			break;
		case EOG_THUMB_VIEW_SELECT_FIRST:
			gtk_tree_path_free (path);
			path = gtk_tree_path_new_first ();
			break;
		case EOG_THUMB_VIEW_SELECT_LAST:
			gtk_tree_path_free (path);
			path = gtk_tree_path_new_from_indices (n_items - 1, -1);
		}
	}

	gtk_icon_view_select_path (GTK_ICON_VIEW (tb), path);
	gtk_icon_view_scroll_to_path (GTK_ICON_VIEW (tb), path, FALSE, 0, 0);
	gtk_tree_path_free (path);
}


void
eog_thumb_view_set_thumbnail_popup (EogThumbView *tb,
				    GtkMenu      *menu)
{
	g_return_if_fail (EOG_IS_THUMB_VIEW (tb));
	g_return_if_fail (tb->priv->menu == NULL);

	tb->priv->menu = (GtkWidget*)g_object_ref (menu);

	gtk_menu_attach_to_widget (GTK_MENU (tb->priv->menu), 
				   GTK_WIDGET (tb), 
				   NULL);

	g_signal_connect (G_OBJECT (tb), "button_press_event",
			  G_CALLBACK (tb_on_button_press_event_cb), NULL);
	
}
#endif


static void 
eog_thumb_view_popup_menu (EogThumbView *tb, GdkEventButton *event)
{
	GtkWidget *popup;
	int button, event_time;

	popup = tb->priv->menu;
	
	if (event) {
		button = event->button;
		event_time = event->time;
	} else {
		button = 0;
		event_time = gtk_get_current_event_time ();
	}
	
	gtk_menu_popup (GTK_MENU (popup), NULL, NULL, NULL, NULL, 
			button, event_time);
}
