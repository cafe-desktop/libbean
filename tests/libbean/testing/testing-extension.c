/*
 * testing-extension.c
 * This file is part of libbean
 *
 * Copyright (C) 2011-2014 - Garrett Regier
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
#include <stdio.h>
#include <string.h>

#include <glib.h>
#include <libbean/bean.h>

#include "testing.h"
#include "testing-extension.h"

#include "introspection-abstract.h"
#include "introspection-base.h"
#include "introspection-callable.h"
#include "introspection-has-prerequisite.h"
#include "introspection-unimplemented.h"

typedef struct _TestFixture TestFixture;

struct _TestFixture {
  BeanEngine *engine;
  BeanPluginInfo *info;
};

static gchar *loader = NULL;
static gchar *extension_plugin = NULL;

static void
test_setup (TestFixture   *fixture,
	    gconstpointer  data G_GNUC_UNUSED)
{
  fixture->engine = testing_engine_new ();
  bean_engine_enable_loader (fixture->engine, loader);
  fixture->info = bean_engine_get_plugin_info (fixture->engine,
                                               extension_plugin);
}

static void
test_teardown (TestFixture   *fixture,
	       gconstpointer  data G_GNUC_UNUSED)
{
  testing_engine_free (fixture->engine);
}

static void
test_runner  (TestFixture   *fixture,
              gconstpointer  data)
{
  g_assert (fixture->info != NULL);
  g_assert (bean_engine_load_plugin (fixture->engine, fixture->info));

  ((void (*) (BeanEngine *, BeanPluginInfo *)) data) (fixture->engine,
                                                      fixture->info);
}

static void
test_extension_garbage_collect (BeanEngine     *engine,
                                BeanPluginInfo *info)
{
  gchar **loaded_plugins;

  bean_engine_garbage_collect (engine);

  /* Check that we can collect the garbage when no plugins are loaded */
  g_assert (bean_engine_unload_plugin (engine, info));

  loaded_plugins = bean_engine_get_loaded_plugins (engine);
  g_assert_cmpstr (loaded_plugins[0], ==, NULL);
  g_strfreev (loaded_plugins);

  bean_engine_garbage_collect (engine);
}

static void
test_extension_provides_valid (BeanEngine     *engine,
                               BeanPluginInfo *info)
{
  g_assert (bean_engine_provides_extension (engine, info,
                                            INTROSPECTION_TYPE_CALLABLE));
}

static void
test_extension_provides_invalid (BeanEngine     *engine,
                                 BeanPluginInfo *info)
{

  testing_util_push_log_hook ("*assertion*G_TYPE_IS_INTERFACE*failed");

  /* Invalid GType */
  bean_engine_provides_extension (engine, info, G_TYPE_INVALID);


  /* GObject but not a GInterface */
  bean_engine_provides_extension (engine, info, BEAN_TYPE_ENGINE);


  /* Does not implement this GType */
  g_assert (!bean_engine_provides_extension (engine, info,
                                             INTROSPECTION_TYPE_UNIMPLEMENTED));

  /* Not loaded */
  g_assert (bean_engine_unload_plugin (engine, info));
  g_assert (!bean_engine_provides_extension (engine, info,
                                             INTROSPECTION_TYPE_CALLABLE));
}

static void
test_extension_create_valid (BeanEngine     *engine,
                             BeanPluginInfo *info)
{
  BeanExtension *extension;

  extension = bean_engine_create_extension (engine, info,
                                            INTROSPECTION_TYPE_CALLABLE,
                                            NULL);

  g_assert (BEAN_IS_EXTENSION (extension));
  g_assert (INTROSPECTION_IS_CALLABLE (extension));

  g_object_unref (extension);
}

static void
test_extension_create_valid_without_properties (BeanEngine     *engine,
                                                BeanPluginInfo *info)
{
  BeanExtension *extension;

  extension =
    bean_engine_create_extension_with_properties (engine, info,
                                                  INTROSPECTION_TYPE_CALLABLE,
                                                  0, NULL, NULL);

  g_assert (BEAN_IS_EXTENSION (extension));
  g_assert (INTROSPECTION_IS_CALLABLE (extension));

  g_object_unref (extension);
}

