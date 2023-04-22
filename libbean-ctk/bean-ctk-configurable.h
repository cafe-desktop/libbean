/*
 * bean-ctk-configurable.h
 * This file is part of libbean
 *
 * Copyright (C) 2009 Steve Frécinaux
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

#ifndef __BEAN_GTK_CONFIGURABLE_H__
#define __BEAN_GTK_CONFIGURABLE_H__

#include <ctk/ctk.h>
#include <libbean/bean.h>

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define BEAN_GTK_TYPE_CONFIGURABLE            (bean_ctk_configurable_get_type ())
#define BEAN_GTK_CONFIGURABLE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), BEAN_GTK_TYPE_CONFIGURABLE, BeanCtkConfigurable))
#define BEAN_GTK_CONFIGURABLE_IFACE(obj)      (G_TYPE_CHECK_CLASS_CAST ((obj), BEAN_GTK_TYPE_CONFIGURABLE, BeanCtkConfigurableInterface))
#define BEAN_GTK_IS_CONFIGURABLE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BEAN_GTK_TYPE_CONFIGURABLE))
#define BEAN_GTK_CONFIGURABLE_GET_IFACE(obj)  (G_TYPE_INSTANCE_GET_INTERFACE ((obj), BEAN_GTK_TYPE_CONFIGURABLE, BeanCtkConfigurableInterface))

/**
 * BeanCtkConfigurable:
 *
 * Interface for configurable plugins.
 */
typedef struct _BeanCtkConfigurable           BeanCtkConfigurable; /* dummy typedef */
typedef struct _BeanCtkConfigurableInterface  BeanCtkConfigurableInterface;

/**
 * BeanCtkConfigurableInterface:
 * @g_iface: The parent interface.
 * @create_configure_widget: Creates the configure widget for the plugin.
 *
 * Provides an interface for configurable plugins.
 */
struct _BeanCtkConfigurableInterface
{
  GTypeInterface g_iface;

  CtkWidget  *(*create_configure_widget)  (BeanCtkConfigurable  *configurable);
};

BEAN_AVAILABLE_IN_ALL
GType       bean_ctk_configurable_get_type                (void)  G_GNUC_CONST;
BEAN_AVAILABLE_IN_ALL
CtkWidget  *bean_ctk_configurable_create_configure_widget (BeanCtkConfigurable  *configurable);

G_END_DECLS

#endif /* __BEAN_GTK_PLUGIN_MANAGER_H__  */
