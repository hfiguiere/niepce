/* Eye Of Gnome - Image Store
 *
 * Copyright (C) 2006 The Free Software Foundation
 * Copyright (c) 2007 Hubert Figuiere
 * 
 * Author: Hubert Figuiere <hub@figuiere.net>
 *
 * Original Author: Claudio Saavedra <csaavedra@alumnos.utalca.cl>
 * Based on code by: Jens Finke <jens@triq.net>
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

#include <boost/bind.hpp>

#include "eog-list-store.h"

#include <string.h>
#include <strings.h>

#include <gtkmm/icontheme.h>
#include <gtkmm/treeiter.h>

#include "library/libfile.h"


//#define EOG_LIST_STORE_GET_PRIVATE(object) 
//	(G_TYPE_INSTANCE_GET_PRIVATE ((object), EOG_TYPE_LIST_STORE, EogListStorePrivate))

//G_DEFINE_TYPE (EogListStore, eog_list_store, GTK_TYPE_LIST_STORE)

#if 0
typedef struct {
	EogListStore *store;
	GnomeVFSURI *uri;
	GnomeVFSFileInfo *info;
} DirLoadingContext;

typedef struct {
	GnomeVFSMonitorHandle *handle;
	const gchar *text_uri;
} MonitorHandleContext;
#endif

struct _EogListStorePrivate {
	GList *monitors;       /* Monitors for the directories */
	gint initial_image;    /* The image that should be selected firstly by the view. */
	GdkPixbuf *busy_image; /* Hourglass image */
	GMutex *mutex;         /* Mutex for saving the jobs in the model */
};


#if 0
static void
foreach_monitors_free (gpointer data, gpointer user_data)
{
	MonitorHandleContext *hctx = (MonitorHandleContext *)data;
	
	gnome_vfs_monitor_cancel (hctx->handle);
	
	g_free (data);
}
#endif

#if 0
static void
eog_list_store_dispose (GObject *object)
{
	EogListStore *store = EOG_LIST_STORE (object);

	g_list_foreach (store->priv->monitors, 
			foreach_monitors_free, NULL);

	g_list_free (store->priv->monitors);

	store->priv->monitors = NULL;

	if(store->priv->busy_image != NULL) {
		g_object_unref (store->priv->busy_image);
		store->priv->busy_image = NULL;
	}

	g_mutex_free (store->priv->mutex);

	G_OBJECT_CLASS (eog_list_store_parent_class)->dispose (object);
}
#endif


/*
   Sorting functions 
*/

static gint
eog_list_store_compare_func (GtkTreeModel *model,
			     GtkTreeIter *a,
			     GtkTreeIter *b,
			     gpointer user_data)
{
	gint r_value;

	const library::LibFile *image_a;
	const library::LibFile *image_b;

	gtk_tree_model_get (model, a, 
			    EOG_LIST_STORE_EOG_IMAGE, &image_a,
			    -1);

	gtk_tree_model_get (model, b, 
			    EOG_LIST_STORE_EOG_IMAGE, &image_b,
			    -1);
	
	r_value = strcasecmp (image_a->name().c_str(), 
			      image_b->name().c_str());

	return r_value;
}




EogListStore::EogListStore()
	: Gtk::ListStore()
{
	_init();
}


EogListStore::EogListStore(const library::LibFile::List &list)
	: Gtk::ListStore()
{
	_init();

	std::for_each(list.begin(), list.end(),
								boost::bind(&EogListStore::append_image,
														this, _1));
}


void
EogListStore::_init()
{
	set_column_types(m_columns);
/*
	self->priv = EOG_LIST_STORE_GET_PRIVATE (self);

	self->priv->monitors = NULL;
	self->priv->initial_image = -1;

	self->priv->busy_image = eog_list_store_get_loading_icon ();

	self->priv->mutex = g_mutex_new ();
*/
	gtk_tree_sortable_set_default_sort_func (GTK_TREE_SORTABLE (this),
						 eog_list_store_compare_func,
						 NULL, NULL);
	
	gtk_tree_sortable_set_sort_column_id (GTK_TREE_SORTABLE (this), 
					      GTK_TREE_SORTABLE_DEFAULT_SORT_COLUMN_ID, 
					      GTK_SORT_ASCENDING);
}


