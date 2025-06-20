/*
 * configurable-plugin.c
 * This file is part of libbean
 *
 * Copyright (C) 2010 - Garrett Regier
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

#include <glib.h>
#include <glib-object.h>
#include <gmodule.h>

#include <libbean/bean.h>
#include <libbean-ctk/bean-ctk.h>

#include "configurable-plugin.h"

static void bean_ctk_configurable_iface_init (BeanCtkConfigurableInterface *iface);

G_DEFINE_DYNAMIC_TYPE_EXTENDED (TestingConfigurablePlugin,
                                testing_configurable_plugin,
                                BEAN_TYPE_EXTENSION_BASE,
                                0,
                                G_IMPLEMENT_INTERFACE_DYNAMIC (BEAN_CTK_TYPE_CONFIGURABLE,
                                                               bean_ctk_configurable_iface_init))

static void
testing_configurable_plugin_init (TestingConfigurablePlugin *configurable G_GNUC_UNUSED)
{
}

static void
testing_configurable_plugin_class_init (TestingConfigurablePluginClass *klass G_GNUC_UNUSED)
{
}

static CtkWidget *
testing_configurable_plugin_create_configure_widget (BeanCtkConfigurable *configurable G_GNUC_UNUSED)
{
  return ctk_label_new ("Hello, World!");
}

static void
bean_ctk_configurable_iface_init (BeanCtkConfigurableInterface *iface)
{
  iface->create_configure_widget = testing_configurable_plugin_create_configure_widget;
}

static void
testing_configurable_plugin_class_finalize (TestingConfigurablePluginClass *klass G_GNUC_UNUSED)
{
}

G_MODULE_EXPORT void
bean_register_types (BeanObjectModule *module)
{
  testing_configurable_plugin_register_type (G_TYPE_MODULE (module));

  bean_object_module_register_extension_type (module,
                                              BEAN_CTK_TYPE_CONFIGURABLE,
                                              TESTING_TYPE_CONFIGURABLE_PLUGIN);
}
