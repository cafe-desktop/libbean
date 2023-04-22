/*
 * bean-plugin-manager-view.c
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

#include <string.h>

#include <libbean/bean-engine.h>
#include <libbean/bean-i18n-priv.h>

#include "bean-ctk-plugin-manager-view.h"
#include "bean-ctk-disable-plugins-dialog.h"
#include "bean-ctk-plugin-manager-store.h"
#include "bean-ctk-configurable.h"

/**
 * SECTION:bean-ctk-plugin-manager-view
 * @short_description: Management tree view for plugins.
 *
 * The #BeanCtkPluginManagerView is a tree view that can be used to manage
 * plugins, i.e. load or unload them, and see some pieces of information.
 *
 * The only thing you need to do as an application writer if you wish
 * to use the view to display your plugins is to instantiate it using
 * bean_ctk_plugin_manager_view_new() and pack it into another
 * widget or a window.
 *
 * Note: Changing the model of the view is not supported.
 *
 **/

struct _BeanCtkPluginManagerViewPrivate {
  BeanEngine *engine;

  BeanCtkPluginManagerStore *store;

  CtkWidget *popup_menu;

  guint show_builtin : 1;
};

/* Properties */
enum {
  PROP_0,
  PROP_ENGINE,
  PROP_SHOW_BUILTIN,
  N_PROPERTIES
};

/* Signals */
enum {
  POPULATE_POPUP,
  LAST_SIGNAL
};

static guint signals[LAST_SIGNAL];
static GParamSpec *properties[N_PROPERTIES] = { NULL };

G_DEFINE_TYPE_WITH_PRIVATE (BeanCtkPluginManagerView,
                            bean_ctk_plugin_manager_view,
                            CTK_TYPE_TREE_VIEW)

#define GET_PRIV(o) \
  (bean_ctk_plugin_manager_view_get_instance_private (o))

static void
convert_iter_to_child_iter (BeanCtkPluginManagerView *view,
                            CtkTreeIter              *iter)
{
  BeanCtkPluginManagerViewPrivate *priv = GET_PRIV (view);

  if (!priv->show_builtin)
    {
      CtkTreeModel *model;
      CtkTreeIter child_iter;

      model = ctk_tree_view_get_model (CTK_TREE_VIEW (view));

      ctk_tree_model_filter_convert_iter_to_child_iter (CTK_TREE_MODEL_FILTER (model),
                                                        &child_iter, iter);

      *iter = child_iter;
    }
}

static gboolean
convert_child_iter_to_iter (BeanCtkPluginManagerView *view,
                            CtkTreeIter              *child_iter)
{
  BeanCtkPluginManagerViewPrivate *priv = GET_PRIV (view);
  gboolean success = TRUE;

  if (!priv->show_builtin)
    {
      CtkTreeModel *model;
      CtkTreeIter iter;

      model = ctk_tree_view_get_model (CTK_TREE_VIEW (view));

      success = ctk_tree_model_filter_convert_child_iter_to_iter (CTK_TREE_MODEL_FILTER (model),
                                                                  &iter, child_iter);

      if (success)
        *child_iter = iter;
    }

  return success;
}

static GList *
get_dependant_plugins (BeanCtkPluginManagerView *view,
                       BeanPluginInfo           *info)
{
  BeanCtkPluginManagerViewPrivate *priv = GET_PRIV (view);
  const gchar *module_name;
  const GList *plugins;
  GList *dep_plugins = NULL;

  module_name = bean_plugin_info_get_module_name (info);
  plugins = bean_engine_get_plugin_list (priv->engine);

  for (; plugins != NULL; plugins = plugins->next)
    {
      BeanPluginInfo *plugin = (BeanPluginInfo *) plugins->data;

      if (bean_plugin_info_is_hidden (plugin) ||
          !bean_plugin_info_is_loaded (plugin))
        continue;

      /* Don't add builtin plugins if they are not shown */
      if (!priv->show_builtin && bean_plugin_info_is_builtin (plugin))
        continue;

      if (bean_plugin_info_has_dependency (plugin, module_name))
        dep_plugins = g_list_prepend (dep_plugins, plugin);
    }

  return dep_plugins;
}

