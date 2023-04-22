/*
 * bean-plugin-manager.c
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
#include <girepository.h>

#ifdef OS_OSX
#include "bean-utils-osx.h"
#endif

#include <libbean/bean-engine.h>
#include <libbean/bean-plugin-info.h>
#include <libbean/bean-i18n-priv.h>

#include "bean-ctk-plugin-manager.h"
#include "bean-ctk-plugin-manager-view.h"
#include "bean-ctk-configurable.h"

/**
 * SECTION:bean-ctk-plugin-manager
 * @short_description: Management GUI for plugins.
 *
 * The #BeanGtkPluginManager is a widget that can be used to manage plugins,
 * i.e. load or unload them, and see some pieces of information.
 *
 * <inlinegraphic fileref="bean-ctk-plugin-manager.png" format="PNG" />
 *
 * The only thing you need to do as an application writer if you wish to use
 * the manager to configure your plugins is to instantiate it using
 * bean_ctk_plugin_manager_new() and pack it into another widget or a window
 * (as in the screenshot above).
 *
 **/

struct _BeanGtkPluginManagerPrivate {
  BeanEngine *engine;

  GtkWidget *sw;
  GtkWidget *view;

  GtkWidget *about;

  GtkWidget *about_button;
  GtkWidget *configure_button;
};

/* Properties */
enum {
  PROP_0,
  PROP_ENGINE,
  PROP_VIEW,
  N_PROPERTIES
};

static GParamSpec *properties[N_PROPERTIES] = { NULL };

G_DEFINE_TYPE_WITH_PRIVATE (BeanGtkPluginManager,
                            bean_ctk_plugin_manager,
                            GTK_TYPE_BOX)

static GtkWindow *
get_toplevel (GtkWidget *widget)
{
  GtkWindow *toplevel;

  toplevel = (GtkWindow *) ctk_widget_get_toplevel (widget);
  if (!GTK_IS_WINDOW (toplevel))
    return NULL;

  /* Make sure the window always has a window group */
  if (!ctk_window_has_group (toplevel))
    {
      GtkWindowGroup *window_group;

      window_group = ctk_window_group_new ();
      ctk_window_group_add_window (window_group, toplevel);
      g_object_unref (window_group);
    }

  return toplevel;
}

static gboolean
plugin_is_configurable (BeanGtkPluginManager *pm,
                        BeanPluginInfo       *info)
{
  BeanGtkPluginManagerPrivate *priv = bean_ctk_plugin_manager_get_instance_private (pm);

  if (info == NULL || !bean_plugin_info_is_loaded (info))
    return FALSE;

  return bean_engine_provides_extension (priv->engine, info,
                                         BEAN_GTK_TYPE_CONFIGURABLE);
}

static void
update_button_sensitivity (BeanGtkPluginManager *pm,
                           BeanPluginInfo       *info)
{
  BeanGtkPluginManagerPrivate *priv = bean_ctk_plugin_manager_get_instance_private (pm);

  ctk_widget_set_sensitive (priv->about_button, info != NULL);
  ctk_widget_set_sensitive (priv->configure_button,
                            plugin_is_configurable (pm, info));
}

static void
show_about_cb (GtkWidget            *widget,
               BeanGtkPluginManager *pm)
{
  BeanGtkPluginManagerPrivate *priv = bean_ctk_plugin_manager_get_instance_private (pm);
  BeanGtkPluginManagerView *view;
  BeanPluginInfo *info;
  GtkWindow *toplevel;
  gboolean modal;

  view = BEAN_GTK_PLUGIN_MANAGER_VIEW (priv->view);

  info = bean_ctk_plugin_manager_view_get_selected_plugin (view);
  g_return_if_fail (info != NULL);

  toplevel = get_toplevel (GTK_WIDGET (pm));
  modal = toplevel == NULL ? FALSE : ctk_window_get_modal (toplevel);

  /* If there is another about dialog already open destroy it */
  g_clear_pointer (&priv->about, ctk_widget_destroy);

  priv->about = GTK_WIDGET (g_object_new (GTK_TYPE_ABOUT_DIALOG,
                                          "program-name", bean_plugin_info_get_name (info),
                                          "copyright", bean_plugin_info_get_copyright (info),
                                          "authors", bean_plugin_info_get_authors (info),
                                          "comments", bean_plugin_info_get_description (info),
                                          "website", bean_plugin_info_get_website (info),
                                          "logo-icon-name", bean_plugin_info_get_icon_name (info),
                                          "version", bean_plugin_info_get_version (info),
                                          "destroy-with-parent", TRUE,
                                          "transient-for", toplevel,
                                          "modal", modal,
                                          NULL));

  g_signal_connect (priv->about,
                    "response",
                    G_CALLBACK (ctk_widget_destroy),
                    NULL);
  g_signal_connect (priv->about,
                    "destroy",
                    G_CALLBACK (ctk_widget_destroyed),
                    &priv->about);

  ctk_widget_show (priv->about);
}

