/*
 * plugin-manager.c
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

#include "config.h"

#include <ctk/ctk.h>

#include <libbean/bean.h>
#include <libbean/bean-i18n-priv.h>
#include <libbean-ctk/bean-ctk.h>

#include "testing/testing.h"

typedef struct _TestFixture TestFixture;

struct _TestFixture {
  BeanEngine *engine;
  CtkWidget *window;
  BeanCtkPluginManager *manager;
  BeanCtkPluginManagerView *view;
  CtkTreeSelection *selection;
  CtkTreeModel *model;
  CtkWidget *about_button;
  CtkWidget *configure_button;
};

static void
notify_model_cb (CtkTreeView *view,
                 GParamSpec  *pspec,
                 TestFixture *fixture)
{
  fixture->model = ctk_tree_view_get_model (GTK_TREE_VIEW (fixture->view));
}

static void
test_setup (TestFixture   *fixture,
            gconstpointer  data)
{
  fixture->engine = testing_engine_new ();
  fixture->window = ctk_window_new (GTK_WINDOW_TOPLEVEL);
  fixture->manager = BEAN_GTK_PLUGIN_MANAGER (bean_ctk_plugin_manager_new (NULL));
  fixture->view = BEAN_GTK_PLUGIN_MANAGER_VIEW (bean_ctk_plugin_manager_get_view (fixture->manager));
  fixture->selection = ctk_tree_view_get_selection (GTK_TREE_VIEW (fixture->view));

  ctk_container_add (GTK_CONTAINER (fixture->window),
                     GTK_WIDGET (fixture->manager));

  g_signal_connect (fixture->view,
                    "notify::model",
                    G_CALLBACK (notify_model_cb),
                    fixture);

  /* Set the model */
  g_object_notify (G_OBJECT (fixture->view), "model");

  fixture->about_button = ctk_test_find_widget (fixture->window,
                                                _("About"),
                                                GTK_TYPE_BUTTON);
  g_assert (GTK_IS_BUTTON (fixture->about_button));

  fixture->configure_button = ctk_test_find_widget (fixture->window,
                                                    _("Preferences"),
                                                    GTK_TYPE_BUTTON);
  g_assert (GTK_IS_BUTTON (fixture->configure_button));
}

static void
test_teardown (TestFixture   *fixture,
               gconstpointer  data)
{
  ctk_widget_destroy (GTK_WIDGET (fixture->window));

  testing_engine_free (fixture->engine);
}

static void
test_runner (TestFixture   *fixture,
             gconstpointer  data)
{
  ((void (*) (TestFixture *)) data) (fixture);
}

static CtkWidget *
find_window_by_title (CtkWindow   *window,
                      const gchar *title)
{
  CtkWindowGroup *group;
  GList *windows;
  GList *l;
  CtkWidget *found_window = NULL;

  group = ctk_window_get_group (window);
  windows = ctk_window_group_list_windows (group);

  for (l = windows; l != NULL; l = l->next)
    {
      if (g_strcmp0 (ctk_window_get_title (l->data), title) == 0)
        {
          found_window = l->data;
          break;
        }
    }

  g_list_free (windows);

  g_assert (GTK_IS_WINDOW (found_window));

  return found_window;
}

static void
test_ctk_plugin_manager_about_button_sensitivity (TestFixture *fixture)
{
  CtkTreeIter iter;
  BeanPluginInfo *info;

  testing_util_push_log_hook ("Could not find plugin 'does-not-exist'*");

  /* Must come first otherwise the first iter may
   * be after a revealed builtin plugin
   */
  bean_ctk_plugin_manager_view_set_show_builtin (fixture->view, TRUE);

  /* Causes the plugin to become unavailable */
  info = bean_engine_get_plugin_info (fixture->engine, "unavailable");
  bean_engine_load_plugin (fixture->engine, info);

  g_assert (ctk_tree_model_get_iter_first (fixture->model, &iter));

  do
    {
      ctk_tree_selection_select_iter (fixture->selection, &iter);

      g_assert (ctk_widget_get_sensitive (fixture->about_button));
    }
  while (ctk_tree_model_iter_next (fixture->model, &iter));
}