static void
toggle_enabled (BeanCtkPluginManagerView *view,
                CtkTreeIter              *iter)
{
  BeanCtkPluginManagerViewPrivate *priv = GET_PRIV (view);
  BeanPluginInfo *info;

  info = bean_ctk_plugin_manager_store_get_plugin (priv->store, iter);

  if (bean_plugin_info_is_loaded (info))
    {
      GList *dep_plugins;

      dep_plugins = get_dependant_plugins (view, info);

      if (dep_plugins != NULL)
        {
          CtkWindow *parent;
          CtkWidget *dialog;
          gint response;

          parent = CTK_WINDOW (ctk_widget_get_toplevel (CTK_WIDGET (view)));

          /* The dialog takes the list so don't free it */
          dialog = bean_ctk_disable_plugins_dialog_new (parent, info,
                                                        dep_plugins);

          response = ctk_dialog_run (CTK_DIALOG (dialog));

          ctk_widget_destroy (dialog);

          if (response != CTK_RESPONSE_OK)
            return;
        }
    }

  bean_ctk_plugin_manager_store_toggle_enabled (priv->store, iter);
}

static void
plugin_list_changed_cb (BeanEngine               *engine,
                        GParamSpec               *pspec,
                        BeanCtkPluginManagerView *view)
{
  BeanCtkPluginManagerViewPrivate *priv = GET_PRIV (view);
  BeanPluginInfo *info;

  info = bean_ctk_plugin_manager_view_get_selected_plugin (view);

  bean_ctk_plugin_manager_store_reload (priv->store);

  if (info != NULL)
    bean_ctk_plugin_manager_view_set_selected_plugin (view, info);
}

static gboolean
filter_builtins_visible (BeanCtkPluginManagerStore *store,
                         CtkTreeIter               *iter,
                         BeanCtkPluginManagerView  *view)
{
  BeanCtkPluginManagerViewPrivate *priv = GET_PRIV (view);
  BeanPluginInfo *info;

  /* We never filter showing builtins */
  g_assert (priv->show_builtin == FALSE);

  info = bean_ctk_plugin_manager_store_get_plugin (store, iter);

  if (info == NULL)
    return FALSE;

  return !bean_plugin_info_is_builtin (info);
}

static void
enabled_toggled_cb (CtkCellRendererToggle    *cell,
                    gchar                    *path_str,
                    BeanCtkPluginManagerView *view)
{
  CtkTreeModel *model;
  CtkTreePath *path;
  CtkTreeIter iter;

  model = ctk_tree_view_get_model (CTK_TREE_VIEW (view));
  path = ctk_tree_path_new_from_string (path_str);

  if (ctk_tree_model_get_iter (model, &iter, path))
    {
      convert_iter_to_child_iter (view, &iter);
      toggle_enabled (view, &iter);
    }

  ctk_tree_path_free (path);
}

/* Callback used as the interactive search comparison function */
static gboolean
name_search_cb (CtkTreeModel             *model,
                gint                      column,
                const gchar              *key,
                CtkTreeIter              *iter,
                BeanCtkPluginManagerView *view)
{
  BeanCtkPluginManagerViewPrivate *priv = GET_PRIV (view);
  CtkTreeIter child_iter = *iter;
  BeanPluginInfo *info;
  gchar *normalized_string;
  gchar *normalized_key;
  gchar *case_normalized_string;
  gchar *case_normalized_key;
  gint key_len;
  gboolean retval;

  convert_iter_to_child_iter (view, &child_iter);
  info = bean_ctk_plugin_manager_store_get_plugin (priv->store, &child_iter);

  if (info == NULL)
    return FALSE;

  normalized_string = g_utf8_normalize (bean_plugin_info_get_name (info), -1, G_NORMALIZE_ALL);
  normalized_key = g_utf8_normalize (key, -1, G_NORMALIZE_ALL);
  case_normalized_string = g_utf8_casefold (normalized_string, -1);
  case_normalized_key = g_utf8_casefold (normalized_key, -1);

  key_len = strlen (case_normalized_key);

  /* Oddly enough, this callback must return whether to stop the search
   * because we found a match, not whether we actually matched.
   */
  retval = strncmp (case_normalized_key, case_normalized_string, key_len) != 0;

  g_free (normalized_key);
  g_free (normalized_string);
  g_free (case_normalized_key);
  g_free (case_normalized_string);

  return retval;
}

static void
enabled_menu_cb (CtkMenu                  *menu,
                 BeanCtkPluginManagerView *view)
{
  CtkTreeIter iter;
  CtkTreeSelection *selection;

  selection = ctk_tree_view_get_selection (CTK_TREE_VIEW (view));

  g_return_if_fail (ctk_tree_selection_get_selected (selection, NULL, &iter));

  convert_iter_to_child_iter (view, &iter);

  toggle_enabled (view, &iter);
}

