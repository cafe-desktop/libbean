/*
 * bean-ctk-configurable.c
 * This file is part of libbean
 *
 * Copyright (C) 2009-2010 Steve Frécinaux
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "bean-ctk-configurable.h"

/**
 * SECTION:bean-ctk-configurable
 * @short_description: Interface for providing a plugin configuration UI.
 *
 * The #BeanCtkConfigurable interface will allow a plugin to provide a
 * graphical interface for the user to configure the plugin through the
 * #BeanCtkPluginManager: the #BeanCtkPluginManager will make its
 * “Configure Plugin” button active when the selected plugin implements
 * the #BeanCtkConfigurable interface.
 *
 * To allow plugin configuration from the #BeanCtkPluginManager, the
 * plugin writer will just need to implement the
 * BeanCtkConfigurableInterface.create_configure_widget() method.
 **/

G_DEFINE_INTERFACE(BeanCtkConfigurable, bean_ctk_configurable, G_TYPE_OBJECT)

static void
bean_ctk_configurable_default_init (BeanCtkConfigurableInterface *iface)
{
}

/**
 * bean_ctk_configurable_create_configure_widget:
 * @configurable: A #BeanCtkConfigurable
 *
 * Creates the configure widget for the plugin. The returned widget
 * should allow configuring all the relevant aspects of the plugin, and should
 * allow instant-apply, as promoted by the Gnome Human Interface Guidelines.
 *
 * #BeanCtkPluginManager will embed the returned widget into a dialog box,
 * but you shouldn't take this behaviour for granted as other implementations
 * of a plugin manager UI might do otherwise.
 *
 * This method should always return a valid #CtkWidget instance, never %NULL.
 *
 * Returns: (transfer full): A #CtkWidget used for configuration.
 */
CtkWidget *
bean_ctk_configurable_create_configure_widget (BeanCtkConfigurable *configurable)
{
  BeanCtkConfigurableInterface *iface;

  g_return_val_if_fail (BEAN_CTK_IS_CONFIGURABLE (configurable), NULL);

  iface = BEAN_CTK_CONFIGURABLE_GET_IFACE (configurable);

  if (G_LIKELY (iface->create_configure_widget != NULL))
    return iface->create_configure_widget (configurable);

  /* Default implementation */
  return NULL;
}
