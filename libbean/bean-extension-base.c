/*
 * bean-extension-base.c
 * This file is part of libbean
 *
 * Copyright (C) 2002-2008 Paolo Maggi
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

#include "config.h"

#include "bean-extension-base.h"
#include "bean-plugin-info-priv.h"

/**
 * SECTION:bean-extension-base
 * @short_description: Base class for C extensions.
 * @see_also: #BeanPluginInfo
 *
 * #BeanExtensionBase can optionally be used as a base class for the extensions
 * of your plugin. By inheriting from it, you will make your extension able to
 * access the related #BeanPluginInfo, and especially the location where all
 * the data of your plugin lives.
 *
 * Non-C extensions will usually not inherit from this class: Python
 * plugins automatically get a "plugin_info" attribute that serves
 * the same purpose.
 **/

struct _BeanExtensionBasePrivate {
  BeanPluginInfo *info;
};

/* properties */
enum {
  PROP_0,
  PROP_PLUGIN_INFO,
  PROP_DATA_DIR,
  N_PROPERTIES
};

static GParamSpec *properties[N_PROPERTIES] = { NULL };

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE (BeanExtensionBase,
                                     bean_extension_base,
                                     G_TYPE_OBJECT)

#define GET_PRIV(o) \
  (bean_extension_base_get_instance_private (o))

static void
bean_extension_base_get_property (GObject    *object,
                                  guint       prop_id,
                                  GValue     *value,
                                  GParamSpec *pspec)
{
  BeanExtensionBase *extbase = BEAN_EXTENSION_BASE (object);

  switch (prop_id)
    {
    case PROP_PLUGIN_INFO:
      g_value_set_boxed (value, bean_extension_base_get_plugin_info (extbase));
      break;
    case PROP_DATA_DIR:
      g_value_take_string (value, bean_extension_base_get_data_dir (extbase));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
bean_extension_base_set_property (GObject      *object,
                                  guint         prop_id,
                                  const GValue *value,
                                  GParamSpec   *pspec)
{
  BeanExtensionBase *extbase = BEAN_EXTENSION_BASE (object);
  BeanExtensionBasePrivate *priv = GET_PRIV (extbase);

  switch (prop_id)
    {
    case PROP_PLUGIN_INFO:
      priv->info = g_value_get_boxed (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
bean_extension_base_init (BeanExtensionBase *extbase G_GNUC_UNUSED)
{
}

static void
bean_extension_base_class_init (BeanExtensionBaseClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->get_property = bean_extension_base_get_property;
  object_class->set_property = bean_extension_base_set_property;

  /**
   * BeanExtensionBase:plugin-info:
   *
   * The #BeanPluginInfo related to the current plugin.
   */
  properties[PROP_PLUGIN_INFO] =
    g_param_spec_boxed ("plugin-info",
                        "Plugin Information",
                        "Information related to the current plugin",
                        BEAN_TYPE_PLUGIN_INFO,
                        G_PARAM_READWRITE |
                        G_PARAM_CONSTRUCT_ONLY |
                        G_PARAM_STATIC_STRINGS);

  /**
   * BeanExtensionBase:data-dir:
   *
   * The The full path of the directory where the plugin
   * should look for its data files.
   *
   * Note: This is the same path as that returned by
   * bean_plugin_info_get_data_dir().
   */
  properties[PROP_DATA_DIR] =
    g_param_spec_string ("data-dir",
                         "Data Directory",
                         "The full path of the directory where the "
                         "plugin should look for its data files",
                         NULL,
                         G_PARAM_READABLE |
                         G_PARAM_STATIC_STRINGS);

  g_object_class_install_properties (object_class, N_PROPERTIES, properties);
}

/**
 * bean_extension_base_get_plugin_info:
 * @extbase: A #BeanExtensionBase.
 *
 * Get information relative to @extbase.
 *
 * Return value: (transfer none): the #BeanPluginInfo relative
 * to the #BeanExtensionBase.
 */
BeanPluginInfo *
bean_extension_base_get_plugin_info (BeanExtensionBase *extbase)
{
  BeanExtensionBasePrivate *priv = GET_PRIV (extbase);

  g_return_val_if_fail (BEAN_IS_EXTENSION_BASE (extbase), NULL);

  return priv->info;
}

/**
 * bean_extension_base_get_data_dir:
 * @extbase: A #BeanExtensionBase.
 *
 * Get the path of the directory where the plugin should look for
 * its data files.
 *
 * Return value: A newly allocated string with the path of the
 * directory where the plugin should look for its data files
 */
gchar *
bean_extension_base_get_data_dir (BeanExtensionBase *extbase)
{
  BeanExtensionBasePrivate *priv = GET_PRIV (extbase);

  g_return_val_if_fail (BEAN_IS_EXTENSION_BASE (extbase), NULL);

  return g_strdup (bean_plugin_info_get_data_dir (priv->info));
}
