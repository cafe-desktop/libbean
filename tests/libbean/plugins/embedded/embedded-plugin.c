/*
 * embedded-plugin.c
 * This file is part of libbean
 *
 * Copyright (C) 2015 - Garrett Regier
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

#include <glib-object.h>

#include <libbean/bean.h>

#include "embedded-plugin.h"

typedef struct {
  GObject *object;
} TestingEmbeddedPluginPrivate;

static void bean_activatable_iface_init (BeanActivatableInterface *iface);

G_DEFINE_TYPE_EXTENDED (TestingEmbeddedPlugin,
                        testing_embedded_plugin,
                        BEAN_TYPE_EXTENSION_BASE,
                        0,
                        G_ADD_PRIVATE (TestingEmbeddedPlugin)
                        G_IMPLEMENT_INTERFACE (BEAN_TYPE_ACTIVATABLE,
                                               bean_activatable_iface_init))

#define GET_PRIV(o) \
  (testing_embedded_plugin_get_instance_private (o))

enum {
  PROP_0,
  PROP_OBJECT
};

static void
testing_embedded_plugin_set_property (GObject      *object,
                                      guint         prop_id,
                                      const GValue *value,
                                      GParamSpec   *pspec)
{
  TestingEmbeddedPlugin *plugin = TESTING_EMBEDDED_PLUGIN (object);
  TestingEmbeddedPluginPrivate *priv = GET_PRIV (plugin);

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
testing_embedded_plugin_get_property (GObject    *object,
                                      guint       prop_id,
                                      GValue     *value,
                                      GParamSpec *pspec)
{
  TestingEmbeddedPlugin *plugin = TESTING_EMBEDDED_PLUGIN (object);
  TestingEmbeddedPluginPrivate *priv = GET_PRIV (plugin);

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
testing_embedded_plugin_init (TestingEmbeddedPlugin *plugin G_GNUC_UNUSED)
{
}

static void
testing_embedded_plugin_activate (BeanActivatable *activatable G_GNUC_UNUSED)
{
}

static void
testing_embedded_plugin_deactivate (BeanActivatable *activatable G_GNUC_UNUSED)
{
}

static void
testing_embedded_plugin_class_init (TestingEmbeddedPluginClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = testing_embedded_plugin_set_property;
  object_class->get_property = testing_embedded_plugin_get_property;

  g_object_class_override_property (object_class, PROP_OBJECT, "object");
}

static void
bean_activatable_iface_init (BeanActivatableInterface *iface)
{
  iface->activate = testing_embedded_plugin_activate;
  iface->deactivate = testing_embedded_plugin_deactivate;
}

G_MODULE_EXPORT void
testing_embedded_plugin_register_types (BeanObjectModule *module)
{
  bean_object_module_register_extension_type (module,
                                              BEAN_TYPE_ACTIVATABLE,
                                              TESTING_TYPE_EMBEDDED_PLUGIN);
}
