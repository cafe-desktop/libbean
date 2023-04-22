/*
 * bean-plugin-manager-store.h
 * This file is part of libbean
 *
 * Copyright (C) 2002 Paolo Maggi and James Willcox
 * Copyright (C) 2003-2006 Paolo Maggi, Paolo Borelli
 * Copyright (C) 2007-2009 Paolo Maggi, Paolo Borelli, Steve Frécinaux
 * Copyright (C) 2010 Garrett Regier
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

#ifndef __PEAS_GTK_PLUGIN_MANAGER_STORE_H__
#define __PEAS_GTK_PLUGIN_MANAGER_STORE_H__

#include <gtk/gtk.h>
#include <libbean/bean-engine.h>
#include <libbean/bean-plugin-info.h>

G_BEGIN_DECLS

typedef enum {
  PEAS_GTK_PLUGIN_MANAGER_STORE_ENABLED_COLUMN = 0,
  PEAS_GTK_PLUGIN_MANAGER_STORE_CAN_ENABLE_COLUMN,
  PEAS_GTK_PLUGIN_MANAGER_STORE_ICON_GICON_COLUMN,
  PEAS_GTK_PLUGIN_MANAGER_STORE_ICON_STOCK_ID_COLUMN,
  PEAS_GTK_PLUGIN_MANAGER_STORE_ICON_VISIBLE_COLUMN,
  PEAS_GTK_PLUGIN_MANAGER_STORE_INFO_COLUMN,
  PEAS_GTK_PLUGIN_MANAGER_STORE_INFO_SENSITIVE_COLUMN,
  PEAS_GTK_PLUGIN_MANAGER_STORE_PLUGIN_COLUMN,
  PEAS_GTK_PLUGIN_MANAGER_STORE_N_COLUMNS
} BeanGtkPluginManagerStoreColumns;

/*
 * Type checking and casting macros
 */
#define PEAS_GTK_TYPE_PLUGIN_MANAGER_STORE            (bean_gtk_plugin_manager_store_get_type())
#define PEAS_GTK_PLUGIN_MANAGER_STORE(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), PEAS_GTK_TYPE_PLUGIN_MANAGER_STORE, BeanGtkPluginManagerStore))
#define PEAS_GTK_PLUGIN_MANAGER_STORE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), PEAS_GTK_TYPE_PLUGIN_MANAGER_STORE, BeanGtkPluginManagerStoreClass))
#define PEAS_GTK_IS_PLUGIN_MANAGER_STORE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), PEAS_GTK_TYPE_PLUGIN_MANAGER_STORE))
#define PEAS_GTK_IS_PLUGIN_MANAGER_STORE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), PEAS_GTK_TYPE_PLUGIN_MANAGER_STORE))
#define PEAS_GTK_PLUGIN_MANAGER_STORE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), PEAS_GTK_TYPE_PLUGIN_MANAGER_STORE, BeanGtkPluginManagerStoreClass))

typedef struct _BeanGtkPluginManagerStore         BeanGtkPluginManagerStore;
typedef struct _BeanGtkPluginManagerStoreClass    BeanGtkPluginManagerStoreClass;

struct _BeanGtkPluginManagerStore {
  GtkListStore parent;
};

struct _BeanGtkPluginManagerStoreClass {
  GtkListStoreClass parent_class;
};

GType                       bean_gtk_plugin_manager_store_get_type              (void) G_GNUC_CONST;
BeanGtkPluginManagerStore  *bean_gtk_plugin_manager_store_new                   (BeanEngine                *engine);

void                        bean_gtk_plugin_manager_store_reload                (BeanGtkPluginManagerStore *store);

void                        bean_gtk_plugin_manager_store_set_enabled           (BeanGtkPluginManagerStore *store,
                                                                                 GtkTreeIter               *iter,
                                                                                 gboolean                   enabled);
gboolean                    bean_gtk_plugin_manager_store_get_enabled           (BeanGtkPluginManagerStore *store,
                                                                                 GtkTreeIter               *iter);
void                        bean_gtk_plugin_manager_store_set_all_enabled       (BeanGtkPluginManagerStore *store,
                                                                                 gboolean                  enabled);
void                        bean_gtk_plugin_manager_store_toggle_enabled        (BeanGtkPluginManagerStore *store,
                                                                                 GtkTreeIter               *iter);

gboolean                    bean_gtk_plugin_manager_store_can_enable            (BeanGtkPluginManagerStore *store,
                                                                                 GtkTreeIter               *iter);

BeanPluginInfo             *bean_gtk_plugin_manager_store_get_plugin            (BeanGtkPluginManagerStore *store,
                                                                                 GtkTreeIter               *iter);

gboolean                    bean_gtk_plugin_manager_store_get_iter_from_plugin  (BeanGtkPluginManagerStore *store,
                                                                                 GtkTreeIter               *iter,
                                                                                 const BeanPluginInfo      *info);
G_END_DECLS

#endif /* __PEAS_GTK_PLUGIN_MANAGER_STORE_H__  */
