/*
 * has-dep-plugin.c
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

#include "has-dep-plugin.h"

typedef struct {
  GObject *object;
} TestingHasDepPluginPrivate;

static void bean_activatable_iface_init (BeanActivatableInterface *iface);

G_DEFINE_DYNAMIC_TYPE_EXTENDED (TestingHasDepPlugin,
                                testing_has_dep_plugin,
                                BEAN_TYPE_EXTENSION_BASE,
                                0,
                                G_ADD_PRIVATE_DYNAMIC (TestingHasDepPlugin)
                                G_IMPLEMENT_INTERFACE_DYNAMIC (BEAN_TYPE_ACTIVATABLE,
                                                               bean_activatable_iface_init))

#define GET_PRIV(o) \
  (testing_has_dep_plugin_get_instance_private (o))

enum {
  PROP_0,
  PROP_OBJECT
};

static void
testing_has_dep_plugin_set_property (GObject      *object,
                                     guint         prop_id,
                                     const GValue *value,
                                     GParamSpec   *pspec)
{
  TestingHasDepPlugin *plugin = TESTING_HAS_DEP_PLUGIN (object);
  TestingHasDepPluginPrivate *priv = GET_PRIV (plugin);

  switch (prop_id)
    {
    case PROP_OBJECT:
      priv->object = g_value_get_object (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
testing_has_dep_plugin_get_property (GObject    *object,
                                     guint       prop_id,
                                     GValue     *value,
                                     GParamSpec *pspec)
{
  TestingHasDepPlugin *plugin = TESTING_HAS_DEP_PLUGIN (object);
  TestingHasDepPluginPrivate *priv = GET_PRIV (plugin);

  switch (prop_id)
    {
    case PROP_OBJECT:
      g_value_set_object (value, priv->object);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
testing_has_dep_plugin_init (TestingHasDepPlugin *plugin G_GNUC_UNUSED)
{
}

static void
testing_has_dep_plugin_activate (BeanActivatable *activatable G_GNUC_UNUSED)
{
}

static void
testing_has_dep_plugin_deactivate (BeanActivatable *activatable G_GNUC_UNUSED)
{
}

static void
testing_has_dep_plugin_class_init (TestingHasDepPluginClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = testing_has_dep_plugin_set_property;
  object_class->get_property = testing_has_dep_plugin_get_property;

  g_object_class_override_property (object_class, PROP_OBJECT, "object");
}

static void
bean_activatable_iface_init (BeanActivatableInterface *iface)
{
  iface->activate = testing_has_dep_plugin_activate;
  iface->deactivate = testing_has_dep_plugin_deactivate;
}

static void
testing_has_dep_plugin_class_finalize (TestingHasDepPluginClass *klass G_GNUC_UNUSED)
{
}

G_MODULE_EXPORT void
bean_register_types (BeanObjectModule *module)
{
  testing_has_dep_plugin_register_type (G_TYPE_MODULE (module));

  bean_object_module_register_extension_type (module,
                                              BEAN_TYPE_ACTIVATABLE,
                                              TESTING_TYPE_HAS_DEP_PLUGIN);
}