static void
help_button_cb (GtkWidget      *button,
                BeanPluginInfo *info)
{
  const gchar *help_uri;
#ifndef OS_OSX
  GError *error = NULL;
  GtkWidget *error_dlg;
#endif

  g_return_if_fail (bean_plugin_info_get_help_uri (info) != NULL);

  help_uri = bean_plugin_info_get_help_uri (info);

#ifdef OS_OSX
  bean_open_url_osx (help_uri);
#else

  ctk_show_uri_on_window (get_toplevel (button), help_uri, GDK_CURRENT_TIME, &error);
  if (error == NULL)
    return;

  g_debug ("Failed to show help URI: '%s'", help_uri);

  error_dlg = ctk_message_dialog_new (get_toplevel (button),
                                      GTK_DIALOG_MODAL |
                                      GTK_DIALOG_DESTROY_WITH_PARENT,
                                      GTK_MESSAGE_ERROR,
                                      GTK_BUTTONS_CLOSE,
                                      _("There was an error displaying the help."));

  ctk_message_dialog_format_secondary_text (GTK_MESSAGE_DIALOG (error_dlg),
                                            "%s", error->message);

  g_signal_connect (error_dlg,
                    "response",
                    G_CALLBACK (ctk_widget_destroy),
                    NULL);

  ctk_widget_show_all (error_dlg);

  g_error_free (error);
#endif
}

static void
show_configure_cb (GtkWidget            *widget,
                   BeanGtkPluginManager *pm)
{
  BeanGtkPluginManagerPrivate *priv = bean_ctk_plugin_manager_get_instance_private (pm);
  BeanGtkPluginManagerView *view;
  BeanPluginInfo *info;
  BeanExtension *exten;
  GtkWidget *conf_widget = NULL;
  GtkWidget *conf_dlg;
  GtkWidget *vbox;

  view = BEAN_GTK_PLUGIN_MANAGER_VIEW (priv->view);

  info = bean_ctk_plugin_manager_view_get_selected_plugin (view);
  g_return_if_fail (info != NULL);

  exten = bean_engine_create_extension (priv->engine, info, BEAN_GTK_TYPE_CONFIGURABLE, NULL);
  g_return_if_fail (BEAN_IS_EXTENSION (exten));

  conf_widget = bean_ctk_configurable_create_configure_widget (BEAN_GTK_CONFIGURABLE (exten));
  g_object_unref (exten);

  g_return_if_fail (GTK_IS_WIDGET (conf_widget));
  g_return_if_fail (!ctk_widget_is_toplevel (conf_widget));

  conf_dlg = ctk_dialog_new_with_buttons (bean_plugin_info_get_name (info),
                                          get_toplevel (GTK_WIDGET (pm)),
                                          GTK_DIALOG_MODAL |
                                          GTK_DIALOG_DESTROY_WITH_PARENT,
                                          _("_Close"),
                                          GTK_RESPONSE_CLOSE,
                                          NULL);

  vbox = ctk_dialog_get_content_area (GTK_DIALOG (conf_dlg));
  ctk_box_pack_start (GTK_BOX (vbox), conf_widget, TRUE, TRUE, 0);

  if (bean_plugin_info_get_help_uri (info) != NULL)
    {
      GtkWidget *help_button;

      help_button = ctk_dialog_add_button (GTK_DIALOG (conf_dlg),
                                           _("_Help"), GTK_RESPONSE_HELP);

      g_signal_connect (help_button,
                        "clicked",
                        G_CALLBACK (help_button_cb),
                        info);
    }

  ctk_widget_show_all (conf_dlg);

  g_signal_connect (conf_dlg,
                    "response",
                    G_CALLBACK (ctk_widget_destroy),
                    NULL);
}

