/*
 * bean-ctk-plugin-manager.h
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

#ifndef __BEAN_CTK_PLUGIN_MANAGER_H__
#define __BEAN_CTK_PLUGIN_MANAGER_H__

#include <ctk/ctk.h>
#include <libbean/bean-engine.h>

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define BEAN_CTK_TYPE_PLUGIN_MANAGER            (bean_ctk_plugin_manager_get_type())
#define BEAN_CTK_PLUGIN_MANAGER(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), BEAN_CTK_TYPE_PLUGIN_MANAGER, BeanCtkPluginManager))
#define BEAN_CTK_PLUGIN_MANAGER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), BEAN_CTK_TYPE_PLUGIN_MANAGER, BeanCtkPluginManagerClass))
#define BEAN_CTK_IS_PLUGIN_MANAGER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), BEAN_CTK_TYPE_PLUGIN_MANAGER))
#define BEAN_CTK_IS_PLUGIN_MANAGER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), BEAN_CTK_TYPE_PLUGIN_MANAGER))
#define BEAN_CTK_PLUGIN_MANAGER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), BEAN_CTK_TYPE_PLUGIN_MANAGER, BeanCtkPluginManagerClass))

typedef struct _BeanCtkPluginManager        BeanCtkPluginManager;
typedef struct _BeanCtkPluginManagerClass   BeanCtkPluginManagerClass;
typedef struct _BeanCtkPluginManagerPrivate BeanCtkPluginManagerPrivate;

/**
 * BeanCtkPluginManager:
 *
 * The #BeanCtkPluginManager structure contains only private data
 * and should only be accessed using the provided API.
 */
struct _BeanCtkPluginManager
{
  CtkBox box;

  /*< private > */
  BeanCtkPluginManagerPrivate *priv;
};

/**
 * BeanCtkPluginManagerClass:
 * @parent_class: The parent class.
 *
 * The class structure for #BeanCtkPluginManager.
 */
struct _BeanCtkPluginManagerClass
{
  CtkBoxClass parent_class;

  /*< private >*/
  gpointer padding[8];
};

BEAN_AVAILABLE_IN_ALL
GType       bean_ctk_plugin_manager_get_type  (void)  G_GNUC_CONST;
BEAN_AVAILABLE_IN_ALL
CtkWidget  *bean_ctk_plugin_manager_new       (BeanEngine           *engine);

BEAN_AVAILABLE_IN_ALL
CtkWidget  *bean_ctk_plugin_manager_get_view  (BeanCtkPluginManager *pm);

G_END_DECLS

#endif /* __BEAN_CTK_PLUGIN_MANAGER_H__  */
