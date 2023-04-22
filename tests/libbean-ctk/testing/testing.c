/*
 * testing.c
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

#include <stdlib.h>

#include <glib.h>
#include <girepository.h>
#include <testing-util.h>

#include "testing.h"

void
testing_init (gint    *argc,
              gchar ***argv)
{
  GError *error = NULL;
  static gboolean initialized = FALSE;

  if (initialized)
    return;

  /* Must set environment variables first */
  testing_util_envars ();

  ctk_test_init (argc, argv, NULL);

  /* Must be called after ctk_test_init()
   * because it changed the log settings
   */
  testing_util_init ();

  g_irepository_require_private (g_irepository_get_default (),
                                 BUILDDIR "/libbean-ctk",
                                 "BeanCtk", "1.0", 0, &error);
  g_assert_no_error (error);

  initialized = TRUE;
}

BeanEngine *
testing_engine_new (void)
{
  BeanEngine *engine;

  engine = testing_util_engine_new ();
  bean_engine_add_search_path (engine, BUILDDIR "/tests/libbean-ctk/plugins",
                                       SRCDIR   "/tests/libbean-ctk/plugins");

  return engine;
}

BeanPluginInfo *
testing_get_plugin_info_for_iter (BeanCtkPluginManagerView *view,
                                  CtkTreeIter              *iter)
{
  CtkTreeSelection *selection;
  CtkTreeIter selected_iter;
  gboolean had_selection;
  BeanPluginInfo *info;

  /* This is annoying but the only way to get the plugin info
   * is to ask the view for the info of the selected plugin
   */

  selection = ctk_tree_view_get_selection (CTK_TREE_VIEW (view));

  had_selection = ctk_tree_selection_get_selected (selection,
                                                   NULL, &selected_iter);

  ctk_tree_selection_select_iter (selection, iter);

  info = bean_ctk_plugin_manager_view_get_selected_plugin (view);

  if (had_selection)
    ctk_tree_selection_select_iter (selection, &selected_iter);

  return info;
}

gboolean
testing_get_iter_for_plugin_info (BeanCtkPluginManagerView *view,
                                  BeanPluginInfo           *info,
                                  CtkTreeIter              *iter)
{
  CtkTreeModel *model;
  CtkTreeIter pos_iter;

  model = ctk_tree_view_get_model (CTK_TREE_VIEW (view));

  g_assert (ctk_tree_model_get_iter_first (model, &pos_iter));

  do
    {
      if (testing_get_plugin_info_for_iter (view, &pos_iter) == info)
        {
          if (iter != NULL)
            *iter = pos_iter;

          return TRUE;
        }
    }
  while (ctk_tree_model_iter_next (model, &pos_iter));

  return FALSE;
}

static gboolean
delete_event_cb (CtkWidget *window,
                 GdkEvent  *event,
                 CtkWidget *widget)
{
  ctk_main_quit ();

  return TRUE;
}

/* This takes a gpointer for ease of debugging */
void
testing_show_widget (gpointer widget)
{
  CtkWidget *window;
  GLogFunc orig_log_handler;

  g_assert (CTK_IS_WIDGET (widget));

  widget = ctk_widget_get_toplevel (CTK_WIDGET (widget));

  if (ctk_widget_is_toplevel (CTK_WIDGET (widget)))
    window = widget;
  else
    {
      window = ctk_window_new (CTK_WINDOW_TOPLEVEL);
      ctk_container_add (CTK_CONTAINER (window), CTK_WIDGET (widget));
    }

  ctk_window_set_default_size (CTK_WINDOW (window), 200, 100);

  ctk_widget_show_all (window);

  g_signal_connect (window,
                    "delete-event",
                    G_CALLBACK (delete_event_cb),
                    widget);

  /* Do not abort if a warning occurs while running the widget */
  orig_log_handler = g_log_set_default_handler (g_log_default_handler, NULL);

  ctk_main ();

  g_log_set_default_handler (orig_log_handler, NULL);
}
