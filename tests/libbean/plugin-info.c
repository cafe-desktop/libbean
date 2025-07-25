/*
 * plugin-info.c
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
#include <libbean/bean.h>

#include "testing/testing.h"

typedef struct _TestFixture TestFixture;

struct _TestFixture {
  BeanEngine *engine;
};

static void
test_setup (TestFixture   *fixture,
	    gconstpointer  data G_GNUC_UNUSED)
{
  fixture->engine = testing_engine_new ();
}

static void
test_teardown (TestFixture   *fixture,
	       gconstpointer  data G_GNUC_UNUSED)
{
  testing_engine_free (fixture->engine);
}

static void
test_runner (TestFixture   *fixture,
             gconstpointer  data)
{
  ((void (*) (BeanEngine *engine)) data) (fixture->engine);
}

static void
test_plugin_info_verify_full_info (BeanEngine *engine)
{
  BeanPluginInfo *info;
  GError *error = NULL;
  const gchar **authors;

  info = bean_engine_get_plugin_info (engine, "full-info");

  g_assert (!bean_plugin_info_is_loaded (info));
  g_assert (bean_plugin_info_is_available (info, &error));
  g_assert_no_error (error);
  g_assert (bean_plugin_info_is_builtin (info));

  g_assert_cmpstr (bean_plugin_info_get_module_name (info), ==, "full-info");
  g_assert (g_str_has_suffix (bean_plugin_info_get_module_dir (info), "/tests/plugins"));
  g_assert (g_str_has_suffix (bean_plugin_info_get_data_dir (info), "/tests/plugins/full-info"));

  g_assert_cmpstr (bean_plugin_info_get_dependencies (info)[0], ==, "something");
  g_assert_cmpstr (bean_plugin_info_get_dependencies (info)[1], ==, "something-else");
  g_assert_cmpstr (bean_plugin_info_get_dependencies (info)[2], ==, NULL);

  g_assert_cmpstr (bean_plugin_info_get_name (info), ==, "Full Info");
  g_assert_cmpstr (bean_plugin_info_get_description (info), ==, "Has full info.");
  g_assert_cmpstr (bean_plugin_info_get_icon_name (info), ==, "ctk-ok");
  g_assert_cmpstr (bean_plugin_info_get_website (info), ==, "https://wiki.gnome.org/Projects/Libbean");
  g_assert_cmpstr (bean_plugin_info_get_copyright (info), ==, "Copyright © 2010 Garrett Regier");
  g_assert_cmpstr (bean_plugin_info_get_version (info), ==, "1.0");
  g_assert_cmpstr (bean_plugin_info_get_help_uri (info), ==, "Help Me!");

  authors = bean_plugin_info_get_authors (info);
  g_assert (authors != NULL && authors[1] == NULL);
  g_assert_cmpstr (authors[0], ==, "Garrett Regier");

  g_assert_cmpstr (bean_plugin_info_get_external_data (info, "External"), ==, "external data");
  g_assert_cmpstr (bean_plugin_info_get_external_data (info, "X-External"), ==, "external data");
}

static void
test_plugin_info_verify_min_info (BeanEngine *engine)
{
  BeanPluginInfo *info;
  GError *error = NULL;
  const gchar **authors;

  info = bean_engine_get_plugin_info (engine, "min-info");

  g_assert (!bean_plugin_info_is_loaded (info));
  g_assert (bean_plugin_info_is_available (info, &error));
  g_assert_no_error (error);
  g_assert (!bean_plugin_info_is_builtin (info));

  g_assert_cmpstr (bean_plugin_info_get_module_name (info), ==, "min-info");
  g_assert (g_str_has_suffix (bean_plugin_info_get_module_dir (info), "/tests/plugins"));
  g_assert (g_str_has_suffix (bean_plugin_info_get_data_dir (info), "/tests/plugins/min-info"));

  g_assert_cmpstr (bean_plugin_info_get_dependencies (info)[0], ==, NULL);

  g_assert_cmpstr (bean_plugin_info_get_name (info), ==, "Min Info");
  g_assert_cmpstr (bean_plugin_info_get_description (info), ==, NULL);
  g_assert_cmpstr (bean_plugin_info_get_icon_name (info), ==, "libbean-plugin");
  g_assert_cmpstr (bean_plugin_info_get_website (info), ==, NULL);
  g_assert_cmpstr (bean_plugin_info_get_copyright (info), ==, NULL);
  g_assert_cmpstr (bean_plugin_info_get_version (info), ==, NULL);
  g_assert_cmpstr (bean_plugin_info_get_help_uri (info), ==, NULL);

  authors = bean_plugin_info_get_authors (info);
  g_assert (authors != NULL && authors[0] == NULL);
}

static void
test_plugin_info_has_dep (BeanEngine *engine)
{
  BeanPluginInfo *info;

  info = bean_engine_get_plugin_info (engine, "full-info");

  g_assert (bean_plugin_info_has_dependency (info, "something"));
  g_assert (bean_plugin_info_has_dependency (info, "something-else"));
  g_assert (!bean_plugin_info_has_dependency (info, "does-not-exist"));


  info = bean_engine_get_plugin_info (engine, "min-info");

  g_assert_cmpstr (bean_plugin_info_get_dependencies (info)[0], ==, NULL);
  g_assert (!bean_plugin_info_has_dependency (info, "does-not-exist"));
}

static void
test_plugin_info_missing_module (BeanEngine *engine)
{
  g_assert (bean_engine_get_plugin_info (engine, "invalid-info-module") == NULL);
}

static void
test_plugin_info_missing_name (BeanEngine *engine)
{
  g_assert (bean_engine_get_plugin_info (engine, "invalid-info-name") == NULL);
}

static void
test_plugin_info_os_dependant_help (BeanEngine *engine)
{
  BeanPluginInfo *info;
  const gchar *help;

  info = bean_engine_get_plugin_info (engine, "os-dependant-help");

  help = bean_plugin_info_get_help_uri (info);

#ifdef G_OS_WIN32
  g_assert_cmpstr (help, ==, "Windows");
#elif defined(OS_OSX)
  g_assert_cmpstr (help, ==, "MacOS-X");
#else
  g_assert_cmpstr (help, ==, "GNOME");
#endif
}

int
main (int    argc,
      char **argv)
{
  testing_init (&argc, &argv);

#define TEST(path, ftest) \
  g_test_add ("/plugin-info/" path, TestFixture, \
              (gpointer) test_plugin_info_##ftest, \
              test_setup, test_runner, test_teardown)

  TEST ("verify-full-info", verify_full_info);
  TEST ("verify-min-info", verify_min_info);

  TEST ("has-dep", has_dep);

  TEST ("missing-module", missing_module);
  TEST ("missing-name", missing_name);

  TEST ("os-dependant-help", os_dependant_help);

#undef TEST

  return testing_run_tests ();
}