EogListStore::~EogListStore()
{
}


Glib::RefPtr<Gdk::Pixbuf>
EogListStore::get_loading_icon (void)
{
	if(!m_loading_icon) {
		Glib::RefPtr<Gtk::IconTheme> icon_theme = Gtk::IconTheme::get_default();
		
		/* FIXME: The 16 added to EOG_LIST_STORE_THUMB_SIZE should be 
			 calculated from the BLUR_RADIUS and RECTANGLE_OUTLINE macros 
			 in eog-thumb-shadow.c */
		m_loading_icon = icon_theme->load_icon("image-loading", /* icon name */
																					 EOG_LIST_STORE_THUMB_SIZE + 16, /* size */
																					 (Gtk::IconLookupFlags)0 /* flags */);
	}
	return m_loading_icon;
}


/**
   Searchs for a file in the store. If found and @iter_found is not NULL,
   then sets @iter_found to a #GtkTreeIter pointing to the file.
 */
gboolean
EogListStore::is_file_in_list_store (const gchar *info_uri,
																		 Gtk::TreeIter &iter_found)
{
	gboolean found = FALSE;
	library::LibFile::Ptr image;

	Gtk::TreeRow iter;

//	iter = std::find_if(children().begin(), children().end(),
//											boost::bind(&library::LibFile::isUri,
//																	boost::bind(&Gtk::TreeRow::get_value, _1, 
//																							m_columns.m_image)
//																	info_uri));
	found = (children().end() != iter);
	iter_found = iter;
	return found;
}

#if 0
static void
eog_job_thumbnail_cb (EogJobThumbnail *job, gpointer data)
{
	EogListStore *store;
	Gtk::TreeModel::iterator iter;
	gchar *filename;
	EogImage *image;
	
	g_return_if_fail (EOG_IS_LIST_STORE (data));

	store = EOG_LIST_STORE (data);

/* 	thumbnail = g_object_ref (job->thumbnail); */
	
	filename = gnome_vfs_uri_to_string (job->uri_entry, GNOME_VFS_URI_HIDE_NONE);

	if (is_file_in_list_store (store, filename, &iter)) {
		gtk_tree_model_get (GTK_TREE_MODEL (store), &iter, 
				    EOG_LIST_STORE_EOG_IMAGE, &image,
				    -1);

		eog_image_set_thumbnail (image, job->thumbnail);

		gtk_list_store_set (GTK_LIST_STORE (store), &iter, 
				    EOG_LIST_STORE_THUMBNAIL, job->thumbnail,
				    EOG_LIST_STORE_THUMB_SET, TRUE,
				    EOG_LIST_STORE_EOG_JOB, NULL,
				    -1);
/* 		g_object_unref (thumbnail); */
	}

	g_free (filename);
}
#endif

void
EogListStore::append_image (const library::LibFile::Ptr &image)
{
	Gtk::TreeModel::iterator iter = append();

	Gtk::TreeModel::Row row = *iter;

	row[m_columns.m_image] = image;
	row[m_columns.m_thumb_set] = false;
	row[m_columns.m_thumbnail] = get_loading_icon();
}

#if 0
void 
eog_list_store_append_image_from_uri (EogListStore *store, GnomeVFSURI *uri_entry)
{
	EogImage *image;
	
	g_return_if_fail (EOG_IS_LIST_STORE (store));

	image = eog_image_new_uri (uri_entry);

	eog_list_store_append_image (store, image);
}