static void
test_extension_create_valid_with_properties (BeanEngine     *engine,
                                             BeanPluginInfo *info)
{
  BeanExtension *extension;
  IntrospectionAbstract *abstract;
  GValue prop_values[1] = { G_VALUE_INIT };
  const gchar *prop_names[1] = { "abstract-property" };

  g_assert (bean_engine_load_plugin (engine, info));

  g_value_init (&prop_values[0], G_TYPE_INT);
  g_value_set_int (&prop_values[0], 47);
  extension =
      bean_engine_create_extension_with_properties (engine, info,
                                                    INTROSPECTION_TYPE_ABSTRACT,
                                                    G_N_ELEMENTS (prop_values),
                                                    prop_names,
                                                    prop_values);
  abstract = INTROSPECTION_ABSTRACT (extension);
  g_assert_cmpint (introspection_abstract_get_value (abstract), ==, 47);

  g_object_unref (extension);
  g_value_unset (&prop_values[0]);
}

static void
test_extension_create_invalid (BeanEngine     *engine,
                               BeanPluginInfo *info)
{
  BeanExtension *extension;

  testing_util_push_log_hook ("*assertion*G_TYPE_IS_INTERFACE*failed");
  testing_util_push_log_hook ("*does not provide a 'IntrospectionUnimplemented' extension");
  testing_util_push_log_hook ("*type 'IntrospectionCallable' has no property named 'does-not-exist'");
  testing_util_push_log_hook ("*assertion*bean_plugin_info_is_loaded*failed");

  /* Invalid GType */
  extension = bean_engine_create_extension (engine, info, G_TYPE_INVALID, NULL);
  g_assert (!BEAN_IS_EXTENSION (extension));


  /* GObject but not a GInterface */
  extension = bean_engine_create_extension (engine, info, BEAN_TYPE_ENGINE, NULL);
  g_assert (!BEAN_IS_EXTENSION (extension));


  /* Does not implement this GType */
  extension = bean_engine_create_extension (engine, info,
                                            INTROSPECTION_TYPE_UNIMPLEMENTED,
                                            NULL);
  g_assert (!BEAN_IS_EXTENSION (extension));

  /* Interface does not have a specified property */
  extension = bean_engine_create_extension (engine, info,
                                            INTROSPECTION_TYPE_CALLABLE,
                                            "does-not-exist", "",
                                            NULL);
  g_assert (!BEAN_IS_EXTENSION (extension));

  /* Not loaded */
  g_assert (bean_engine_unload_plugin (engine, info));
  extension = bean_engine_create_extension (engine, info,
                                            INTROSPECTION_TYPE_CALLABLE,
                                            NULL);
  g_assert (!BEAN_IS_EXTENSION (extension));
}

static void
test_extension_create_invalid_with_properties (BeanEngine     *engine,
                                               BeanPluginInfo *info)
{
  BeanExtension *extension;
  GValue prop_values[1] = { G_VALUE_INIT };
  const gchar *prop_names[1] = { NULL };
  GValue prop_values2[1] = { G_VALUE_INIT };
  const gchar *prop_names2[1] = { "does-not-exist" };

  g_value_init (&prop_values[0], G_TYPE_STRING);
  g_value_set_string (&prop_values[0], "foo");

  testing_util_push_log_hook ("*assertion*G_TYPE_IS_INTERFACE*failed");
  testing_util_push_log_hook ("*does not provide a 'IntrospectionUnimplemented' extension");
  testing_util_push_log_hook ("*property name*should not be NULL.");
  testing_util_push_log_hook ("*property value*should*initialized GValue.");
  testing_util_push_log_hook ("*assertion*bean_plugin_info_is_loaded*failed");

  /* Invalid GType */
  extension = bean_engine_create_extension_with_properties (engine, info,
                                                            G_TYPE_INVALID, 0,
                                                            NULL, NULL);
  g_assert (!BEAN_IS_EXTENSION (extension));

  /* GObject but not a GInterface */
  extension = bean_engine_create_extension_with_properties (engine, info,
                                                            BEAN_TYPE_ENGINE, 0,
                                                            NULL, NULL);
  g_assert (!BEAN_IS_EXTENSION (extension));

  /* Does not implement this GType */
  extension =
    bean_engine_create_extension_with_properties (engine, info,
                                                  INTROSPECTION_TYPE_UNIMPLEMENTED,
                                                  0, NULL, NULL);
  g_assert (!BEAN_IS_EXTENSION (extension));

  /* Interface has a NULL property name*/
  extension =
    bean_engine_create_extension_with_properties (engine, info,
                                                  INTROSPECTION_TYPE_CALLABLE,
                                                  G_N_ELEMENTS (prop_names2),
                                                  prop_names, prop_values);
  g_assert (!BEAN_IS_EXTENSION (extension));

  /* Interface has a not initialiazed GValue */
  extension =
    bean_engine_create_extension_with_properties (engine, info,
                                                  INTROSPECTION_TYPE_CALLABLE,
                                                  G_N_ELEMENTS (prop_names2),
                                                  prop_names2, prop_values2);
  g_assert (!BEAN_IS_EXTENSION (extension));

  /* Not loaded */
  g_assert (bean_engine_unload_plugin (engine, info));
  extension =
    bean_engine_create_extension_with_properties (engine, info,
                                                  INTROSPECTION_TYPE_CALLABLE,
                                                  0, NULL, NULL);
  g_assert (!BEAN_IS_EXTENSION (extension));

  g_value_unset (&prop_values[0]);

}

