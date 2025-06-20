/*
 * bean-ctk-disable-plugins-dialog.c
 * This file is part of libbean
 *
 * Copyright (C) 2011 Garrett Regier
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

#include <libbean/bean-i18n-priv.h>
#include <libbean/bean-plugin-info.h>

#include "bean-ctk-disable-plugins-dialog.h"

enum {
  PLUGIN_INFO_NAME_COLUMN = 0
};

typedef struct {
  BeanPluginInfo *plugin_info;
  GList *dep_plugins;
} BeanCtkDisablePluginsDialogPrivate;

/* Properties */
enum {
  PROP_0,
  PROP_PLUGIN_INFO,
  PROP_DEPENDENT_PLUGINS,
  N_PROPERTIES
};

static GParamSpec *properties[N_PROPERTIES] = { NULL };

G_DEFINE_TYPE_WITH_PRIVATE (BeanCtkDisablePluginsDialog,
                            bean_ctk_disable_plugins_dialog,
                            CTK_TYPE_MESSAGE_DIALOG)

#define GET_PRIV(o) \
  (bean_ctk_disable_plugins_dialog_get_instance_private (o))

static gint
model_name_sort_func (CtkListStore *store,
		      CtkTreeIter  *iter1,
		      CtkTreeIter  *iter2 G_GNUC_UNUSED,
		      gpointer      user_data G_GNUC_UNUSED)
{
  gchar *name1, *name2;
  gint retval;

  ctk_tree_model_get (CTK_TREE_MODEL (store), iter1,
                      PLUGIN_INFO_NAME_COLUMN, &name1,
                      -1);

  ctk_tree_model_get (CTK_TREE_MODEL (store), iter1,
                      PLUGIN_INFO_NAME_COLUMN, &name2,
                      -1);

  retval = g_utf8_collate (name1, name2);

  g_free (name1);
  g_free (name2);

  return retval;
}

static void
build_multiple_dependent_plugins (BeanCtkDisablePluginsDialog *dialog)
{
  BeanCtkDisablePluginsDialogPrivate *priv = GET_PRIV (dialog);
  gchar *message;
  CtkWidget *message_area;
  CtkWidget *sw;
  CtkListStore *store;
  GList *dep_plugin;
  CtkWidget *tree_view;
  CtkCellRenderer *cell;

  message = g_strconcat ("<span weight=\"bold\" size=\"larger\">",
                         _("Additional plugins must be disabled"),
                         "</span>", NULL);

  ctk_message_dialog_set_markup (CTK_MESSAGE_DIALOG (dialog), message);
  g_free (message);

  ctk_message_dialog_format_secondary_text (CTK_MESSAGE_DIALOG (dialog),
      _("The following plugins depend on “%s” and will also be disabled:"),
      bean_plugin_info_get_name (priv->plugin_info));

  message_area = ctk_message_dialog_get_message_area (CTK_MESSAGE_DIALOG (dialog));

  sw = ctk_scrolled_window_new (NULL, NULL);
  ctk_scrolled_window_set_shadow_type (CTK_SCROLLED_WINDOW (sw),
                                       CTK_SHADOW_IN);
  ctk_scrolled_window_set_policy (CTK_SCROLLED_WINDOW (sw),
                                  CTK_POLICY_AUTOMATIC, CTK_POLICY_AUTOMATIC);
  ctk_box_pack_start (CTK_BOX (message_area), sw, TRUE, TRUE, 0);

  store = ctk_list_store_new (1, G_TYPE_STRING);

  /* Sort on the plugin names */
  ctk_tree_sortable_set_default_sort_func (CTK_TREE_SORTABLE (store),
                                           (CtkTreeIterCompareFunc) model_name_sort_func,
                                           NULL, NULL);
  ctk_tree_sortable_set_sort_column_id (CTK_TREE_SORTABLE (store),
                                        CTK_TREE_SORTABLE_DEFAULT_SORT_COLUMN_ID,
                                        CTK_SORT_ASCENDING);

  for (dep_plugin = priv->dep_plugins; dep_plugin != NULL;
       dep_plugin = dep_plugin->next)
    {
      BeanPluginInfo *plugin = (BeanPluginInfo *) dep_plugin->data;
      CtkTreeIter iter;

      ctk_list_store_append (store, &iter);
      ctk_list_store_set (store, &iter,
                          PLUGIN_INFO_NAME_COLUMN, bean_plugin_info_get_name (plugin),
                          -1);
    }

  tree_view = ctk_tree_view_new_with_model (CTK_TREE_MODEL (store));
  ctk_tree_view_set_headers_visible (CTK_TREE_VIEW (tree_view), FALSE);
  ctk_tree_view_set_enable_search (CTK_TREE_VIEW (tree_view), FALSE);
  ctk_widget_set_size_request (tree_view, 260, 120);
  ctk_container_add (CTK_CONTAINER (sw), tree_view);

  cell = ctk_cell_renderer_text_new ();
  ctk_tree_view_insert_column_with_attributes (CTK_TREE_VIEW (tree_view),
                                               0, _("Plugins"),
                                               cell,
                                               "text", PLUGIN_INFO_NAME_COLUMN,
                                               NULL);

  g_object_unref (store);

  ctk_widget_show_all (sw);
}

