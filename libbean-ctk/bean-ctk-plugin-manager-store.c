/*
 * bean-ctk-plugin-manager-store.c
 * This file is part of libbean
 *
 * Copyright (C) 2002 Paolo Maggi and James Willcox
 * Copyright (C) 2003-2006 Paolo Maggi, Paolo Borelli
 * Copyright (C) 2007-2009 Paolo Maggi, Paolo Borelli, Steve Frécinaux
 * Copyright (C) 2010 Garrett Regier
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

#include <gio/gio.h>

#include <libbean/bean-plugin-info.h>

#include "bean-ctk-plugin-manager-store.h"

static const GType ColumnTypes[] = {
  G_TYPE_BOOLEAN, /* Enabled */
  G_TYPE_BOOLEAN, /* Enabled Visible */
  G_TYPE_OBJECT,  /* GIcon Icon */
  G_TYPE_STRING,  /* Stock ID Icon */
  G_TYPE_BOOLEAN, /* Icon Visible */
  G_TYPE_STRING,  /* Info */
  G_TYPE_BOOLEAN, /* Info Visible */
  /* To avoid having to unref it all the time */
  G_TYPE_POINTER  /* BeanPluginInfo */
};

G_STATIC_ASSERT (G_N_ELEMENTS (ColumnTypes) == BEAN_CTK_PLUGIN_MANAGER_STORE_N_COLUMNS);

typedef struct {
  BeanEngine *engine;
} BeanCtkPluginManagerStorePrivate;

/* Properties */
enum {
  PROP_0,
  PROP_ENGINE,
  N_PROPERTIES
};

static GParamSpec *properties[N_PROPERTIES] = { NULL };

G_DEFINE_TYPE_WITH_PRIVATE (BeanCtkPluginManagerStore,
                            bean_ctk_plugin_manager_store,
                            CTK_TYPE_LIST_STORE)

#define GET_PRIV(o) \
  (bean_ctk_plugin_manager_store_get_instance_private (o))

static void
update_plugin (BeanCtkPluginManagerStore *store,
               CtkTreeIter               *iter,
               BeanPluginInfo            *info)
{
  gboolean loaded;
  gboolean available;
  gboolean builtin;
  gchar *markup;
  const gchar *icon_name;
  const gchar *icon_stock_id = NULL;
  GIcon *icon_gicon = NULL;

  loaded = bean_plugin_info_is_loaded (info);
  available = bean_plugin_info_is_available (info, NULL);
  builtin = bean_plugin_info_is_builtin (info);

  if (bean_plugin_info_get_description (info) == NULL)
    {
      markup = g_markup_printf_escaped ("<b>%s</b>",
                                        bean_plugin_info_get_name (info));
    }
  else
    {
      markup = g_markup_printf_escaped ("<b>%s</b>\n%s",
                                        bean_plugin_info_get_name (info),
                                        bean_plugin_info_get_description (info));
    }

  if (!available)
    {
      icon_gicon = g_themed_icon_new ("dialog-error");
    }
  else
    {
      gchar *icon_path;

      icon_name = bean_plugin_info_get_icon_name (info);
      icon_path = g_build_filename (bean_plugin_info_get_data_dir (info),
                                    icon_name,
                                    NULL);

      /* Prevent warning for the common case that icon_path
       * does not exist but warn when it is a directory
       */
      if (g_file_test (icon_path, G_FILE_TEST_EXISTS))
        {
          GFile *icon_file;

          icon_file = g_file_new_for_path (icon_path);
          icon_gicon = g_file_icon_new (icon_file);

          g_object_unref (icon_file);
        }
      else
        {
          gint i;
          CtkIconTheme *icon_theme;
          const gchar * const *names;
          gboolean found_icon = FALSE;

          icon_gicon = g_themed_icon_new_with_default_fallbacks (icon_name);

          icon_theme = ctk_icon_theme_get_default ();
          names = g_themed_icon_get_names (G_THEMED_ICON (icon_gicon));

          for (i = 0; !found_icon && names[i] != NULL; ++i)
            found_icon = ctk_icon_theme_has_icon (icon_theme, names[i]);

          if (!found_icon)
            {
              CtkStockItem stock_item;

              g_clear_object (&icon_gicon);

              if (ctk_stock_lookup (icon_name, &stock_item))
                {
                  icon_stock_id = icon_name;
                }
              else
                {
                  icon_gicon = g_themed_icon_new ("libbean-plugin");
                }
            }
        }

      g_free (icon_path);
    }

  ctk_list_store_set (CTK_LIST_STORE (store), iter,
    BEAN_CTK_PLUGIN_MANAGER_STORE_ENABLED_COLUMN,        loaded,
    BEAN_CTK_PLUGIN_MANAGER_STORE_CAN_ENABLE_COLUMN,     !builtin && available,
    BEAN_CTK_PLUGIN_MANAGER_STORE_ICON_GICON_COLUMN,     icon_gicon,
    BEAN_CTK_PLUGIN_MANAGER_STORE_ICON_STOCK_ID_COLUMN,  icon_stock_id,
    BEAN_CTK_PLUGIN_MANAGER_STORE_ICON_VISIBLE_COLUMN,   !available,
    BEAN_CTK_PLUGIN_MANAGER_STORE_INFO_COLUMN,           markup,
    BEAN_CTK_PLUGIN_MANAGER_STORE_INFO_SENSITIVE_COLUMN, available && (!builtin || loaded),
    BEAN_CTK_PLUGIN_MANAGER_STORE_PLUGIN_COLUMN,         info,
    -1);

  g_clear_object (&icon_gicon);
  g_free (markup);
}