static void
test_extension_create_with_prerequisite (BeanEngine     *engine,
                                         BeanPluginInfo *info)
{
  BeanExtension *extension;
  gint prerequisite_property = -1;

  extension = bean_engine_create_extension (engine, info,
                                            INTROSPECTION_TYPE_HAS_PREREQUISITE,
                                            "prerequisite-property", 47,
                                            NULL);

  g_assert (INTROSPECTION_IS_HAS_PREREQUISITE (extension));
  g_assert (INTROSPECTION_IS_CALLABLE (extension));

  g_object_get (extension,
                "prerequisite-property", &prerequisite_property,
                NULL);
  g_assert_cmpint (prerequisite_property, ==, 47);

  g_object_unref (extension);
}

static void
test_extension_reload (BeanEngine     *engine,
                       BeanPluginInfo *info)
{
  gint i;

  for (i = 0; i < 3; ++i)
    {
      g_assert (bean_engine_load_plugin (engine, info));
      g_assert (bean_engine_unload_plugin (engine, info));
    }
}

static void
test_extension_plugin_info (BeanEngine     *engine,
                            BeanPluginInfo *info)
{
  BeanExtension *extension;
  IntrospectionBase *base;

  g_assert (bean_engine_load_plugin (engine, info));

  extension = bean_engine_create_extension (engine, info,
                                            INTROSPECTION_TYPE_BASE,
                                            NULL);

  base = INTROSPECTION_BASE (extension);

  g_assert (introspection_base_get_plugin_info (base) == info);

  g_object_unref (extension);
}

static void
test_extension_get_settings (BeanEngine     *engine,
                             BeanPluginInfo *info)
{
  BeanExtension *extension;
  IntrospectionBase *base;
  GSettings *settings;

  g_assert (bean_engine_load_plugin (engine, info));

  extension = bean_engine_create_extension (engine, info,
                                            INTROSPECTION_TYPE_BASE,
                                            NULL);

  base = INTROSPECTION_BASE (extension);

  settings = introspection_base_get_settings (base);
  g_assert (G_IS_SETTINGS (settings));

  g_object_unref (settings);
  g_object_unref (extension);
}

static void
test_extension_abstract (BeanEngine     *engine,
                         BeanPluginInfo *info)
{
  BeanExtension *extension;
  IntrospectionAbstract *abstract;

  g_assert (bean_engine_load_plugin (engine, info));

  extension = bean_engine_create_extension (engine, info,
                                            INTROSPECTION_TYPE_ABSTRACT,
                                            "abstract-property", 47,
                                            NULL);

  abstract = INTROSPECTION_ABSTRACT (extension);

  g_assert_cmpint (introspection_abstract_get_value (abstract), ==, 47);
  introspection_abstract_set_value (abstract, -22);
  g_assert_cmpint (introspection_abstract_get_value (abstract), ==, -22);

  g_object_unref (extension);
}

static gint
run_in_multiple_threads (GFunc    func,
                         gpointer user_data)
{
  gint i, n_threads;
  GThreadPool *pool;
  GError *error = NULL;

  /* Avoid too many threads, but try to get some good contention */
  n_threads = g_get_num_processors () + 2;

  pool = g_thread_pool_new (func, user_data, n_threads, TRUE, &error);
  g_assert_no_error (error);

  for (i = 0; i < n_threads; ++i)
    {
      /* Cannot supply NULL as the data... */
      g_thread_pool_push (pool, GUINT_TO_POINTER (i + 1), &error);
      g_assert_no_error (error);
    }

  g_thread_pool_free (pool, FALSE, TRUE);

  return n_threads;
}

static void
multiple_threads_loaders_in_thread (guint    nth_thread G_GNUC_UNUSED,
                                    gboolean use_nonglobal_loaders)
{
  gint i, j;
  BeanEngine *engine;
  BeanPluginInfo *info;
  GObject *extension;

  engine = testing_engine_new_full (use_nonglobal_loaders);
  bean_engine_enable_loader (engine, loader);

  info = bean_engine_get_plugin_info (engine, extension_plugin);
  g_assert (info != NULL);

  for (i = 0; i < 10; ++i)
    {
      g_assert (bean_engine_load_plugin (engine, info));

      for (j = 0; j < 50; ++j)
        {
          extension = bean_engine_create_extension (engine, info,
                                                    INTROSPECTION_TYPE_BASE,
                                                    NULL);

          g_assert (extension != NULL);
          g_object_unref (extension);
        }

      g_assert (bean_engine_unload_plugin (engine, info));
    }

  testing_engine_free (engine);
}

