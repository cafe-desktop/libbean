/*
 * bean-gtk-configurable.h
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

#ifndef __PEAS_GTK_CONFIGURABLE_H__
#define __PEAS_GTK_CONFIGURABLE_H__

#include <gtk/gtk.h>
#include <libbean/bean.h>

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define PEAS_GTK_TYPE_CONFIGURABLE            (bean_gtk_configurable_get_type ())
#define PEAS_GTK_CONFIGURABLE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), PEAS_GTK_TYPE_CONFIGURABLE, BeanGtkConfigurable))
#define PEAS_GTK_CONFIGURABLE_IFACE(obj)      (G_TYPE_CHECK_CLASS_CAST ((obj), PEAS_GTK_TYPE_CONFIGURABLE, BeanGtkConfigurableInterface))
#define PEAS_GTK_IS_CONFIGURABLE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), PEAS_GTK_TYPE_CONFIGURABLE))
#define PEAS_GTK_CONFIGURABLE_GET_IFACE(obj)  (G_TYPE_INSTANCE_GET_INTERFACE ((obj), PEAS_GTK_TYPE_CONFIGURABLE, BeanGtkConfigurableInterface))

/**
 * BeanGtkConfigurable:
 *
 * Interface for configurable plugins.
 */
typedef struct _BeanGtkConfigurable           BeanGtkConfigurable; /* dummy typedef */
typedef struct _BeanGtkConfigurableInterface  BeanGtkConfigurableInterface;

/**
 * BeanGtkConfigurableInterface:
 * @g_iface: The parent interface.
 * @create_configure_widget: Creates the configure widget for the plugin.
 *
 * Provides an interface for configurable plugins.
 */
struct _BeanGtkConfigurableInterface
{
  GTypeInterface g_iface;

  GtkWidget  *(*create_configure_widget)  (BeanGtkConfigurable  *configurable);
};

PEAS_AVAILABLE_IN_ALL
GType       bean_gtk_configurable_get_type                (void)  G_GNUC_CONST;
PEAS_AVAILABLE_IN_ALL
GtkWidget  *bean_gtk_configurable_create_configure_widget (BeanGtkConfigurable  *configurable);

G_END_DECLS

#endif /* __PEAS_GTK_PLUGIN_MANAGER_H__  */
