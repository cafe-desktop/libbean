/*
 * bean-plugin-loader.c
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

#include "config.h"

#include "bean-plugin-loader.h"

G_DEFINE_ABSTRACT_TYPE (BeanPluginLoader, bean_plugin_loader, G_TYPE_OBJECT)

static void
bean_plugin_loader_finalize (GObject *object)
{
  g_debug ("Plugin Loader '%s' Finalized", G_OBJECT_TYPE_NAME (object));

  G_OBJECT_CLASS (bean_plugin_loader_parent_class)->finalize (object);
}

static void
bean_plugin_loader_init (BeanPluginLoader *loader G_GNUC_UNUSED)
{
}

static void
bean_plugin_loader_class_init (BeanPluginLoaderClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = bean_plugin_loader_finalize;
}

gboolean
bean_plugin_loader_initialize (BeanPluginLoader *loader)
{
  BeanPluginLoaderClass *klass;

  g_return_val_if_fail (BEAN_IS_PLUGIN_LOADER (loader), FALSE);

  klass = BEAN_PLUGIN_LOADER_GET_CLASS (loader);

  /* Do this here instead of each time */
  g_return_val_if_fail (klass->load != NULL, FALSE);
  g_return_val_if_fail (klass->unload != NULL, FALSE);
  g_return_val_if_fail (klass->provides_extension != NULL, FALSE);
  g_return_val_if_fail (klass->create_extension != NULL, FALSE);

  if (klass->initialize != NULL)
    return klass->initialize (loader);

  return TRUE;
}

gboolean
bean_plugin_loader_is_global (BeanPluginLoader *loader)
{
  BeanPluginLoaderClass *klass;

  g_return_val_if_fail (BEAN_IS_PLUGIN_LOADER (loader), FALSE);

  klass = BEAN_PLUGIN_LOADER_GET_CLASS (loader);

  if (klass->is_global != NULL)
    return klass->is_global (loader);

  return TRUE;
}

gboolean
bean_plugin_loader_load (BeanPluginLoader *loader,
                         BeanPluginInfo   *info)
{

  g_return_val_if_fail (BEAN_IS_PLUGIN_LOADER (loader), FALSE);

  return BEAN_PLUGIN_LOADER_GET_CLASS (loader)->load (loader, info);
}

void
bean_plugin_loader_unload (BeanPluginLoader *loader,
                           BeanPluginInfo   *info)
{
  g_return_if_fail (BEAN_IS_PLUGIN_LOADER (loader));

  BEAN_PLUGIN_LOADER_GET_CLASS (loader)->unload (loader, info);
}

gboolean
bean_plugin_loader_provides_extension (BeanPluginLoader *loader,
                                       BeanPluginInfo   *info,
                                       GType             ext_type)
{
  BeanPluginLoaderClass *klass;

  g_return_val_if_fail (BEAN_IS_PLUGIN_LOADER (loader), FALSE);

  klass = BEAN_PLUGIN_LOADER_GET_CLASS (loader);
  return klass->provides_extension (loader, info, ext_type);
}

BeanExtension *
bean_plugin_loader_create_extension (BeanPluginLoader *loader,
                                     BeanPluginInfo   *info,
                                     GType             ext_type,
                                     guint             n_parameters,
                                     GParameter       *parameters)
{
  BeanPluginLoaderClass *klass;

  g_return_val_if_fail (BEAN_IS_PLUGIN_LOADER (loader), NULL);

  klass = BEAN_PLUGIN_LOADER_GET_CLASS (loader);
  return klass->create_extension (loader, info, ext_type,
                                  n_parameters, parameters);
}

void
bean_plugin_loader_garbage_collect (BeanPluginLoader *loader)
{
  BeanPluginLoaderClass *klass;

  g_return_if_fail (BEAN_IS_PLUGIN_LOADER (loader));

  klass = BEAN_PLUGIN_LOADER_GET_CLASS (loader);

  if (klass->garbage_collect != NULL)
    klass->garbage_collect (loader);
}