static void
test_ctk_plugin_manager_configure_button_sensitivity (TestFixture *fixture)
{
  CtkTreeIter iter;
  BeanPluginInfo *info;

  testing_util_push_log_hook ("Could not find plugin 'does-not-exist'*");

  /* Must come first otherwise the first iter may
   * be after a revealed builtin plugin
   */
  bean_ctk_plugin_manager_view_set_show_builtin (fixture->view, TRUE);

  /* So we can configure them */
  info = bean_engine_get_plugin_info (fixture->engine, "builtin-configurable");
  bean_engine_load_plugin (fixture->engine, info);
  info = bean_engine_get_plugin_info (fixture->engine, "configurable");
  bean_engine_load_plugin (fixture->engine, info);

  /* Causes the plugin to become unavailable */
  info = bean_engine_get_plugin_info (fixture->engine, "unavailable");
  bean_engine_load_plugin (fixture->engine, info);

  g_assert (ctk_tree_model_get_iter_first (fixture->model, &iter));

  do
    {
      gboolean sensitive;

      ctk_tree_selection_select_iter (fixture->selection, &iter);

      info = testing_get_plugin_info_for_iter (fixture->view, &iter);

      if (!bean_plugin_info_is_loaded (info))
        {
          sensitive = FALSE;
        }
      else
        {
          sensitive = bean_engine_provides_extension (fixture->engine, info,
                                                      BEAN_GTK_TYPE_CONFIGURABLE);
        }

      g_assert_cmpint (ctk_widget_get_sensitive (fixture->configure_button),
                       ==, sensitive);
    }
  while (ctk_tree_model_iter_next (fixture->model, &iter));
}

static void
test_ctk_plugin_manager_plugin_loaded (TestFixture *fixture)
{
  CtkTreeIter iter;
  BeanPluginInfo *info;

  info = bean_engine_get_plugin_info (fixture->engine, "configurable");
  testing_get_iter_for_plugin_info (fixture->view, info, &iter);

  ctk_tree_selection_select_iter (fixture->selection, &iter);

  g_assert (!ctk_widget_is_sensitive (fixture->configure_button));
  bean_engine_load_plugin (fixture->engine, info);
  g_assert (ctk_widget_is_sensitive (fixture->configure_button));
}

static void
test_ctk_plugin_manager_plugin_unloaded (TestFixture *fixture)
{
  CtkTreeIter iter;
  BeanPluginInfo *info;

  test_ctk_plugin_manager_plugin_loaded (fixture);

  info = bean_engine_get_plugin_info (fixture->engine, "configurable");
  testing_get_iter_for_plugin_info (fixture->view, info, &iter);

  g_assert (ctk_widget_is_sensitive (fixture->configure_button));
  bean_engine_unload_plugin (fixture->engine, info);
  g_assert (!ctk_widget_is_sensitive (fixture->configure_button));
}

static void
test_ctk_plugin_manager_about_dialog (TestFixture *fixture)
{
  gint i;
  CtkTreeIter iter;
  CtkWidget *window;
  BeanPluginInfo *info;
  const gchar **authors_plugin;
  const gchar * const *authors_dialog;

  /* Full Info is a builtin plugin */
  bean_ctk_plugin_manager_view_set_show_builtin (fixture->view, TRUE);

  info = bean_engine_get_plugin_info (fixture->engine, "full-info");

  testing_get_iter_for_plugin_info (fixture->view, info, &iter);
  ctk_tree_selection_select_iter (fixture->selection, &iter);

  /* Must be first so we the window is added to the window group */
  ctk_button_clicked (GTK_BUTTON (fixture->about_button));

  window = find_window_by_title (GTK_WINDOW (fixture->window),
                                 "About Full Info");
  g_assert (GTK_IS_ABOUT_DIALOG (window));


  g_assert_cmpstr (ctk_about_dialog_get_program_name (GTK_ABOUT_DIALOG (window)),
                   ==, bean_plugin_info_get_name (info));
  g_assert_cmpstr (ctk_about_dialog_get_copyright (GTK_ABOUT_DIALOG (window)),
                   ==, bean_plugin_info_get_copyright (info));
  g_assert_cmpstr (ctk_about_dialog_get_website (GTK_ABOUT_DIALOG (window)),
                   ==, bean_plugin_info_get_website (info));
  g_assert_cmpstr (ctk_about_dialog_get_logo_icon_name (GTK_ABOUT_DIALOG (window)),
                   ==, bean_plugin_info_get_icon_name (info));
  g_assert_cmpstr (ctk_about_dialog_get_version (GTK_ABOUT_DIALOG (window)),
                   ==, bean_plugin_info_get_version (info));
  g_assert_cmpstr (ctk_about_dialog_get_comments (GTK_ABOUT_DIALOG (window)),
                   ==, bean_plugin_info_get_description (info));

  authors_plugin = bean_plugin_info_get_authors (info);
  authors_dialog = ctk_about_dialog_get_authors (GTK_ABOUT_DIALOG (window));

  for (i = 0; authors_plugin[i] == NULL && authors_dialog[i] == NULL; ++i)
    g_assert_cmpstr (authors_plugin[i], ==, authors_dialog[i]);

  ctk_widget_destroy (window);
}

