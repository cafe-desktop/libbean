/*
 * builtin-configurable.c
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

#include "builtin-configurable.h"

static void bean_ctk_configurable_iface_init (BeanGtkConfigurableInterface *iface);

G_DEFINE_DYNAMIC_TYPE_EXTENDED (TestingBuiltinConfigurable,
                                testing_builtin_configurable,
                                BEAN_TYPE_EXTENSION_BASE,
                                0,
                                G_IMPLEMENT_INTERFACE_DYNAMIC (BEAN_GTK_TYPE_CONFIGURABLE,
                                                               bean_ctk_configurable_iface_init))

static void
testing_builtin_configurable_init (TestingBuiltinConfigurable *configurable)
{
}

static void
testing_builtin_configurable_class_init (TestingBuiltinConfigurableClass *klass)
{
}

static GtkWidget *
testing_builtin_create_configure_widget (BeanGtkConfigurable *configurable)
{
  return ctk_label_new ("Hello, World!");
}

static void
bean_ctk_configurable_iface_init (BeanGtkConfigurableInterface *iface)
{
  iface->create_configure_widget = testing_builtin_create_configure_widget;
}

static void
testing_builtin_configurable_class_finalize (TestingBuiltinConfigurableClass *klass)
{
}

G_MODULE_EXPORT void
bean_register_types (BeanObjectModule *module)
{
  testing_builtin_configurable_register_type (G_TYPE_MODULE (module));

  bean_object_module_register_extension_type (module,
                                              BEAN_GTK_TYPE_CONFIGURABLE,
                                              TESTING_TYPE_BUILTIN_CONFIGURABLE);
}