static void
vfs_monitor_dir_cb (GnomeVFSMonitorHandle *handle,
		    const gchar *monitor_uri,
		    const gchar *info_uri,
		    GnomeVFSMonitorEventType event_type,
		    gpointer user_data)
{
	EogListStore *store = EOG_LIST_STORE (user_data);
	GnomeVFSURI *uri = NULL;
	GtkTreeIter iter;
	gchar *mimetype;

	switch (event_type) {
	case GNOME_VFS_MONITOR_EVENT_CHANGED:
		mimetype = gnome_vfs_get_mime_type (info_uri);

		if (is_file_in_list_store (store, info_uri, &iter)) {
			if (eog_image_is_supported_mime_type (mimetype)) {
				eog_list_store_thumbnail_unset (store, &iter);
				eog_list_store_thumbnail_set (store, &iter);
			} else {
				gtk_list_store_remove (GTK_LIST_STORE (store), &iter);
			}
		} else {
			if (eog_image_is_supported_mime_type (mimetype)) {
				uri = gnome_vfs_uri_new (info_uri);
				eog_list_store_append_image_from_uri (store, uri);
				gnome_vfs_uri_unref (uri);
			}
		}

		g_free (mimetype);
		break;

	case GNOME_VFS_MONITOR_EVENT_DELETED:
		if (is_file_in_list_store (store, info_uri, &iter)) {
			gtk_list_store_remove (GTK_LIST_STORE (store), &iter);
		}
		break;

	case GNOME_VFS_MONITOR_EVENT_CREATED:
		if (is_file_in_list_store (store, info_uri, NULL)) {
			uri = gnome_vfs_uri_new (info_uri);
			eog_list_store_append_image_from_uri (store, uri);
			gnome_vfs_uri_unref (uri);
		}
		break;

	case GNOME_VFS_MONITOR_EVENT_METADATA_CHANGED:
	case GNOME_VFS_MONITOR_EVENT_STARTEXECUTING:
	case GNOME_VFS_MONITOR_EVENT_STOPEXECUTING:
		break;
	}
}

/* 
 * Called for each file in a directory. Checks if the file is some
 * sort of image. If so, it creates an image object and adds it to the
 * list.
 */
static gboolean
directory_visit_cb (const gchar *rel_uri,
		    GnomeVFSFileInfo *info,
		    gboolean recursing_will_loop,
		    gpointer data,
		    gboolean *recurse)
{
	GnomeVFSURI *uri;
	EogListStore *store;
	gboolean load_uri = FALSE;
	DirLoadingContext *ctx;
	
	ctx = (DirLoadingContext*) data;
	store = ctx->store;
	
        if ((info->valid_fields & GNOME_VFS_FILE_INFO_FIELDS_MIME_TYPE) > 0 &&
            !g_str_has_prefix (info->name, ".")) {
		if (eog_image_is_supported_mime_type (info->mime_type)) {
			load_uri = TRUE;
		}
	}

	if (load_uri) {
		uri = gnome_vfs_uri_append_file_name (ctx->uri, rel_uri);
		eog_list_store_append_image_from_uri (store, uri);
	}

	return TRUE;
}

static void 
eog_list_store_append_directory (EogListStore *store, 
				 GnomeVFSURI *uri, 
				 GnomeVFSFileInfo *info)
{
	DirLoadingContext ctx;
	MonitorHandleContext *hctx = g_new0(MonitorHandleContext, 1);

	hctx->text_uri = gnome_vfs_uri_get_path (uri);
	
	g_return_if_fail (info->type == GNOME_VFS_FILE_TYPE_DIRECTORY);

	ctx.uri = uri;
	ctx.store = store;
	ctx.info = info;
	
	gnome_vfs_monitor_add  (&hctx->handle, hctx->text_uri,
				GNOME_VFS_MONITOR_DIRECTORY,
				vfs_monitor_dir_cb,
				store);

	/* prepend seems more efficient to me, we don't need this list
	   to be sorted */
	store->priv->monitors = g_list_prepend (store->priv->monitors, hctx);
	
	/* Forcing slow MIME type checking, so we don't need to make 
	   workarounds for files with strange extensions (#333551) */
	gnome_vfs_directory_visit_uri (uri, 
				       (GnomeVFSFileInfoOptions)(GNOME_VFS_FILE_INFO_DEFAULT |
				       GNOME_VFS_FILE_INFO_FOLLOW_LINKS |
				       GNOME_VFS_FILE_INFO_GET_MIME_TYPE | 
				       GNOME_VFS_FILE_INFO_FORCE_SLOW_MIME_TYPE),
				       (GnomeVFSDirectoryVisitOptions)
																 GNOME_VFS_DIRECTORY_VISIT_DEFAULT,
				       directory_visit_cb,
				       &ctx);
}

