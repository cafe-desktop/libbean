/*
 * bean-gtk-plugin-manager.h
 * This file is part of libbean
 *
 * Copyright (C) 2005-2009 Paolo Maggi, Paolo Borelli
 *
 * libbean is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * libbean is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA.
 */

#ifndef __BEAN_GTK_PLUGIN_MANAGER_H__
#define __BEAN_GTK_PLUGIN_MANAGER_H__

#include <gtk/gtk.h>
#include <libbean/bean-engine.h>

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define BEAN_GTK_TYPE_PLUGIN_MANAGER            (bean_gtk_plugin_manager_get_type())
#define BEAN_GTK_PLUGIN_MANAGER(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), BEAN_GTK_TYPE_PLUGIN_MANAGER, BeanGtkPluginManager))
#define BEAN_GTK_PLUGIN_MANAGER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), BEAN_GTK_TYPE_PLUGIN_MANAGER, BeanGtkPluginManagerClass))
#define BEAN_GTK_IS_PLUGIN_MANAGER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), BEAN_GTK_TYPE_PLUGIN_MANAGER))
#define BEAN_GTK_IS_PLUGIN_MANAGER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), BEAN_GTK_TYPE_PLUGIN_MANAGER))
#define BEAN_GTK_PLUGIN_MANAGER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), BEAN_GTK_TYPE_PLUGIN_MANAGER, BeanGtkPluginManagerClass))

typedef struct _BeanGtkPluginManager        BeanGtkPluginManager;
typedef struct _BeanGtkPluginManagerClass   BeanGtkPluginManagerClass;
typedef struct _BeanGtkPluginManagerPrivate BeanGtkPluginManagerPrivate;

/**
 * BeanGtkPluginManager:
 *
 * The #BeanGtkPluginManager structure contains only private data
 * and should only be accessed using the provided API.
 */
struct _BeanGtkPluginManager
{
  GtkBox box;

  /*< private > */
  BeanGtkPluginManagerPrivate *priv;
};

/**
 * BeanGtkPluginManagerClass:
 * @parent_class: The parent class.
 *
 * The class structure for #BeanGtkPluginManager.
 */
struct _BeanGtkPluginManagerClass
{
  GtkBoxClass parent_class;

  /*< private >*/
  gpointer padding[8];
};

BEAN_AVAILABLE_IN_ALL
GType       bean_gtk_plugin_manager_get_type  (void)  G_GNUC_CONST;
BEAN_AVAILABLE_IN_ALL
GtkWidget  *bean_gtk_plugin_manager_new       (BeanEngine           *engine);

BEAN_AVAILABLE_IN_ALL
GtkWidget  *bean_gtk_plugin_manager_get_view  (BeanGtkPluginManager *pm);

G_END_DECLS

#endif /* __BEAN_GTK_PLUGIN_MANAGER_H__  */