static void
plugin_loaded_toggled_cb (BeanEngine           *engine,
                          BeanPluginInfo       *info,
                          BeanGtkPluginManager *pm)
{
  BeanGtkPluginManagerPrivate *priv = bean_ctk_plugin_manager_get_instance_private (pm);
  BeanGtkPluginManagerView *view;
  BeanPluginInfo *selected;

  view = BEAN_GTK_PLUGIN_MANAGER_VIEW (priv->view);
  selected = bean_ctk_plugin_manager_view_get_selected_plugin (view);

  if (selected == info)
    update_button_sensitivity (pm, info);
}

static void
selection_changed_cb (BeanGtkPluginManager *pm)
{
  BeanGtkPluginManagerPrivate *priv = bean_ctk_plugin_manager_get_instance_private (pm);
  BeanGtkPluginManagerView *view;
  BeanPluginInfo *info;

  view = BEAN_GTK_PLUGIN_MANAGER_VIEW (priv->view);
  info = bean_ctk_plugin_manager_view_get_selected_plugin (view);

  update_button_sensitivity (pm, info);
}

static void
populate_popup_cb (BeanGtkPluginManagerView *view,
                   GtkMenu                  *menu,
                   BeanGtkPluginManager     *pm)
{
  BeanPluginInfo *info;
  GtkWidget *item;

  info = bean_ctk_plugin_manager_view_get_selected_plugin (view);

  if (info == NULL)
    return;

  item = ctk_check_menu_item_new_with_mnemonic (_("Pr_eferences"));
  g_signal_connect (item, "activate", G_CALLBACK (show_configure_cb), pm);
  ctk_widget_set_sensitive (item, plugin_is_configurable (pm, info));
  ctk_menu_shell_prepend (GTK_MENU_SHELL (menu), item);

  item = ctk_check_menu_item_new_with_mnemonic (_("_About"));
  g_signal_connect (item, "activate", G_CALLBACK (show_about_cb), pm);
  ctk_menu_shell_prepend (GTK_MENU_SHELL (menu), item);
}

static void
bean_ctk_plugin_manager_init (BeanGtkPluginManager *pm)
{
  BeanGtkPluginManagerPrivate *priv = bean_ctk_plugin_manager_get_instance_private (pm);
  GtkWidget *toolbar;
  GtkStyleContext *context;
  GtkToolItem *toolitem;
  GtkWidget *toolbar_box;
  GtkWidget *item_box;

  /* If we are using a BeanGtkPluginManager, we know for sure we will be using
     libbean-ctk, so let's load the typelib for it here. */
  g_irepository_require (g_irepository_get_default (),
                         "BeanGtk", "1.0", 0, NULL);

  ctk_orientable_set_orientation (GTK_ORIENTABLE (pm),
                                  GTK_ORIENTATION_VERTICAL);

  priv->sw = ctk_scrolled_window_new (NULL, NULL);
  ctk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (priv->sw),
                                  GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  ctk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (priv->sw),
                                       GTK_SHADOW_IN);
  context = ctk_widget_get_style_context (priv->sw);
  ctk_style_context_set_junction_sides (context, GTK_JUNCTION_BOTTOM);
  ctk_box_pack_start (GTK_BOX (pm), priv->sw, TRUE, TRUE, 0);

  toolbar = ctk_toolbar_new ();
  ctk_toolbar_set_icon_size (GTK_TOOLBAR (toolbar), GTK_ICON_SIZE_MENU);
  context = ctk_widget_get_style_context (toolbar);
  ctk_style_context_set_junction_sides (context, GTK_JUNCTION_TOP);
  ctk_style_context_add_class (context, GTK_STYLE_CLASS_INLINE_TOOLBAR);
  ctk_box_pack_start (GTK_BOX (pm), toolbar, FALSE, FALSE, 0);

  toolitem = ctk_tool_item_new ();
  ctk_tool_item_set_expand (toolitem, TRUE);
  ctk_toolbar_insert (GTK_TOOLBAR (toolbar), toolitem, -1);

  /* this box is needed to get the items at the end of the toolbar */
  toolbar_box = ctk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);
  ctk_container_add (GTK_CONTAINER (toolitem), toolbar_box);

  /* we need another box to disable css grouping */
  item_box = ctk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  ctk_box_pack_end (GTK_BOX (toolbar_box), item_box, FALSE, FALSE, 0);

  priv->about_button = ctk_button_new_with_mnemonic (_("_About"));
  ctk_box_pack_start (GTK_BOX (item_box), priv->about_button,
                      FALSE, FALSE, 0);

  /* we need another box to disable css grouping */
  item_box = ctk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  ctk_box_pack_end (GTK_BOX (toolbar_box), item_box, FALSE, FALSE, 0);

  priv->configure_button = ctk_button_new_with_mnemonic (_("_Preferences"));
  ctk_box_pack_start (GTK_BOX (item_box), priv->configure_button,
                      FALSE, FALSE, 0);

  /* setup a window of a sane size. */
  ctk_widget_set_size_request (GTK_WIDGET (priv->sw), 270, 100);

  g_signal_connect (priv->about_button,
                    "clicked",
                    G_CALLBACK (show_about_cb),
                    pm);
  g_signal_connect (priv->configure_button,
                    "clicked",
                    G_CALLBACK (show_configure_cb),
                    pm);
}