static void
test_extension_multiple_threads_global_loaders (BeanEngine     *engine G_GNUC_UNUSED,
						BeanPluginInfo *info G_GNUC_UNUSED)
{
  run_in_multiple_threads ((GFunc) multiple_threads_loaders_in_thread,
                           GINT_TO_POINTER (FALSE));
}

static void
test_extension_multiple_threads_nonglobal_loaders (BeanEngine     *engine G_GNUC_UNUSED,
						   BeanPluginInfo *info G_GNUC_UNUSED)
{
  run_in_multiple_threads ((GFunc) multiple_threads_loaders_in_thread,
                           GINT_TO_POINTER (TRUE));
}

static void
multiple_threads_callbacks_in_thread (guint            nth_thread G_GNUC_UNUSED,
                                      BeanActivatable *activatable)
{
  gint i;

  for (i = 0; i < 100; ++i)
    bean_activatable_update_state (activatable);
}

static void
test_extension_multiple_threads_callbacks (BeanEngine     *engine,
                                           BeanPluginInfo *info)
{
  BeanExtension *extension;
  gint n_threads, update_count;

  extension = bean_engine_create_extension (engine, info,
                                            BEAN_TYPE_ACTIVATABLE,
                                            NULL);

  n_threads = run_in_multiple_threads ((GFunc) multiple_threads_callbacks_in_thread,
                                       extension);

  g_object_get (extension, "update-count", &update_count, NULL);
  g_assert_cmpint (update_count, ==, n_threads * 100);

  g_object_unref (extension);
}

static void
test_extension_call_no_args (BeanEngine     *engine,
                             BeanPluginInfo *info)
{
  BeanExtension *extension;
  IntrospectionCallable *callable;

  extension = bean_engine_create_extension (engine, info,
                                            INTROSPECTION_TYPE_CALLABLE,
                                            NULL);

  callable = INTROSPECTION_CALLABLE (extension);

  g_assert (bean_extension_call (extension, "call_no_args"));
  introspection_callable_call_no_args (callable);

  g_object_unref (extension);
}

static void
test_extension_call_with_return (BeanEngine     *engine,
                                 BeanPluginInfo *info)
{
  BeanExtension *extension;
  IntrospectionCallable *callable;
  gchar *return_val = NULL;

  extension = bean_engine_create_extension (engine, info,
                                            INTROSPECTION_TYPE_CALLABLE,
                                            NULL);

  callable = INTROSPECTION_CALLABLE (extension);

  g_assert (bean_extension_call (extension, "call_with_return", &return_val));
  g_assert_cmpstr (return_val, ==, "Hello, World!");

  g_free (return_val);
  return_val = NULL;

  return_val = introspection_callable_call_with_return (callable);
  g_assert_cmpstr (return_val, ==, "Hello, World!");

  g_free (return_val);
  g_object_unref (extension);
}

static void
test_extension_call_single_arg (BeanEngine     *engine,
                                BeanPluginInfo *info)
{
  BeanExtension *extension;
  IntrospectionCallable *callable;
  gboolean called = FALSE;

  extension = bean_engine_create_extension (engine, info,
                                            INTROSPECTION_TYPE_CALLABLE,
                                            NULL);

  callable = INTROSPECTION_CALLABLE (extension);

  g_assert (bean_extension_call (extension, "call_single_arg", &called));
  g_assert (called);

  called = FALSE;

  introspection_callable_call_single_arg (callable, &called);
  g_assert (called);

  g_object_unref (extension);
}

static void
test_extension_call_multi_args (BeanEngine     *engine,
                                BeanPluginInfo *info)
{
  BeanExtension *extension;
  IntrospectionCallable *callable;
  gint in, out, inout;
  gint inout_saved;

  extension = bean_engine_create_extension (engine, info,
                                            INTROSPECTION_TYPE_CALLABLE,
                                            NULL);

  callable = INTROSPECTION_CALLABLE (extension);

  in = g_random_int ();
  inout = g_random_int ();
  inout_saved = inout;

  g_assert (bean_extension_call (extension, "call_multi_args",
                                 in, &out, &inout));

  g_assert_cmpint (inout_saved, ==, out);
  g_assert_cmpint (in, ==, inout);

  in = g_random_int ();
  inout = g_random_int ();
  inout_saved = inout;

  introspection_callable_call_multi_args (callable, in, &out, &inout);

  g_assert_cmpint (inout_saved, ==, out);
  g_assert_cmpint (in, ==, inout);

  g_object_unref (extension);
}