static void
test_ctk_plugin_manager_configure_dialog (TestFixture *fixture)
{
  CtkTreeIter iter;
  CtkWidget *window;
  BeanPluginInfo *info;
  GList *list;
  GList *list_it;
  CtkWidget *content;
  CtkWidget *label = NULL;
  CtkWidget *close_button = NULL;
  CtkWidget *help_button = NULL;

  info = bean_engine_get_plugin_info (fixture->engine, "configurable");

  bean_engine_load_plugin (fixture->engine, info);

  testing_get_iter_for_plugin_info (fixture->view, info, &iter);
  ctk_tree_selection_select_iter (fixture->selection, &iter);

  /* Must be first so the window is added to the window group */
  ctk_button_clicked (GTK_BUTTON (fixture->configure_button));

  window = find_window_by_title (GTK_WINDOW (fixture->window), "Configurable");
  g_assert (GTK_IS_DIALOG (window));

  content = ctk_dialog_get_content_area (GTK_DIALOG (window));
  list = ctk_container_get_children (GTK_CONTAINER (content));

  for (list_it = list; list_it != NULL; list_it = list_it->next)
    {
      if (GTK_IS_LABEL (list_it->data))
        {
          const gchar *text = ctk_label_get_text (GTK_LABEL (list_it->data));

          if (g_strcmp0 (text, "Hello, World!") == 0)
            label = GTK_WIDGET (list_it->data);
        }
    }

  g_assert (label != NULL);

  g_list_free (list);


  close_button = ctk_dialog_get_widget_for_response (GTK_DIALOG (window),
                                                     GTK_RESPONSE_CLOSE);
  g_assert (close_button != NULL);

  help_button = ctk_dialog_get_widget_for_response (GTK_DIALOG (window),
                                                    GTK_RESPONSE_HELP);
  g_assert (help_button != NULL);

  ctk_widget_destroy (window);
}

static void
test_ctk_plugin_manager_ctkbuilder (void)
{
  CtkBuilder *builder;
  GError *error = NULL;
  BeanCtkPluginManager *manager;
  BeanCtkPluginManagerView *view;
  static const gchar *ctkbuilder_string =
    "<?xml version='1.0' encoding='UTF-8'?>\n"
    "<interface>\n"
    "<object class='BeanCtkPluginManagerView' id='view'>\n"
    "  <property name='show-builtin'>True</property>\n"
    "</object>\n"
    "<object class='BeanCtkPluginManager' id='manager'>\n"
    "  <property name='view'>view</property>\n"
    "</object>\n"
    "</interface>";

  builder = ctk_builder_new ();

  ctk_builder_add_from_string (builder, ctkbuilder_string, -1, &error);
  g_assert_no_error (error);

  manager = BEAN_GTK_PLUGIN_MANAGER (ctk_builder_get_object (builder, "manager"));
  g_assert (BEAN_GTK_IS_PLUGIN_MANAGER (manager));

  view = BEAN_GTK_PLUGIN_MANAGER_VIEW (bean_ctk_plugin_manager_get_view (manager));

  g_assert (G_OBJECT (view) == ctk_builder_get_object (builder, "view"));

  g_assert (bean_ctk_plugin_manager_view_get_show_builtin (view));

  /* Freeing the builder will free the objects */
  g_object_unref (builder);
}

int
main (int    argc,
      char **argv)
{
  testing_init (&argc, &argv);

#define TEST(path, ftest) \
  g_test_add ("/ctk/plugin-manager/" path, TestFixture, \
              (gpointer) test_ctk_plugin_manager_##ftest, \
              test_setup, test_runner, test_teardown)

#define TEST_FUNC(path, ftest) \
  g_test_add_func ("/ctk/plugin-manager/" path, \
                   test_ctk_plugin_manager_##ftest)

  TEST ("about-button-sensitivity", about_button_sensitivity);
  TEST ("configure-button-sensitivity", configure_button_sensitivity);

  TEST ("plugin-loaded", plugin_loaded);
  TEST ("plugin-unloaded", plugin_unloaded);

  TEST ("about-dialog", about_dialog);
  TEST ("configure-dialog", configure_dialog);

  TEST_FUNC ("ctkbuilder", ctkbuilder);

#undef TEST

  return testing_run_tests ();
}