static void
build_single_dependent_plugin (BeanCtkDisablePluginsDialog *dialog)
{
  BeanCtkDisablePluginsDialogPrivate *priv = GET_PRIV (dialog);
  gchar *message;

  message = g_strconcat ("<span weight=\"bold\" size=\"larger\">",
                         _("An additional plugin must be disabled"),
                         "</span>", NULL);

  ctk_message_dialog_set_markup (CTK_MESSAGE_DIALOG (dialog), message);
  g_free (message);

  ctk_message_dialog_format_secondary_text (CTK_MESSAGE_DIALOG (dialog),
      _("The “%s” plugin depends on the “%s” plugin.\n"
        "If you disable “%s”, “%s” will also be disabled."),
      bean_plugin_info_get_name (priv->plugin_info),
      bean_plugin_info_get_name (priv->dep_plugins->data),
      bean_plugin_info_get_name (priv->plugin_info),
      bean_plugin_info_get_name (priv->dep_plugins->data));
}

static void
bean_ctk_disable_plugins_dialog_init (BeanCtkDisablePluginsDialog *dialog)
{
  ctk_window_set_modal (CTK_WINDOW (dialog), TRUE);

  ctk_dialog_add_button (CTK_DIALOG (dialog),
                         _("_Cancel"), CTK_RESPONSE_CANCEL);
  ctk_dialog_add_button (CTK_DIALOG (dialog),
                         _("Disable Plugins"), CTK_RESPONSE_OK);
}

static void
bean_ctk_disable_plugins_dialog_set_property (GObject      *object,
                                              guint         prop_id,
                                              const GValue *value,
                                              GParamSpec   *pspec)
{
  BeanCtkDisablePluginsDialog *dialog = BEAN_CTK_DISABLE_PLUGINS_DIALOG (object);
  BeanCtkDisablePluginsDialogPrivate *priv = GET_PRIV (dialog);

  switch (prop_id)
    {
    case PROP_PLUGIN_INFO:
      priv->plugin_info = g_value_get_pointer (value);
      break;
    case PROP_DEPENDENT_PLUGINS:
      priv->dep_plugins = g_value_get_pointer (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
bean_ctk_disable_plugins_dialog_get_property (GObject    *object,
                                              guint       prop_id,
                                              GValue     *value,
                                              GParamSpec *pspec)
{
  BeanCtkDisablePluginsDialog *dialog = BEAN_CTK_DISABLE_PLUGINS_DIALOG (object);
  BeanCtkDisablePluginsDialogPrivate *priv = GET_PRIV (dialog);

  switch (prop_id)
    {
    case PROP_PLUGIN_INFO:
      g_value_set_pointer (value, priv->plugin_info);
      break;
    case PROP_DEPENDENT_PLUGINS:
      g_value_set_pointer (value, priv->dep_plugins);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
bean_ctk_disable_plugins_dialog_constructed (GObject *object)
{
  BeanCtkDisablePluginsDialog *dialog = BEAN_CTK_DISABLE_PLUGINS_DIALOG (object);
  BeanCtkDisablePluginsDialogPrivate *priv = GET_PRIV (dialog);

  if (priv->dep_plugins->next == NULL)
    build_single_dependent_plugin (dialog);
  else
    build_multiple_dependent_plugins (dialog);

  G_OBJECT_CLASS (bean_ctk_disable_plugins_dialog_parent_class)->constructed (object);
}

static void
bean_ctk_disable_plugins_dialog_finalize (GObject *object)
{
  BeanCtkDisablePluginsDialog *dialog = BEAN_CTK_DISABLE_PLUGINS_DIALOG (object);
  BeanCtkDisablePluginsDialogPrivate *priv = GET_PRIV (dialog);

  g_list_free (priv->dep_plugins);

  G_OBJECT_CLASS (bean_ctk_disable_plugins_dialog_parent_class)->finalize (object);
}

static void
bean_ctk_disable_plugins_dialog_class_init (BeanCtkDisablePluginsDialogClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->get_property = bean_ctk_disable_plugins_dialog_get_property;
  object_class->set_property = bean_ctk_disable_plugins_dialog_set_property;
  object_class->constructed = bean_ctk_disable_plugins_dialog_constructed;
  object_class->finalize = bean_ctk_disable_plugins_dialog_finalize;

  properties[PROP_PLUGIN_INFO] =
  g_param_spec_pointer ("plugin-info",
                        "Plugin Information",
                        "Plugin that is being disabled",
                        G_PARAM_READWRITE |
                        G_PARAM_CONSTRUCT_ONLY |
                        G_PARAM_STATIC_STRINGS);

  properties[PROP_DEPENDENT_PLUGINS] =
    g_param_spec_pointer ("dependent-plugins",
                          "Dependant plugins",
                          "Dependant plugins",
                          G_PARAM_READWRITE |
                          G_PARAM_CONSTRUCT_ONLY |
                          G_PARAM_STATIC_STRINGS);

  g_object_class_install_properties (object_class, N_PROPERTIES, properties);
}

/*
 * bean_ctk_disable_plugins_dialog_new:
 * @parent: transient window.
 * @info: the #BeanPluginInfo being disabled.
 * @dep_plugins: (transfer container) (element-type Bean.PluginInfo):
 *  list of plugins that are dependent on @info.
 *
 * Creates a new #BeanCtkDisablePluginsDialog.
 *
 * Returns: the new #BeanCtkDisablePluginsDialog.
 */
CtkWidget  *
bean_ctk_disable_plugins_dialog_new (CtkWindow      *parent,
                                     BeanPluginInfo *info,
                                     GList          *dep_plugins)
{
  return CTK_WIDGET (g_object_new (BEAN_CTK_TYPE_DISABLE_PLUGINS_DIALOG,
                                   "transient-for", parent,
                                   "plugin-info", info,
                                   "dependent-plugins", dep_plugins,
                                   "message-type", CTK_MESSAGE_QUESTION,
                                   NULL));
}