static void
test_extension_call_abstract (BeanEngine     *engine,
                              BeanPluginInfo *info)
{
  BeanExtension *extension;
  gint value = 0;

  g_assert (bean_engine_load_plugin (engine, info));

  extension = bean_engine_create_extension (engine, info,
                                            INTROSPECTION_TYPE_ABSTRACT,
                                            "abstract-property", 47,
                                            NULL);

  g_assert (bean_extension_call (extension, "get_value", &value));
  g_assert_cmpint (value, ==, 47);

  g_assert (bean_extension_call (extension, "set_value", -22));
  g_assert (bean_extension_call (extension, "get_value", &value));
  g_assert_cmpint (value, ==, -22);

  g_object_unref (extension);
}

#define _EXTENSION_TEST(loader, path, ftest) \
  G_STMT_START { \
    gchar *full_path = g_strdup_printf (EXTENSION_TEST_NAME (%s, "%s"), \
                                        loader, path); \
\
    g_test_add (full_path, TestFixture, \
                (gpointer) test_extension_##ftest, \
                test_setup, test_runner, test_teardown); \
\
    g_free (full_path); \
  } G_STMT_END

void
testing_extension_basic (const gchar *loader_)
{
  gint i, j;
  gchar *loader_name;
  BeanEngine *engine;

  loader = g_strdup (loader_);

  loader_name = g_new0 (gchar, strlen (loader) + 1);
  for (i = 0, j = 0; loader[i] != '\0'; ++i)
    {
      if (loader[i] != '.')
        loader_name[j++] = loader[i];
    }

  extension_plugin = g_strdup_printf ("extension-%s", loader_name);
  g_free (loader_name);

  engine = testing_engine_new ();
  bean_engine_enable_loader (engine, loader);

  /* Check that the loaders are created lazily */
  g_assert (g_type_from_name ("BeanPluginLoader") == G_TYPE_INVALID);

  testing_engine_free (engine);


  _EXTENSION_TEST (loader, "garbage-collect", garbage_collect);

  _EXTENSION_TEST (loader, "provides-valid", provides_valid);
  _EXTENSION_TEST (loader, "provides-invalid", provides_invalid);

  _EXTENSION_TEST (loader, "create-valid", create_valid);
  _EXTENSION_TEST (loader, "create-valid-without-properties",
                   create_valid_without_properties);
  _EXTENSION_TEST (loader, "create-valid-with-properties",
                   create_valid_with_properties);
  _EXTENSION_TEST (loader, "create-invalid", create_invalid);
  _EXTENSION_TEST (loader, "create-invalid-with-properties",
                   create_invalid_with_properties);
  _EXTENSION_TEST (loader, "create-with-prerequisite", create_with_prerequisite);

  _EXTENSION_TEST (loader, "reload", reload);

  _EXTENSION_TEST (loader, "plugin-info", plugin_info);
  _EXTENSION_TEST (loader, "get-settings", get_settings);

  _EXTENSION_TEST (loader, "abstract", abstract);

  _EXTENSION_TEST (loader, "multiple-threads/global-loaders",
                   multiple_threads_global_loaders);
  _EXTENSION_TEST (loader, "multiple-threads/nonglobal-loaders",
                   multiple_threads_nonglobal_loaders);

  /* Not needed for C plugins as they are independent of libbean */
  if (g_strcmp0 (loader, "c") != 0)
    {
      _EXTENSION_TEST (loader, "multiple-threads/callbacks",
                       multiple_threads_callbacks);
    }
}

void
testing_extension_callable (const gchar *loader)
{
  _EXTENSION_TEST (loader, "call-no-args", call_no_args);
  _EXTENSION_TEST (loader, "call-with-return", call_with_return);
  _EXTENSION_TEST (loader, "call-single-arg", call_single_arg);
  _EXTENSION_TEST (loader, "call-multi-args", call_multi_args);
  _EXTENSION_TEST (loader, "call-abstract", call_abstract);
}

void
testing_extension_add (const gchar   *path,
                       GTestDataFunc  func)
{
  g_test_add (path, TestFixture, (gpointer) func,
              test_setup, test_runner, test_teardown);
}

int
testing_extension_run_tests (void)
{
  int retval;

  retval = testing_run_tests ();

  g_free (extension_plugin);
  g_free (loader);

  return retval;
}