static gboolean
get_uri_info (GnomeVFSURI *uri, GnomeVFSFileInfo *info)
{
	GnomeVFSResult result;
	
	g_return_val_if_fail (uri != NULL, FALSE);
	g_return_val_if_fail (info != NULL, FALSE);
	
	gnome_vfs_file_info_clear (info);

	result = gnome_vfs_get_file_info_uri (uri, info,
					      (GnomeVFSFileInfoOptions)(GNOME_VFS_FILE_INFO_DEFAULT |
					      GNOME_VFS_FILE_INFO_FOLLOW_LINKS |
					      GNOME_VFS_FILE_INFO_GET_MIME_TYPE));
	
	return (result == GNOME_VFS_OK);
}

void
eog_list_store_add_uris (EogListStore *store, GList *uri_list) 
{
	GList *it;
	GnomeVFSFileInfo *info;
	GnomeVFSURI *initial_uri = NULL;
	GtkTreeIter iter;

	if (uri_list == NULL) {
		return;
	}
	
	info = gnome_vfs_file_info_new ();

	gtk_tree_sortable_set_sort_column_id (GTK_TREE_SORTABLE (store),
					      -1, GTK_SORT_ASCENDING);
	
	for (it = uri_list; it != NULL; it = it->next) {
		GnomeVFSURI *uri = (GnomeVFSURI *) it->data;

		if (!get_uri_info (uri, info))
			continue;
			
		if (info->type == GNOME_VFS_FILE_TYPE_DIRECTORY) {
			eog_list_store_append_directory (store, uri, info);
		} else if (info->type == GNOME_VFS_FILE_TYPE_REGULAR && 
			   g_list_length (uri_list) == 1) {

			initial_uri = gnome_vfs_uri_dup (uri); 

			uri = gnome_vfs_uri_get_parent (uri);
			
			if (!get_uri_info (uri, info))
				continue;

			if (info->type == GNOME_VFS_FILE_TYPE_DIRECTORY) {
				eog_list_store_append_directory (store, uri, info);

				if (!is_file_in_list_store (store, 
							    gnome_vfs_uri_to_string (initial_uri, 
										     GNOME_VFS_URI_HIDE_NONE), 
							    &iter)) {
					eog_list_store_append_image_from_uri (store, initial_uri);
				}
			} else {
				eog_list_store_append_image_from_uri (store, initial_uri);
			}
		} else if (info->type == GNOME_VFS_FILE_TYPE_REGULAR && 
			   g_list_length (uri_list) > 1) {
			eog_list_store_append_image_from_uri (store, uri);
		}
	}

	gnome_vfs_file_info_unref (info);

	gtk_tree_sortable_set_sort_column_id (GTK_TREE_SORTABLE (store),
					      GTK_TREE_SORTABLE_DEFAULT_SORT_COLUMN_ID, 
					      GTK_SORT_ASCENDING);
	
	if (initial_uri && 
	    is_file_in_list_store (store, 
				   gnome_vfs_uri_to_string (initial_uri, GNOME_VFS_URI_HIDE_NONE), 
				   &iter)) {
		store->priv->initial_image = eog_list_store_get_pos_by_iter (store, &iter);
		gnome_vfs_uri_unref (initial_uri);
	} else {
		store->priv->initial_image = 0;
	} 
}
#endif

void
EogListStore::remove_image(const library::LibFile::Ptr &image)
{
	Gtk::TreeModel::iterator iter;
	const gchar *file;

//	g_return_if_fail (EOG_IS_LIST_STORE (store));
//	g_return_if_fail (EOG_IS_IMAGE (image));

	file = image->uri().c_str();
	
	if (is_file_in_list_store (file, iter)) {
		erase(iter);
	}
}