static void
enable_all_menu_cb (CtkMenu                  *menu,
                    BeanCtkPluginManagerView *view)
{
  BeanCtkPluginManagerViewPrivate *priv = GET_PRIV (view);

  bean_ctk_plugin_manager_store_set_all_enabled (priv->store, TRUE);
}

static void
disable_all_menu_cb (CtkMenu                  *menu,
                     BeanCtkPluginManagerView *view)
{
  BeanCtkPluginManagerViewPrivate *priv = GET_PRIV (view);

  bean_ctk_plugin_manager_store_set_all_enabled (priv->store, FALSE);
}

static CtkWidget *
create_popup_menu (BeanCtkPluginManagerView *view)
{
  BeanPluginInfo *info;
  CtkWidget *menu;
  CtkWidget *item;

  info = bean_ctk_plugin_manager_view_get_selected_plugin (view);

  if (info == NULL)
    return NULL;

  menu = ctk_menu_new ();

  item = ctk_check_menu_item_new_with_mnemonic (_("_Enabled"));
  ctk_check_menu_item_set_active (CTK_CHECK_MENU_ITEM (item),
                                  bean_plugin_info_is_loaded (info));
  g_signal_connect (item, "toggled", G_CALLBACK (enabled_menu_cb), view);
  ctk_widget_set_sensitive (item, bean_plugin_info_is_available (info, NULL) &&
                                  !bean_plugin_info_is_builtin (info));
  ctk_menu_shell_append (CTK_MENU_SHELL (menu), item);

  item = ctk_separator_menu_item_new ();
  ctk_menu_shell_append (CTK_MENU_SHELL (menu), item);

  item = ctk_menu_item_new_with_mnemonic (_("E_nable All"));
  g_signal_connect (item, "activate", G_CALLBACK (enable_all_menu_cb), view);
  ctk_menu_shell_append (CTK_MENU_SHELL (menu), item);

  item = ctk_menu_item_new_with_mnemonic (_("_Disable All"));
  g_signal_connect (item, "activate", G_CALLBACK (disable_all_menu_cb), view);
  ctk_menu_shell_append (CTK_MENU_SHELL (menu), item);

  g_signal_emit (view, signals[POPULATE_POPUP], 0, menu);

  ctk_widget_show_all (menu);

  return menu;
}

static void
popup_menu_detach (BeanCtkPluginManagerView *view,
                   CtkMenu                  *menu)
{
  BeanCtkPluginManagerViewPrivate *priv = GET_PRIV (view);

  priv->popup_menu = NULL;
}

static void
get_selected_area (CtkTreeView  *tree_view,
                   CdkRectangle *area)
{
  CtkTreeSelection *selection;
  CtkTreeIter iter;

  selection = ctk_tree_view_get_selection (tree_view);

  if (ctk_tree_selection_get_selected (selection, NULL, &iter))
    {
      CtkTreeModel *model;
      CtkTreePath *path;

      model = ctk_tree_view_get_model (tree_view);
      path = ctk_tree_model_get_path (model, &iter);
      ctk_tree_view_get_cell_area (tree_view,
                                   path,
                                   ctk_tree_view_get_column (tree_view, 0),
                                   area);
      ctk_tree_path_free (path);
    }
  else
    {
      ctk_widget_get_allocation (CTK_WIDGET (tree_view), area);
    }
}

#if !CTK_CHECK_VERSION(3, 22, 0)
static void
menu_position_under_tree_view (CtkMenu     *menu,
                               gint        *x,
                               gint        *y,
                               gboolean    *push_in,
                               CtkTreeView *tree_view)
{
  CtkTreeSelection *selection;
  CtkTreeIter iter;
  CdkWindow *window;

  selection = ctk_tree_view_get_selection (tree_view);

  window = ctk_widget_get_window (CTK_WIDGET (tree_view));
  cdk_window_get_origin (window, x, y);

  if (ctk_tree_selection_get_selected (selection, NULL, &iter))
    {
      CtkTreeModel *model;
      CtkTreePath *path;
      CdkRectangle rect;

      model = ctk_tree_view_get_model (tree_view);
      path = ctk_tree_model_get_path (model, &iter);
      ctk_tree_view_get_cell_area (tree_view,
                                   path,
                                   ctk_tree_view_get_column (tree_view, 0), /* FIXME 0 for RTL ? */
                                   &rect);
      ctk_tree_path_free (path);

      *x += rect.x;
      *y += rect.y + rect.height;

      if (ctk_widget_get_direction (CTK_WIDGET (tree_view)) == CTK_TEXT_DIR_RTL)
        {
          CtkRequisition requisition;
          ctk_widget_get_preferred_size (CTK_WIDGET (menu), &requisition,
                                         NULL);
          *x += rect.width - requisition.width;
        }
    }
  else
    {
      CtkAllocation allocation;

      ctk_widget_get_allocation (CTK_WIDGET (tree_view), &allocation);

      *x += allocation.x;
      *y += allocation.y;

      if (ctk_widget_get_direction (CTK_WIDGET (tree_view)) == CTK_TEXT_DIR_RTL)
        {
          CtkRequisition requisition;

          ctk_widget_get_preferred_size (CTK_WIDGET (menu), &requisition,
                                         NULL);

          *x += allocation.width - requisition.width;
        }
    }

  *push_in = TRUE;
}
#endif

