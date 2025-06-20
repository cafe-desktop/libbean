/*
 * bean-plugin-loader-c.c
 * This file is part of libbean
 *
 * Copyright (C) 2008 - Jesse van den Kieboom
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

#include <string.h>

#include "bean-plugin-loader-c.h"

#include "bean-extension-base.h"
#include "bean-object-module.h"
#include "bean-plugin-info-priv.h"

typedef struct {
  GMutex lock;

  GHashTable *loaded_plugins;
} BeanPluginLoaderCPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (BeanPluginLoaderC,
                            bean_plugin_loader_c,
                            BEAN_TYPE_PLUGIN_LOADER)

#define GET_PRIV(o) \
  (bean_plugin_loader_c_get_instance_private (o))

static GQuark quark_extension_type = 0;
static const gchar *intern_plugin_info = NULL;

static gboolean
bean_plugin_loader_c_load (BeanPluginLoader *loader,
                           BeanPluginInfo   *info)
{
  BeanPluginLoaderC *cloader = BEAN_PLUGIN_LOADER_C (loader);
  BeanPluginLoaderCPrivate *priv = GET_PRIV (cloader);

  g_mutex_lock (&priv->lock);

  if (!g_hash_table_lookup_extended (priv->loaded_plugins,
                                     info->filename,
                                     NULL, (gpointer *) &info->loader_data))
    {
      const gchar *module_name, *module_dir;

      module_name = bean_plugin_info_get_module_name (info);
      module_dir = bean_plugin_info_get_module_dir (info);

      if (info->embedded != NULL)
        {
          info->loader_data = bean_object_module_new_embedded (module_name,
                                                               info->embedded);
        }
      else
        {
          /* Force all C modules to be resident in case they
           * use libraries that do not deal well with reloading.
           * Furthermore, we use local linkage to improve module isolation.
           */
          info->loader_data = bean_object_module_new_full (module_name,
                                                           module_dir,
                                                           TRUE, TRUE);
        }

      if (!g_type_module_use (G_TYPE_MODULE (info->loader_data)))
        g_clear_object (&info->loader_data);

      g_hash_table_insert (priv->loaded_plugins,
                           g_strdup (info->filename), info->loader_data);
    }

  g_mutex_unlock (&priv->lock);
  return info->loader_data != NULL;
}

static void
bean_plugin_loader_c_unload (BeanPluginLoader *loader G_GNUC_UNUSED,
                             BeanPluginInfo   *info)
{
  /* Don't bother unloading the plugin's
   * GTypeModule as it is always resident
   */
  info->loader_data = NULL;
}

static gboolean
bean_plugin_loader_c_provides_extension  (BeanPluginLoader *loader G_GNUC_UNUSED,
                                          BeanPluginInfo   *info,
                                          GType             exten_type)
{
  return bean_object_module_provides_object (info->loader_data, exten_type);
}

static BeanExtension *
bean_plugin_loader_c_create_extension (BeanPluginLoader *loader G_GNUC_UNUSED,
                                       BeanPluginInfo   *info,
                                       GType             exten_type,
                                       guint             n_parameters,
                                       GParameter       *parameters)
{
  GParameter *exten_parameters;
  gpointer instance;

  /* We want to add a "plugin-info" property so we can pass it to
   * the extension if it inherits from BeanExtensionBase. No need to
   * actually "duplicate" the GValues, a memcpy is sufficient as the
   * source GValues are longer lived than our local copy.
   */
  exten_parameters = g_newa (GParameter, n_parameters + 1);
  memcpy (exten_parameters, parameters, sizeof (GParameter) * n_parameters);

  /* Initialize our additional property.
   * If the instance does not have a plugin-info property
   * then BeanObjectModule will remove the property.
   */
  exten_parameters[n_parameters].name = intern_plugin_info;
  memset (&exten_parameters[n_parameters].value, 0, sizeof (GValue));
  g_value_init (&exten_parameters[n_parameters].value, BEAN_TYPE_PLUGIN_INFO);
  g_value_set_boxed (&exten_parameters[n_parameters].value, info);

  instance = bean_object_module_create_object (info->loader_data,
                                               exten_type,
                                               n_parameters + 1,
                                               exten_parameters);

  g_value_unset (&exten_parameters[n_parameters].value);

  if (instance == NULL)
    return NULL;

  g_return_val_if_fail (G_IS_OBJECT (instance), NULL);
  g_return_val_if_fail (G_TYPE_CHECK_INSTANCE_TYPE (instance, exten_type), NULL);

  /* We have to remember which interface we are instantiating
   * for the deprecated bean_extension_get_extension_type().
   */
  g_object_set_qdata (instance, quark_extension_type,
                      GSIZE_TO_POINTER (exten_type));

  return instance;
}

static void
bean_plugin_loader_c_init (BeanPluginLoaderC *cloader)
{
  BeanPluginLoaderCPrivate *priv = GET_PRIV (cloader);

  g_mutex_init (&priv->lock);

  /* loaded_plugins maps BeanPluginInfo:filename to a BeanObjectModule */
  priv->loaded_plugins = g_hash_table_new_full (g_str_hash, g_str_equal,
                                                g_free, NULL);
}

static void
bean_plugin_loader_c_finalize (GObject *object)
{
  BeanPluginLoaderC *cloader = BEAN_PLUGIN_LOADER_C (object);
  BeanPluginLoaderCPrivate *priv = GET_PRIV (cloader);

  g_mutex_clear (&priv->lock);

  g_hash_table_destroy (priv->loaded_plugins);

  G_OBJECT_CLASS (bean_plugin_loader_c_parent_class)->finalize (object);
}

static void
bean_plugin_loader_c_class_init (BeanPluginLoaderCClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  BeanPluginLoaderClass *loader_class = BEAN_PLUGIN_LOADER_CLASS (klass);

  quark_extension_type = g_quark_from_static_string ("bean-extension-type");
  intern_plugin_info = g_intern_static_string ("plugin-info");

  object_class->finalize = bean_plugin_loader_c_finalize;

  loader_class->load = bean_plugin_loader_c_load;
  loader_class->unload = bean_plugin_loader_c_unload;
  loader_class->provides_extension = bean_plugin_loader_c_provides_extension;
  loader_class->create_extension = bean_plugin_loader_c_create_extension;
}

/*
 * bean_plugin_loader_c_new:
 *
 * Return a new instance of #BeanPluginLoaderC.
 *
 * Returns: a new instance of #BeanPluginLoaderC.
 */
BeanPluginLoader *
bean_plugin_loader_c_new (void)
{
  return BEAN_PLUGIN_LOADER (g_object_new (BEAN_TYPE_PLUGIN_LOADER_C, NULL));
}