gint
eog_list_store_get_pos_by_image (EogListStore *store, 
																 const library::LibFile::Ptr &image)
{
	const gchar *file;
	Gtk::TreeIter iter;
	gint pos = -1;

//	g_return_val_if_fail (EOG_IS_LIST_STORE (store), -1);
	
	file = image->uri().c_str();

	if (store->is_file_in_list_store (file, iter)) {
		pos = eog_list_store_get_pos_by_iter (store, iter.gobj());
	}

	return pos;
}

EogImage *
eog_list_store_get_image_by_pos (EogListStore *store, const gint pos)
{
	EogImage *image = NULL;
	GtkTreeIter iter;

//	g_return_val_if_fail (EOG_IS_LIST_STORE (store), NULL);

	if (gtk_tree_model_iter_nth_child (GTK_TREE_MODEL (store), &iter, NULL, pos)) {
		gtk_tree_model_get (GTK_TREE_MODEL (store), &iter, 
				    EOG_LIST_STORE_EOG_IMAGE, &image,
				    -1);
	}

	return image;
}

gint
eog_list_store_get_pos_by_iter (EogListStore *store, 
				GtkTreeIter *iter)
{
	gint *indices;
	GtkTreePath *path;
	gint pos;

	path = gtk_tree_model_get_path (GTK_TREE_MODEL (store), iter);
	indices = gtk_tree_path_get_indices (path);
	pos = indices [0];
	gtk_tree_path_free (path);

	return pos;
}

gint
eog_list_store_length (EogListStore *store)
{
//	g_return_val_if_fail (EOG_IS_LIST_STORE (store), -1);

	return gtk_tree_model_iter_n_children (GTK_TREE_MODEL (store), NULL);
}

gint
eog_list_store_get_initial_pos (EogListStore *store)
{
//	g_return_val_if_fail (EOG_IS_LIST_STORE (store), -1);

	return 0;//store->priv->initial_image;
}

void
eog_list_store_thumbnail_set (EogListStore *store, 
			      GtkTreeIter *iter)
{
#if 0
	EogJob *job;
	EogImage *image;
	gboolean *thumb_set;
	GnomeVFSURI *uri;

	gtk_tree_model_get (GTK_TREE_MODEL (store), iter, 
			    EOG_LIST_STORE_THUMB_SET, &thumb_set,
			    -1);

	if (thumb_set) {
		return;
	}
	gtk_tree_model_get (GTK_TREE_MODEL (store), iter, 
			    EOG_LIST_STORE_EOG_IMAGE, &image, 
			    -1);
	
	uri = eog_image_get_uri (image);
	job = eog_job_thumbnail_new (uri);
	gnome_vfs_uri_unref (uri);

	g_signal_connect (job,
			  "finished",
			  G_CALLBACK (eog_job_thumbnail_cb),
			  store);
	
	g_mutex_lock (store->priv->mutex);
	gtk_list_store_set (GTK_LIST_STORE (store), iter,
			    EOG_LIST_STORE_EOG_JOB, job, 
			    -1);
	eog_job_queue_add_job (job);
	g_mutex_unlock (store->priv->mutex);
	g_object_unref (job);
	g_object_unref (image);
#endif
}

void
eog_list_store_thumbnail_unset (EogListStore *store, 
				GtkTreeIter *iter)
{
#if 0
	EogImage *image;
	EogJob *job;

	gtk_tree_model_get (GTK_TREE_MODEL (store), iter, 
			    EOG_LIST_STORE_EOG_IMAGE, &image,
			    EOG_LIST_STORE_EOG_JOB, &job,
			    -1);

	if (job != NULL) {
		g_mutex_lock (store->priv->mutex);
		eog_job_queue_remove_job (job);
		gtk_list_store_set (GTK_LIST_STORE (store), iter,
				    EOG_LIST_STORE_EOG_JOB, NULL,
				    -1);
		g_mutex_unlock (store->priv->mutex);
	}

	eog_image_set_thumbnail (image, NULL);
	g_object_unref (image);

	gtk_list_store_set (GTK_LIST_STORE (store), iter,
			    EOG_LIST_STORE_THUMBNAIL, store->priv->busy_image,
			    EOG_LIST_STORE_THUMB_SET, FALSE,
			    -1);	
#endif
}