static gboolean
show_popup_menu (CtkTreeView              *tree_view,
                 BeanCtkPluginManagerView *view,
                 CdkEventButton           *event)
{
  BeanCtkPluginManagerViewPrivate *priv = GET_PRIV (view);

  if (priv->popup_menu)
    ctk_widget_destroy (priv->popup_menu);

  priv->popup_menu = create_popup_menu (view);

  if (priv->popup_menu == NULL)
    return FALSE;

  ctk_menu_attach_to_widget (CTK_MENU (priv->popup_menu),
                             CTK_WIDGET (view),
                             (CtkMenuDetachFunc) popup_menu_detach);

  if (event != NULL)
    {
#if CTK_CHECK_VERSION(3, 22, 0)
      ctk_menu_popup_at_pointer (CTK_MENU (priv->popup_menu),
                                (const CdkEvent *)event);
#else
      ctk_menu_popup (CTK_MENU (priv->popup_menu), NULL, NULL,
                      NULL, NULL, event->button, event->time);
#endif
    }
  else
    {
#if CTK_CHECK_VERSION(3, 22, 0)
      CdkRectangle cell_area;

      get_selected_area (CTK_TREE_VIEW (view), &cell_area);

      ctk_menu_popup_at_rect (CTK_MENU (priv->popup_menu),
                              ctk_widget_get_window (CTK_WIDGET (view)),
                              &cell_area,
                              CDK_GRAVITY_SOUTH_WEST,
                              CDK_GRAVITY_NORTH_WEST,
                              (const CdkEvent *)event);

#else
      ctk_menu_popup (CTK_MENU (priv->popup_menu), NULL, NULL,
                      (CtkMenuPositionFunc) menu_position_under_tree_view,
                      view, 0, ctk_get_current_event_time ());
#endif

      ctk_menu_shell_select_first (CTK_MENU_SHELL (priv->popup_menu),
                                   FALSE);
    }

  return TRUE;
}

static void
plugin_icon_data_func (CtkTreeViewColumn *column,
                       CtkCellRenderer   *cell,
                       CtkTreeModel      *model,
                       CtkTreeIter       *iter)
{
  GIcon *icon_gicon;
  gchar *icon_stock_id;

  ctk_tree_model_get (model, iter,
    BEAN_CTK_PLUGIN_MANAGER_STORE_ICON_GICON_COLUMN, &icon_gicon,
    BEAN_CTK_PLUGIN_MANAGER_STORE_ICON_STOCK_ID_COLUMN, &icon_stock_id,
    -1);

  if (icon_gicon == NULL)
    {
      g_object_set (cell, "stock-id", icon_stock_id, NULL);
    }
  else
    {
      g_object_set (cell, "gicon", icon_gicon, NULL);
      g_object_unref (icon_gicon);
    }

  g_free (icon_stock_id);
}