static void
plugin_loaded_toggled_cb (BeanEngine                *engine G_GNUC_UNUSED,
                          BeanPluginInfo            *info,
                          BeanCtkPluginManagerStore *store)
{
  CtkTreeIter iter;

  if (bean_ctk_plugin_manager_store_get_iter_from_plugin (store, &iter, info))
    update_plugin (store, &iter, info);
}

static gint
model_name_sort_func (BeanCtkPluginManagerStore *store,
		      CtkTreeIter               *iter1,
		      CtkTreeIter               *iter2,
		      gpointer                   user_data G_GNUC_UNUSED)
{
  BeanPluginInfo *info1;
  BeanPluginInfo *info2;

  info1 = bean_ctk_plugin_manager_store_get_plugin (store, iter1);
  info2 = bean_ctk_plugin_manager_store_get_plugin (store, iter2);

  return g_utf8_collate (bean_plugin_info_get_name (info1),
                         bean_plugin_info_get_name (info2));
}

static void
bean_ctk_plugin_manager_store_init (BeanCtkPluginManagerStore *store)
{
  ctk_list_store_set_column_types (CTK_LIST_STORE (store),
                                   BEAN_CTK_PLUGIN_MANAGER_STORE_N_COLUMNS,
                                   (GType *) ColumnTypes);

  /* Sort on the plugin names */
  ctk_tree_sortable_set_default_sort_func (CTK_TREE_SORTABLE (store),
                                           (CtkTreeIterCompareFunc) model_name_sort_func,
                                           NULL, NULL);
  ctk_tree_sortable_set_sort_column_id (CTK_TREE_SORTABLE (store),
                                        CTK_TREE_SORTABLE_DEFAULT_SORT_COLUMN_ID,
                                        CTK_SORT_ASCENDING);
}

