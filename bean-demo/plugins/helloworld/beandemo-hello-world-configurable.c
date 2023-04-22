/*
 * beandemo-hello-world-configurable.c
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

#include <glib.h>
#include <glib-object.h>
#include <gmodule.h>
#include <ctk/ctk.h>

#include <libbean/bean.h>
#include <libbean-ctk/bean-ctk.h>

#include "beandemo-hello-world-configurable.h"

static void bean_ctk_configurable_iface_init (BeanCtkConfigurableInterface *iface);

G_DEFINE_DYNAMIC_TYPE_EXTENDED (BeanDemoHelloWorldConfigurable,
                                beandemo_hello_world_configurable,
                                BEAN_TYPE_EXTENSION_BASE,
                                0,
                                G_IMPLEMENT_INTERFACE_DYNAMIC (BEAN_GTK_TYPE_CONFIGURABLE,
                                                               bean_ctk_configurable_iface_init))

static void
beandemo_hello_world_configurable_init (BeanDemoHelloWorldConfigurable *plugin)
{
  g_debug ("%s", G_STRFUNC);
}

static CtkWidget *
beandemo_hello_world_configurable_create_configure_widget (BeanCtkConfigurable *configurable)
{
  g_debug ("%s", G_STRFUNC);

  return ctk_label_new ("This is a configuration dialog for the HelloWorld plugin.");
}

static void
beandemo_hello_world_configurable_class_init (BeanDemoHelloWorldConfigurableClass *klass)
{
}

static void
bean_ctk_configurable_iface_init (BeanCtkConfigurableInterface *iface)
{
  iface->create_configure_widget = beandemo_hello_world_configurable_create_configure_widget;
}

static void
beandemo_hello_world_configurable_class_finalize (BeanDemoHelloWorldConfigurableClass *klass)
{
}

void
beandemo_hello_world_configurable_register (GTypeModule *module)
{
  beandemo_hello_world_configurable_register_type (module);
}