static void
bean_ctk_plugin_manager_view_init (BeanCtkPluginManagerView *view)
{
  CtkTreeViewColumn *column;
  CtkCellRenderer *cell;

  ctk_widget_set_has_tooltip (CTK_WIDGET (view), TRUE);

  ctk_tree_view_set_headers_visible (CTK_TREE_VIEW (view), FALSE);

  /* first column */
  column = ctk_tree_view_column_new ();
  ctk_tree_view_column_set_title (column, _("Enabled"));
  ctk_tree_view_column_set_resizable (column, FALSE);

  cell = ctk_cell_renderer_toggle_new ();
  ctk_tree_view_column_pack_start (column, cell, FALSE);
  g_object_set (cell, "xpad", 6, NULL);
  ctk_tree_view_column_set_attributes (column, cell,
                                       "active", BEAN_CTK_PLUGIN_MANAGER_STORE_ENABLED_COLUMN,
                                       "activatable", BEAN_CTK_PLUGIN_MANAGER_STORE_CAN_ENABLE_COLUMN,
                                       "sensitive", BEAN_CTK_PLUGIN_MANAGER_STORE_CAN_ENABLE_COLUMN,
                                       "visible", BEAN_CTK_PLUGIN_MANAGER_STORE_CAN_ENABLE_COLUMN,
                                       NULL);
  g_signal_connect (cell,
                    "toggled",
                    G_CALLBACK (enabled_toggled_cb),
                    view);

  ctk_tree_view_append_column (CTK_TREE_VIEW (view), column);

  /* second column */
  column = ctk_tree_view_column_new ();
  ctk_tree_view_column_set_title (column, _("Plugin"));
  ctk_tree_view_column_set_resizable (column, FALSE);

  cell = ctk_cell_renderer_pixbuf_new ();
  ctk_tree_view_column_pack_start (column, cell, FALSE);
  g_object_set (cell, "stock-size", CTK_ICON_SIZE_SMALL_TOOLBAR, NULL);
  ctk_tree_view_column_set_cell_data_func (column, cell,
                                           (CtkTreeCellDataFunc) plugin_icon_data_func,
                                           NULL, NULL);

  cell = ctk_cell_renderer_text_new ();
  ctk_tree_view_column_pack_start (column, cell, TRUE);
  g_object_set (cell, "ellipsize", PANGO_ELLIPSIZE_END, NULL);
  ctk_tree_view_column_set_attributes (column, cell,
                                       "sensitive", BEAN_CTK_PLUGIN_MANAGER_STORE_INFO_SENSITIVE_COLUMN,
                                       "markup", BEAN_CTK_PLUGIN_MANAGER_STORE_INFO_COLUMN,
                                       NULL);

  ctk_tree_view_column_set_spacing (column, 6);
  ctk_tree_view_append_column (CTK_TREE_VIEW (view), column);

  /* Enable search for our non-string column */
  ctk_tree_view_set_search_column (CTK_TREE_VIEW (view),
                                   BEAN_CTK_PLUGIN_MANAGER_STORE_PLUGIN_COLUMN);
  ctk_tree_view_set_search_equal_func (CTK_TREE_VIEW (view),
                                       (CtkTreeViewSearchEqualFunc) name_search_cb,
                                       view,
                                       NULL);
}

static gboolean
bean_ctk_plugin_manager_view_button_press_event (CtkWidget      *tree_view,
                                                 CdkEventButton *event)
{
  BeanCtkPluginManagerView *view = BEAN_CTK_PLUGIN_MANAGER_VIEW (tree_view);
  CtkWidgetClass *widget_class;
  gboolean handled;

  widget_class = CTK_WIDGET_CLASS (bean_ctk_plugin_manager_view_parent_class);

  /* The selection must by updated */
  handled = widget_class->button_press_event (tree_view, event);

  if (event->type != CDK_BUTTON_PRESS || event->button != 3 || !handled)
    return handled;

  return show_popup_menu (CTK_TREE_VIEW (tree_view), view, event);
}

static gboolean
bean_ctk_plugin_manager_view_popup_menu (CtkWidget *tree_view)
{
  BeanCtkPluginManagerView *view = BEAN_CTK_PLUGIN_MANAGER_VIEW (tree_view);

  return show_popup_menu (CTK_TREE_VIEW (tree_view), view, NULL);
}

static gboolean
bean_ctk_plugin_manager_view_query_tooltip (CtkWidget  *widget,
                                            gint        x,
                                            gint        y,
                                            gboolean    keyboard_mode,
                                            CtkTooltip *tooltip)
{
  BeanCtkPluginManagerView *view = BEAN_CTK_PLUGIN_MANAGER_VIEW (widget);
  BeanCtkPluginManagerViewPrivate *priv = GET_PRIV (view);
  gboolean is_row;
  CtkTreeIter iter;
  BeanPluginInfo *info;
  gchar *to_bold, *error_msg, *message;
  GError *error = NULL;

  is_row = ctk_tree_view_get_tooltip_context (CTK_TREE_VIEW (widget),
                                              &x, &y, keyboard_mode,
                                              NULL, NULL, &iter);

  if (!is_row)
    return FALSE;

  convert_iter_to_child_iter (view, &iter);

  info = bean_ctk_plugin_manager_store_get_plugin (priv->store, &iter);

  if (bean_plugin_info_is_available (info, &error))
    return FALSE;

  /* Avoid having markup in a translated string */
  to_bold = g_strdup_printf (_("The plugin “%s” could not be loaded"),
                             bean_plugin_info_get_name (info));

  /* Keep separate because some translations do special things
   * for the ':' and might accidentally not keep the space after it
   */
  error_msg = g_strdup_printf (_("An error occurred: %s"), error->message);

  message = g_strconcat ("<b>", to_bold, "</b>\n", error_msg, NULL);

  ctk_tooltip_set_markup (tooltip, message);

  g_free (message);
  g_free (error_msg);
  g_free (to_bold);
  g_error_free (error);

  return TRUE;
}