static void
bean_ctk_plugin_manager_store_set_property (GObject      *object,
                                            guint         prop_id,
                                            const GValue *value,
                                            GParamSpec   *pspec)
{
  BeanCtkPluginManagerStore *store = BEAN_CTK_PLUGIN_MANAGER_STORE (object);
  BeanCtkPluginManagerStorePrivate *priv = GET_PRIV (store);

  switch (prop_id)
    {
    case PROP_ENGINE:
      priv->engine = g_value_get_object (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
bean_ctk_plugin_manager_store_get_property (GObject    *object,
                                            guint       prop_id,
                                            GValue     *value,
                                            GParamSpec *pspec)
{
  BeanCtkPluginManagerStore *store = BEAN_CTK_PLUGIN_MANAGER_STORE (object);
  BeanCtkPluginManagerStorePrivate *priv = GET_PRIV (store);

  switch (prop_id)
    {
    case PROP_ENGINE:
      g_value_set_object (value, priv->engine);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
bean_ctk_plugin_manager_store_constructed (GObject *object)
{
  BeanCtkPluginManagerStore *store = BEAN_CTK_PLUGIN_MANAGER_STORE (object);
  BeanCtkPluginManagerStorePrivate *priv = GET_PRIV (store);

  if (priv->engine == NULL)
    priv->engine = bean_engine_get_default ();

  g_object_ref (priv->engine);

  g_signal_connect_object (priv->engine,
                           "load-plugin",
                           G_CALLBACK (plugin_loaded_toggled_cb),
                           store,
                           G_CONNECT_AFTER);
  g_signal_connect_object (priv->engine,
                           "unload-plugin",
                           G_CALLBACK (plugin_loaded_toggled_cb),
                           store,
                           G_CONNECT_AFTER);

  bean_ctk_plugin_manager_store_reload (store);

  G_OBJECT_CLASS (bean_ctk_plugin_manager_store_parent_class)->constructed (object);
}

static void
bean_ctk_plugin_manager_store_dispose (GObject *object)
{
  BeanCtkPluginManagerStore *store = BEAN_CTK_PLUGIN_MANAGER_STORE (object);
  BeanCtkPluginManagerStorePrivate *priv = GET_PRIV (store);

  g_clear_object (&priv->engine);

  G_OBJECT_CLASS (bean_ctk_plugin_manager_store_parent_class)->dispose (object);
}

static void
bean_ctk_plugin_manager_store_class_init (BeanCtkPluginManagerStoreClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = bean_ctk_plugin_manager_store_set_property;
  object_class->get_property = bean_ctk_plugin_manager_store_get_property;
  object_class->constructed = bean_ctk_plugin_manager_store_constructed;
  object_class->dispose = bean_ctk_plugin_manager_store_dispose;

  /*
   * BeanCtkPLuginManagerStore:engine:
   *
   * The #BeanEngine this store is attached to.
   */
  properties[PROP_ENGINE] =
    g_param_spec_object ("engine",
                         "engine",
                         "The BeanEngine this store is attached to",
                         BEAN_TYPE_ENGINE,
                         G_PARAM_READWRITE |
                         G_PARAM_CONSTRUCT_ONLY |
                         G_PARAM_STATIC_STRINGS);

  g_object_class_install_properties (object_class, N_PROPERTIES, properties);
}

/*
 * bean_ctk_plugin_manager_store_new:
 * @engine: (allow-none): A #BeanEngine, or %NULL.
 *
 * Creates a new plugin manager store for the given #BeanEngine.
 *
 * If @engine is %NULL, then the default engine will be used.
 *
 * Returns: the new #BeanCtkPluginManagerStore.
 */
BeanCtkPluginManagerStore  *
bean_ctk_plugin_manager_store_new (BeanEngine *engine)
{
  g_return_val_if_fail (engine == NULL || BEAN_IS_ENGINE (engine), NULL);

  return BEAN_CTK_PLUGIN_MANAGER_STORE (g_object_new (BEAN_CTK_TYPE_PLUGIN_MANAGER_STORE,
                                                      "engine", engine,
                                                      NULL));
}

/*
 * bean_ctk_plugin_manager_store_reload:
 * @store: A #BeanCtkPluginManagerStore.
 *
 * Reloads the list of plugins.
 */
void
bean_ctk_plugin_manager_store_reload (BeanCtkPluginManagerStore *store)
{
  BeanCtkPluginManagerStorePrivate *priv = GET_PRIV (store);
  CtkListStore *list_store;
  const GList *plugins;
  CtkTreeIter iter;

  g_return_if_fail (BEAN_CTK_IS_PLUGIN_MANAGER_STORE (store));

  list_store = CTK_LIST_STORE (store);

  ctk_list_store_clear (list_store);

  plugins = bean_engine_get_plugin_list (priv->engine);

  while (plugins != NULL)
    {
      BeanPluginInfo *info;

      info = BEAN_PLUGIN_INFO (plugins->data);

      if (!bean_plugin_info_is_hidden (info))
        {
          ctk_list_store_append (list_store, &iter);
          update_plugin (store, &iter, info);
        }

      plugins = plugins->next;
    }
}

/*
 * bean_ctk_plugin_manager_store_set_enabled:
 * @store: A #BeanCtkPluginManagerStore.
 * @iter: A #CtkTreeIter.
 * @enabled: If the plugin should be enabled.
 *
 * Sets if the plugin at @iter should be enabled.
 */
void
bean_ctk_plugin_manager_store_set_enabled (BeanCtkPluginManagerStore *store,
                                           CtkTreeIter               *iter,
                                           gboolean                   enabled)
{
  BeanCtkPluginManagerStorePrivate *priv = GET_PRIV (store);
  BeanPluginInfo *info;

  g_return_if_fail (BEAN_CTK_IS_PLUGIN_MANAGER_STORE (store));
  g_return_if_fail (iter != NULL);
  g_return_if_fail (bean_ctk_plugin_manager_store_can_enable (store, iter));

  info = bean_ctk_plugin_manager_store_get_plugin (store, iter);
  g_return_if_fail (info != NULL);

  if (enabled)
    {
      bean_engine_load_plugin (priv->engine, info);
    }
  else
    {
      bean_engine_unload_plugin (priv->engine, info);
    }

  /* Don't need to manually update the plugin as
   * BeanEngine::{load,unload}-plugin are connected to
   */
}

/*
 * bean_ctk_plugin_manager_store_set_enabled:
 * @store: A #BeanCtkPluginManagerStore.
 * @iter: A #CtkTreeIter.
 *
 * Returns if the plugin at @iter is enabled.
 *
 * Returns: if the plugin at @iter is enabled.
 */
gboolean
bean_ctk_plugin_manager_store_get_enabled (BeanCtkPluginManagerStore *store,
                                           CtkTreeIter               *iter)
{
  GValue value = G_VALUE_INIT;
  gboolean enabled;

  g_return_val_if_fail (BEAN_CTK_IS_PLUGIN_MANAGER_STORE (store), FALSE);
  g_return_val_if_fail (iter != NULL, FALSE);

  ctk_tree_model_get_value (CTK_TREE_MODEL (store), iter,
                            BEAN_CTK_PLUGIN_MANAGER_STORE_ENABLED_COLUMN, &value);

  g_return_val_if_fail (G_VALUE_HOLDS_BOOLEAN (&value), FALSE);
  enabled = g_value_get_boolean (&value);

  g_value_unset (&value);

  return enabled;
}

/*
 * bean_ctk_plugin_manager_store_set_all_enabled:
 * @store: A #BeanCtkPluginManagerStore.
 * @enabled: If all the plugins should be enabled.
 *
 * Sets if all the plugins should be enabled.
 */
void
bean_ctk_plugin_manager_store_set_all_enabled (BeanCtkPluginManagerStore *store,
                                               gboolean                   enabled)
{
  CtkTreeModel *model;
  CtkTreeIter iter;

  g_return_if_fail (BEAN_CTK_IS_PLUGIN_MANAGER_STORE (store));

  model = CTK_TREE_MODEL (store);

  if (!ctk_tree_model_get_iter_first (model, &iter))
    return;

  do
    {
      if (bean_ctk_plugin_manager_store_can_enable (store, &iter))
        bean_ctk_plugin_manager_store_set_enabled (store, &iter, enabled);
    }
  while (ctk_tree_model_iter_next (model, &iter));
}

/*
 * bean_ctk_plugin_manager_store_toggle_enabled:
 * @store: A #BeanCtkPluginManagerStore.
 * @iter: A #CtkTreeIter.
 *
 * Toggles the if the plugin should should be enabled.
 */
void
bean_ctk_plugin_manager_store_toggle_enabled (BeanCtkPluginManagerStore *store,
                                              CtkTreeIter               *iter)
{
  gboolean enabled;

  g_return_if_fail (BEAN_CTK_IS_PLUGIN_MANAGER_STORE (store));
  g_return_if_fail (iter != NULL);

  enabled = bean_ctk_plugin_manager_store_get_enabled (store, iter);

  bean_ctk_plugin_manager_store_set_enabled (store, iter, !enabled);
}

/*
 * bean_ctk_plugin_manager_store_can_enabled:
 * @store: A #BeanCtkPluginManagerStore.
 * @iter: A #CtkTreeIter.
 *
 * Returns if the plugin at @iter can be enabled.
 * Note: that while a plugin may be enableable there are other factors
 * that can cause it to not be enabled.
 *
 * Returns: if the plugin can be enabled.
 */
gboolean
bean_ctk_plugin_manager_store_can_enable (BeanCtkPluginManagerStore *store,
                                          CtkTreeIter               *iter)
{
  GValue value = G_VALUE_INIT;
  gboolean can_enable;

  g_return_val_if_fail (BEAN_CTK_IS_PLUGIN_MANAGER_STORE (store), FALSE);
  g_return_val_if_fail (iter != NULL, FALSE);

  ctk_tree_model_get_value (CTK_TREE_MODEL (store), iter,
                            BEAN_CTK_PLUGIN_MANAGER_STORE_CAN_ENABLE_COLUMN, &value);

  g_return_val_if_fail (G_VALUE_HOLDS_BOOLEAN (&value), FALSE);
  can_enable = g_value_get_boolean (&value);

  g_value_unset (&value);

  return can_enable;
}

/*
 * bean_ctk_plugin_manager_store_get_plugin:
 * @store: A #BeanCtkPluginManagerStore.
 * @iter: A #CtkTreeIter.
 *
 * Returns the plugin at @iter.
 *
 * Returns: the plugin at @iter.
 */
BeanPluginInfo *
bean_ctk_plugin_manager_store_get_plugin (BeanCtkPluginManagerStore *store,
                                          CtkTreeIter               *iter)
{
  GValue value = G_VALUE_INIT;
  BeanPluginInfo *info;

  g_return_val_if_fail (BEAN_CTK_IS_PLUGIN_MANAGER_STORE (store), NULL);
  g_return_val_if_fail (iter != NULL, NULL);

  ctk_tree_model_get_value (CTK_TREE_MODEL (store), iter,
                            BEAN_CTK_PLUGIN_MANAGER_STORE_PLUGIN_COLUMN, &value);

  g_return_val_if_fail (G_VALUE_HOLDS_POINTER (&value), NULL);
  info = g_value_get_pointer (&value);

  g_value_unset (&value);

  /* We register it as a pointer instead
   * of a boxed so no need to unref it
   */
  return info;
}

/*
 * bean_ctk_plugin_manager_store_get_iter_from_plugin:
 * @store: A #BeanCtkPluginManagerStore.
 * @iter: A #CtkTreeIter.
 * @info: A #BeanPluginInfo.
 *
 * Sets @iter to the @info.
 *
 * Returns: if @iter was set.
 */
gboolean
bean_ctk_plugin_manager_store_get_iter_from_plugin (BeanCtkPluginManagerStore *store,
                                                    CtkTreeIter               *iter,
                                                    const BeanPluginInfo      *info)
{
  CtkTreeModel *model = CTK_TREE_MODEL (store);
  gboolean found = FALSE;

  g_return_val_if_fail (BEAN_CTK_IS_PLUGIN_MANAGER_STORE (store), FALSE);
  g_return_val_if_fail (iter != NULL, FALSE);
  g_return_val_if_fail (info != NULL, FALSE);

  if (ctk_tree_model_get_iter_first (model, iter))
    {
      BeanPluginInfo *current_info;

      do
        {
          current_info = bean_ctk_plugin_manager_store_get_plugin (store, iter);

          found = (info == current_info);
        }
      while (!found && ctk_tree_model_iter_next (model, iter));
    }

  return found;
}
