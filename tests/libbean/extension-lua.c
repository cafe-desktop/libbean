/*
 * extension-lua.c
 * This file is part of libbean
 *
 * Copyright (C) 2014 - Garrett Regier
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

#include <lua.h>
#include <lauxlib.h>

#include <libbean/bean-activatable.h>
#include "libbean/bean-engine-priv.h"

#include "testing/testing-extension.h"
#include "introspection/introspection-base.h"


/* We must stop and start the garbage collector
 * when testing reference counts otherwise issues
 * will occur if the garbage collector runs preemptively.
 */
static void
set_garbage_collector_state (BeanEngine     *engine,
                             BeanPluginInfo *info,
                             gboolean        start)
{
  BeanExtension *extension;

  extension = bean_engine_create_extension (engine, info,
                                            BEAN_TYPE_ACTIVATABLE,
                                            NULL);

  if (start)
    {
      /* collectgarbage('restart') */
      bean_activatable_activate (BEAN_ACTIVATABLE (extension));
    }
  else
    {
      /* collectgarbage('stop') */
      bean_activatable_deactivate (BEAN_ACTIVATABLE (extension));
    }

  g_object_unref (extension);
}

static void
test_extension_lua_instance_refcount (BeanEngine     *engine,
                                      BeanPluginInfo *info)
{
  BeanExtension *extension;

  set_garbage_collector_state (engine, info, FALSE);

  extension = bean_engine_create_extension (engine, info,
                                            BEAN_TYPE_ACTIVATABLE,
                                            NULL);
  g_object_add_weak_pointer (extension, (gpointer *) &extension);

  g_assert (BEAN_IS_EXTENSION (extension));

  /* The Lua wrapper created around the extension
   * object should have increased its refcount by 1.
   */
  g_assert_cmpint (extension->ref_count, ==, 2);

  /* The Lua wrapper around the extension has been garbage collected */
  bean_engine_garbage_collect (engine);
  g_assert_cmpint (G_OBJECT (extension)->ref_count, ==, 1);

  /* Create a new Lua wrapper around the extension */
  bean_activatable_update_state (BEAN_ACTIVATABLE (extension));
  g_assert_cmpint (G_OBJECT (extension)->ref_count, ==, 2);

  /* The Lua wrapper still exists */
  g_object_unref (extension);
  g_assert_cmpint (extension->ref_count, ==, 1);

  /* The Lua wrapper around the extension has been garbage collected */
  bean_engine_garbage_collect (engine);
  g_assert (extension == NULL);

  set_garbage_collector_state (engine, info, TRUE);
}

static void
test_extension_lua_activatable_subject_refcount (BeanEngine     *engine,
                                                 BeanPluginInfo *info)
{
  BeanExtension *extension;
  GObject *object;

  set_garbage_collector_state (engine, info, FALSE);

  /* Create the 'object' property value, to be similar to a GtkWindow
   * instance: a sunk GInitiallyUnowned object.
   */
  object = g_object_new (G_TYPE_INITIALLY_UNOWNED, NULL);
  g_object_add_weak_pointer (object, (gpointer *) &object);
  g_object_ref_sink (object);
  g_assert_cmpint (object->ref_count, ==, 1);

  /* We pre-create the wrapper to make it easier to check reference count */
  extension = bean_engine_create_extension (engine, info,
                                            BEAN_TYPE_ACTIVATABLE,
                                            "object", object,
                                            NULL);
  g_object_add_weak_pointer (extension, (gpointer *) &extension);

  g_assert (BEAN_IS_EXTENSION (extension));

  /* The Lua wrapper created around our dummy
   * object should have increased its refcount by 1.
   */
  bean_engine_garbage_collect (engine);
  g_assert_cmpint (object->ref_count, ==, 2);

  g_object_unref (extension);
  g_assert (extension == NULL);

  /* We unreffed the extension, so it should have been
   * destroyed and our dummy object's refcount should be back to 1.
   */
  bean_engine_garbage_collect (engine);
  g_assert_cmpint (object->ref_count, ==, 1);

  g_object_unref (object);
  g_assert (object == NULL);

  set_garbage_collector_state (engine, info, TRUE);
}

static void
test_extension_lua_nonexistent (BeanEngine *engine)
{
  BeanPluginInfo *info;

  testing_util_push_log_hook ("Error loading plugin "
                              "'extension-lua51-nonexistent'*");

  info = bean_engine_get_plugin_info (engine, "extension-lua51-nonexistent");

  g_assert (!bean_engine_load_plugin (engine, info));
}

int
main (int   argc,
      char *argv[])
{
  testing_init (&argc, &argv);

  /* Only test the basics */
  testing_extension_basic ("lua5.1");

  /* We still need to add the callable tests
   * because of bean_extension_call()
   */
  testing_extension_callable ("lua5.1");

#undef EXTENSION_TEST
#undef EXTENSION_TEST_FUNC
  
#define EXTENSION_TEST(loader, path, func) \
  testing_extension_add (EXTENSION_TEST_NAME (loader, path), \
                         (gpointer) test_extension_lua_##func)

#define EXTENSION_TEST_FUNC(loader, path, func) \
  g_test_add_func (EXTENSION_TEST_NAME (loader, path), \
                   (gpointer) test_extension_lua_##func)

  EXTENSION_TEST (lua5.1, "instance-refcount", instance_refcount);
  EXTENSION_TEST (lua5.1, "activatable-subject-refcount",
                  activatable_subject_refcount);
  EXTENSION_TEST (lua5.1, "nonexistent", nonexistent);

  return testing_extension_run_tests ();
}