static void
bean_ctk_plugin_manager_view_row_activated (CtkTreeView       *tree_view,
                                            CtkTreePath       *path,
                                            CtkTreeViewColumn *column)
{
  BeanCtkPluginManagerView *view = BEAN_CTK_PLUGIN_MANAGER_VIEW (tree_view);
  BeanCtkPluginManagerViewPrivate *priv = GET_PRIV (view);
  CtkTreeIter iter;
  CtkTreeViewClass *tree_view_class;

  if (!ctk_tree_model_get_iter (ctk_tree_view_get_model (tree_view), &iter, path))
    return;

  convert_iter_to_child_iter (view, &iter);

  if (bean_ctk_plugin_manager_store_can_enable (priv->store, &iter))
    toggle_enabled (view, &iter);

  tree_view_class = CTK_TREE_VIEW_CLASS (bean_ctk_plugin_manager_view_parent_class);
  if (tree_view_class->row_activated != NULL)
    tree_view_class->row_activated (tree_view, path, column);
}

static void
bean_ctk_plugin_manager_view_set_property (GObject      *object,
                                           guint         prop_id,
                                           const GValue *value,
                                           GParamSpec   *pspec)
{
  BeanCtkPluginManagerView *view = BEAN_CTK_PLUGIN_MANAGER_VIEW (object);
  BeanCtkPluginManagerViewPrivate *priv = GET_PRIV (view);

  switch (prop_id)
    {
    case PROP_ENGINE:
      priv->engine = g_value_get_object (value);
      break;
    case PROP_SHOW_BUILTIN:
      bean_ctk_plugin_manager_view_set_show_builtin (view,
                                                     g_value_get_boolean (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
bean_ctk_plugin_manager_view_get_property (GObject    *object,
                                           guint       prop_id,
                                           GValue     *value,
                                           GParamSpec *pspec)
{
  BeanCtkPluginManagerView *view = BEAN_CTK_PLUGIN_MANAGER_VIEW (object);
  BeanCtkPluginManagerViewPrivate *priv = GET_PRIV (view);

  switch (prop_id)
    {
    case PROP_ENGINE:
      g_value_set_object (value, priv->engine);
      break;
    case PROP_SHOW_BUILTIN:
      g_value_set_boolean (value,
                           bean_ctk_plugin_manager_view_get_show_builtin (view));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
bean_ctk_plugin_manager_view_constructed (GObject *object)
{
  BeanCtkPluginManagerView *view = BEAN_CTK_PLUGIN_MANAGER_VIEW (object);
  BeanCtkPluginManagerViewPrivate *priv = GET_PRIV (view);

  if (priv->engine == NULL)
    priv->engine = bean_engine_get_default ();

  g_object_ref (priv->engine);

  priv->store = bean_ctk_plugin_manager_store_new (priv->engine);

  /* Properly set the model */
  priv->show_builtin = TRUE;
  bean_ctk_plugin_manager_view_set_show_builtin (view, FALSE);

  g_signal_connect_object (priv->engine,
                           "notify::plugin-list",
                           G_CALLBACK (plugin_list_changed_cb),
                           view,
                           0);

  G_OBJECT_CLASS (bean_ctk_plugin_manager_view_parent_class)->constructed (object);
}

static void
bean_ctk_plugin_manager_view_dispose (GObject *object)
{
  BeanCtkPluginManagerView *view = BEAN_CTK_PLUGIN_MANAGER_VIEW (object);
  BeanCtkPluginManagerViewPrivate *priv = GET_PRIV (view);

  g_clear_pointer (&priv->popup_menu, ctk_widget_destroy);

  g_clear_object (&priv->engine);
  g_clear_object (&priv->store);

  G_OBJECT_CLASS (bean_ctk_plugin_manager_view_parent_class)->dispose (object);
}

static void
bean_ctk_plugin_manager_view_class_init (BeanCtkPluginManagerViewClass *klass)
{
  GType the_type = G_TYPE_FROM_CLASS (klass);
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  CtkWidgetClass *widget_class = CTK_WIDGET_CLASS (klass);
  CtkTreeViewClass *tree_view_class = CTK_TREE_VIEW_CLASS (klass);

  object_class->set_property = bean_ctk_plugin_manager_view_set_property;
  object_class->get_property = bean_ctk_plugin_manager_view_get_property;
  object_class->constructed = bean_ctk_plugin_manager_view_constructed;
  object_class->dispose = bean_ctk_plugin_manager_view_dispose;

  widget_class->button_press_event = bean_ctk_plugin_manager_view_button_press_event;
  widget_class->popup_menu = bean_ctk_plugin_manager_view_popup_menu;
  widget_class->query_tooltip = bean_ctk_plugin_manager_view_query_tooltip;

  tree_view_class->row_activated = bean_ctk_plugin_manager_view_row_activated;

  /**
   * BeanCtkPLuginManagerView:engine:
   *
   * The #BeanEngine this view is attached to.
   */
  properties[PROP_ENGINE] =
    g_param_spec_object ("engine",
                         "engine",
                         "The BeanEngine this view is attached to",
                         BEAN_TYPE_ENGINE,
                         G_PARAM_READWRITE |
                         G_PARAM_CONSTRUCT_ONLY |
                         G_PARAM_STATIC_STRINGS);

  /**
   * BeanCtkPluginManagerView:show-builtin:
   *
   * If builtin plugins should be shown.
   *
   * Deprecated: 1.2: Use hidden plugins instead.
   */
  properties[PROP_SHOW_BUILTIN] =
    g_param_spec_boolean ("show-builtin",
                          "show-builtin",
                          "If builtin plugins should be shown",
                          FALSE,
                          G_PARAM_DEPRECATED |
                          G_PARAM_READWRITE |
                          G_PARAM_STATIC_STRINGS);

  /**
   * BeanCtkPluginManagerView::populate-popup:
   * @view: A #BeanCtkPluginManagerView.
   * @menu: A #CtkMenu.
   *
   * The ::populate-popup signal is emitted before showing the context
   * menu of the view. If you need to add items to the context menu,
   * connect to this signal and add your menuitems to the @menu.
   */
  signals[POPULATE_POPUP] =
    g_signal_new (I_("populate-popup"),
                  the_type,
                  G_SIGNAL_RUN_LAST,
                  G_STRUCT_OFFSET (BeanCtkPluginManagerViewClass, populate_popup),
                  NULL, NULL,
                  g_cclosure_marshal_VOID__OBJECT,
                  G_TYPE_NONE,
                  1,
                  CTK_TYPE_MENU);

  g_object_class_install_properties (object_class, N_PROPERTIES, properties);
}

/**
 * bean_ctk_plugin_manager_view_new:
 * @engine: (allow-none): A #BeanEngine, or %NULL.
 *
 * Creates a new plugin manager view for the given #BeanEngine.
 *
 * If @engine is %NULL, then the default engine will be used.
 *
 * Returns: the new #BeanCtkPluginManagerView.
 */
CtkWidget *
bean_ctk_plugin_manager_view_new (BeanEngine *engine)
{
  g_return_val_if_fail (engine == NULL || BEAN_IS_ENGINE (engine), NULL);

  return CTK_WIDGET (g_object_new (BEAN_CTK_TYPE_PLUGIN_MANAGER_VIEW,
                                   "engine", engine,
                                   NULL));
}

/**
 * bean_ctk_plugin_manager_view_set_show_builtin:
 * @view: A #BeanCtkPluginManagerView.
 * @show_builtin: If builtin plugins should be shown.
 *
 * Sets if builtin plugins should be shown.
 *
 * Deprecated: 1.2: Use hidden plugins instead.
 */
void
bean_ctk_plugin_manager_view_set_show_builtin (BeanCtkPluginManagerView *view,
                                               gboolean                  show_builtin)
{
  BeanCtkPluginManagerViewPrivate *priv = GET_PRIV (view);
  CtkTreeSelection *selection;
  CtkTreeIter iter;
  gboolean iter_set;

  g_return_if_fail (BEAN_CTK_IS_PLUGIN_MANAGER_VIEW (view));

  show_builtin = (show_builtin != FALSE);

  if (priv->show_builtin == show_builtin)
    return;

  selection = ctk_tree_view_get_selection (CTK_TREE_VIEW (view));

  /* We must get the selected iter before setting if builtin
     plugins should be shown so the proper model is set */
  iter_set = ctk_tree_selection_get_selected (selection, NULL, &iter);

  if (iter_set)
    convert_iter_to_child_iter (view, &iter);

  priv->show_builtin = show_builtin;

  if (show_builtin)
    {
      ctk_tree_view_set_model (CTK_TREE_VIEW (view),
                               CTK_TREE_MODEL (priv->store));
    }
  else
    {
      CtkTreeModel *model;

      model = ctk_tree_model_filter_new (CTK_TREE_MODEL (priv->store), NULL);
      ctk_tree_model_filter_set_visible_func (CTK_TREE_MODEL_FILTER (model),
                                              (CtkTreeModelFilterVisibleFunc) filter_builtins_visible,
                                              view,
                                              NULL);

      ctk_tree_view_set_model (CTK_TREE_VIEW (view), model);

      g_object_unref (model);
    }

  if (iter_set && convert_child_iter_to_iter (view, &iter))
    ctk_tree_selection_select_iter (selection, &iter);

  g_object_notify_by_pspec (G_OBJECT (view),
                            properties[PROP_SHOW_BUILTIN]);
}

/**
 * bean_ctk_plugin_manager_view_get_show_builtin:
 * @view: A #BeanCtkPluginManagerView.
 *
 * Returns if builtin plugins should be shown.
 *
 * Returns: if builtin plugins should be shown.
 *
 * Deprecated: 1.2: Use hidden plugins instead.
 */
gboolean
bean_ctk_plugin_manager_view_get_show_builtin (BeanCtkPluginManagerView *view)
{
  BeanCtkPluginManagerViewPrivate *priv = GET_PRIV (view);

  g_return_val_if_fail (BEAN_CTK_IS_PLUGIN_MANAGER_VIEW (view), FALSE);

  return priv->show_builtin;
}

/**
 * bean_ctk_plugin_manager_view_set_selected_plugin:
 * @view: A #BeanCtkPluginManagerView.
 * @info: A #BeanPluginInfo.
 *
 * Selects the given plugin.
 */
void
bean_ctk_plugin_manager_view_set_selected_plugin (BeanCtkPluginManagerView *view,
                                                  BeanPluginInfo           *info)
{
  BeanCtkPluginManagerViewPrivate *priv = GET_PRIV (view);
  CtkTreeIter iter;
  CtkTreeSelection *selection;

  g_return_if_fail (BEAN_CTK_IS_PLUGIN_MANAGER_VIEW (view));
  g_return_if_fail (info != NULL);

  g_return_if_fail (bean_ctk_plugin_manager_store_get_iter_from_plugin (priv->store,
                                                                        &iter, info));

  if (!convert_child_iter_to_iter (view, &iter))
    return;

  selection = ctk_tree_view_get_selection (CTK_TREE_VIEW (view));
  ctk_tree_selection_select_iter (selection, &iter);
}

/**
 * bean_ctk_plugin_manager_view_get_selected_plugin:
 * @view: A #BeanCtkPluginManagerView.
 *
 * Returns the currently selected plugin, or %NULL if a plugin is not selected.
 *
 * Returns: (transfer none): the selected plugin.
 */
BeanPluginInfo *
bean_ctk_plugin_manager_view_get_selected_plugin (BeanCtkPluginManagerView *view)
{
  BeanCtkPluginManagerViewPrivate *priv = GET_PRIV (view);
  CtkTreeSelection *selection;
  CtkTreeIter iter;
  BeanPluginInfo *info = NULL;

  g_return_val_if_fail (BEAN_CTK_IS_PLUGIN_MANAGER_VIEW (view), NULL);

  selection = ctk_tree_view_get_selection (CTK_TREE_VIEW (view));

  /* Since ctk+ 3.4 ctk_tree_view_get_selection() can in practice return NULL
   * here because 'cursor-changed' is emitted during 'destroy' (it wasn't
   * the case previously and is not properly documented as of today).
   */
  if (selection != NULL && ctk_tree_selection_get_selected (selection, NULL, &iter))
    {
      convert_iter_to_child_iter (view, &iter);
      info = bean_ctk_plugin_manager_store_get_plugin (priv->store, &iter);
    }

  return info;
}