static void
bean_ctk_plugin_manager_set_property (GObject      *object,
                                      guint         prop_id,
                                      const GValue *value,
                                      GParamSpec   *pspec)
{
  BeanGtkPluginManager *pm = BEAN_GTK_PLUGIN_MANAGER (object);
  BeanGtkPluginManagerPrivate *priv = bean_ctk_plugin_manager_get_instance_private (pm);

  switch (prop_id)
    {
    case PROP_ENGINE:
      priv->engine = g_value_get_object (value);
      break;
    case PROP_VIEW:
      priv->view = g_value_get_object (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
bean_ctk_plugin_manager_get_property (GObject    *object,
                                      guint       prop_id,
                                      GValue     *value,
                                      GParamSpec *pspec)
{
  BeanGtkPluginManager *pm = BEAN_GTK_PLUGIN_MANAGER (object);
  BeanGtkPluginManagerPrivate *priv = bean_ctk_plugin_manager_get_instance_private (pm);

  switch (prop_id)
    {
    case PROP_ENGINE:
      g_value_set_object (value, priv->engine);
      break;
    case PROP_VIEW:
      g_value_set_object (value, bean_ctk_plugin_manager_get_view (pm));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
bean_ctk_plugin_manager_constructed (GObject *object)
{
  BeanGtkPluginManager *pm = BEAN_GTK_PLUGIN_MANAGER (object);
  BeanGtkPluginManagerPrivate *priv = bean_ctk_plugin_manager_get_instance_private (pm);
  GtkTreeSelection *selection;

  if (priv->engine == NULL)
    priv->engine = bean_engine_get_default ();

  g_object_ref (priv->engine);

  /* When we create the manager, we always rescan the plugins directory
   * Must come after the view has connected to notify::plugin-list
   */
  bean_engine_rescan_plugins (priv->engine);

  /* For the view to behave as expected, we must ensure it uses the same
   * engine as the manager itself.
   */
  if (priv->view != NULL)
    {
      BeanEngine *engine;

      g_object_get (priv->view,
                    "engine", &engine,
                    NULL);

      g_warn_if_fail (engine == priv->engine);

      if (engine != priv->engine)
        g_clear_object (&priv->view);

      g_object_unref (engine);
    }

  if (priv->view == NULL)
    priv->view = bean_ctk_plugin_manager_view_new (priv->engine);

  ctk_container_add (GTK_CONTAINER (priv->sw), priv->view);

  selection = ctk_tree_view_get_selection (GTK_TREE_VIEW (priv->view));

  g_signal_connect_object (selection,
                           "changed",
                           G_CALLBACK (selection_changed_cb),
                           pm,
                           G_CONNECT_SWAPPED);
  g_signal_connect_object (priv->view,
                           "cursor-changed",
                           G_CALLBACK (selection_changed_cb),
                           pm,
                           G_CONNECT_SWAPPED);
  g_signal_connect_object (priv->view,
                           "populate-popup",
                           G_CALLBACK (populate_popup_cb),
                           pm,
                           0);
  g_signal_connect_object (priv->engine,
                           "load-plugin",
                           G_CALLBACK (plugin_loaded_toggled_cb),
                           pm,
                           G_CONNECT_AFTER);
  g_signal_connect_object (priv->engine,
                           "unload-plugin",
                           G_CALLBACK (plugin_loaded_toggled_cb),
                           pm,
                           G_CONNECT_AFTER);

  /* Update the button sensitivity */
  selection_changed_cb (pm);

  G_OBJECT_CLASS (bean_ctk_plugin_manager_parent_class)->constructed (object);
}

static void
bean_ctk_plugin_manager_dispose (GObject *object)
{
  BeanGtkPluginManager *pm = BEAN_GTK_PLUGIN_MANAGER (object);
  BeanGtkPluginManagerPrivate *priv = bean_ctk_plugin_manager_get_instance_private (pm);

  g_clear_object (&priv->engine);
  g_clear_pointer (&priv->about, ctk_widget_destroy);

  G_OBJECT_CLASS (bean_ctk_plugin_manager_parent_class)->dispose (object);
}

static void
bean_ctk_plugin_manager_class_init (BeanGtkPluginManagerClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = bean_ctk_plugin_manager_set_property;
  object_class->get_property = bean_ctk_plugin_manager_get_property;
  object_class->constructed = bean_ctk_plugin_manager_constructed;
  object_class->dispose = bean_ctk_plugin_manager_dispose;

  /**
   * BeanGtkPluginManager:engine:
   *
   * The #BeanEngine this manager is attached to.
   */
  properties[PROP_ENGINE] =
    g_param_spec_object ("engine",
                         "engine",
                         "The BeanEngine this manager is attached to",
                         BEAN_TYPE_ENGINE,
                         G_PARAM_READWRITE |
                         G_PARAM_CONSTRUCT_ONLY |
                         G_PARAM_STATIC_STRINGS);

  /**
   * BeanGtkPluginManager:view:
   *
   * The #BeanGtkPluginManagerView shown in the #BeanGtkPluginManager.
   */
  properties[PROP_VIEW] =
    g_param_spec_object ("view",
                         "view",
                         "The view shown in the manager",
                         BEAN_GTK_TYPE_PLUGIN_MANAGER_VIEW,
                         G_PARAM_READWRITE |
                         G_PARAM_CONSTRUCT_ONLY |
                         G_PARAM_STATIC_STRINGS);

  g_object_class_install_properties (object_class, N_PROPERTIES, properties);
}

/**
 * bean_ctk_plugin_manager_new:
 * @engine: (allow-none): A #BeanEngine, or %NULL.
 *
 * Creates a new plugin manager for the given #BeanEngine.
 *
 * If @engine is %NULL, then the default engine will be used.
 *
 * Returns: the new #BeanGtkPluginManager.
 */
GtkWidget *
bean_ctk_plugin_manager_new (BeanEngine *engine)
{
  g_return_val_if_fail (engine == NULL || BEAN_IS_ENGINE (engine), NULL);

  return GTK_WIDGET (g_object_new (BEAN_GTK_TYPE_PLUGIN_MANAGER,
                                   "engine", engine,
                                   NULL));
}

/**
 * bean_ctk_plugin_manager_get_view:
 * @pm: A #BeanGtkPluginManager.
 *
 * Returns the #BeanGtkPluginManagerView of @pm.
 *
 * Returns: (transfer none): the #GtkTreeView of @pm.
 */
GtkWidget *
bean_ctk_plugin_manager_get_view (BeanGtkPluginManager *pm)
{
  BeanGtkPluginManagerPrivate *priv = bean_ctk_plugin_manager_get_instance_private (pm);

  g_return_val_if_fail (BEAN_GTK_IS_PLUGIN_MANAGER (pm), NULL);

  return priv->view;
}
