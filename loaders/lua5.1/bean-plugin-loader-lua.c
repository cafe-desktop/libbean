/*
 * bean-plugin-loader-lua.c
 * This file is part of libbean
 *
 * Copyright (C) 2014-2015 - Garrett Regier
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

#include "bean-plugin-loader-lua.h"
#include "libbean/bean-plugin-info-priv.h"

#include <string.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "bean-lua-internal.h"
#include "bean-lua-utils.h"


typedef void (* LgiLockFunc) (gpointer lgi_lock);


typedef struct {
  lua_State *L;

  gpointer lgi_lock;
  LgiLockFunc lgi_enter_func;
  LgiLockFunc lgi_leave_func;
} BeanPluginLoaderLuaPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (BeanPluginLoaderLua,
                            bean_plugin_loader_lua,
                            BEAN_TYPE_PLUGIN_LOADER)

#define GET_PRIV(o) \
  (bean_plugin_loader_lua_get_instance_private (o))

static GQuark quark_extension_type = 0;

G_MODULE_EXPORT void
bean_register_types (BeanObjectModule *module)
{
  bean_object_module_register_extension_type (module,
                                              BEAN_TYPE_PLUGIN_LOADER,
                                              BEAN_TYPE_PLUGIN_LOADER_LUA);
}

static lua_State *
thread_enter (BeanPluginLoaderLua *lua_loader,
              BeanPluginInfo      *info)
{
  BeanPluginLoaderLuaPrivate *priv = GET_PRIV (lua_loader);
  lua_State *L = priv->L;
  lua_State *NL = info->loader_data;

  priv->lgi_enter_func (priv->lgi_lock);

  if (NL != NULL)
    {
      /* We should never have multiple values on the stack */
      g_assert_cmpint (lua_gettop (NL), ==, 0);
    }
  else
    {
      luaL_checkstack (L, 2, "");

      lua_pushlightuserdata (L, info);
      NL = lua_newthread (L);
      lua_rawset (L, LUA_REGISTRYINDEX);

      info->loader_data = NL;
    }

  return NL;
}

static void
thread_leave (BeanPluginLoaderLua  *lua_loader,
              BeanPluginInfo       *info,
              lua_State           **L_ptr)
{
  BeanPluginLoaderLuaPrivate *priv = GET_PRIV (lua_loader);
  lua_State *L = info->loader_data;

  /* Prevent keeping the L as a usable variable */
  g_assert (*L_ptr == L);
  *L_ptr = NULL;

  /* The stack should always be empty */
  g_assert_cmpint (lua_gettop (L), ==, 0);

  priv->lgi_leave_func (priv->lgi_lock);
}

static GType
find_lua_extension_type (lua_State      *L,
                         BeanPluginInfo *info,
                         GType           exten_type)
{
  luaL_checkstack (L, 2, "");
  lua_pushstring (L, info->filename);
  lua_pushlightuserdata (L, GSIZE_TO_POINTER (exten_type));

  if (bean_lua_internal_call (L, "find_extension_type",
                              2, LUA_TLIGHTUSERDATA))
    {
      GType extension_type;

      extension_type = (GType) lua_touserdata (L, -1);
      lua_pop (L, 1);

      if (g_type_is_a (extension_type, exten_type))
        return extension_type;

      g_warning ("Found invalid extension type '%s' for '%s'",
                 g_type_name (extension_type), g_type_name (exten_type));
    }

  return G_TYPE_INVALID;
}

static gboolean
bean_plugin_loader_lua_provides_extension (BeanPluginLoader *loader,
                                           BeanPluginInfo   *info,
                                           GType             exten_type)
{
  BeanPluginLoaderLua *lua_loader = BEAN_PLUGIN_LOADER_LUA (loader);
  lua_State *L;
  GType the_type;

  L = thread_enter (lua_loader, info);

  the_type = find_lua_extension_type (L, info, exten_type);

  thread_leave (lua_loader, info, &L);
  return the_type != G_TYPE_INVALID;
}

static BeanExtension *
bean_plugin_loader_lua_create_extension (BeanPluginLoader *loader,
                                         BeanPluginInfo   *info,
                                         GType             exten_type,
                                         guint             n_parameters,
                                         GParameter       *parameters)
{
  BeanPluginLoaderLua *lua_loader = BEAN_PLUGIN_LOADER_LUA (loader);
  lua_State *L;
  GType the_type;
  GObject *object = NULL;

  L = thread_enter (lua_loader, info);

  the_type = find_lua_extension_type (L, info, exten_type);
  if (the_type == G_TYPE_INVALID)
    goto out;

  object = g_object_newv (the_type, n_parameters, parameters);
  if (object == NULL)
    goto out;

  /* We have to remember which interface we are instantiating
   * for the deprecated bean_extension_get_extension_type().
   */
  g_object_set_qdata (object, quark_extension_type,
                      GSIZE_TO_POINTER (exten_type));

  luaL_checkstack (L, 2, "");
  lua_pushlightuserdata (L, object);
  lua_pushlightuserdata (L, info);

  if (!bean_lua_internal_call (L, "setup_extension", 2, LUA_TNIL))
    g_clear_object (&object);

out:

  thread_leave (lua_loader, info, &L);
  return object;
}

static gboolean
bean_plugin_loader_lua_load (BeanPluginLoader *loader,
                             BeanPluginInfo   *info)
{
  BeanPluginLoaderLua *lua_loader = BEAN_PLUGIN_LOADER_LUA (loader);
  lua_State *L;
  gboolean success = FALSE;

  L = thread_enter (lua_loader, info);

  luaL_checkstack (L, 3, "");
  lua_pushstring (L, info->filename);
  lua_pushstring (L, bean_plugin_info_get_module_dir (info));
  lua_pushstring (L, bean_plugin_info_get_module_name (info));

  if (bean_lua_internal_call (L, "load", 3, LUA_TBOOLEAN))
    {
      success = lua_toboolean (L, -1);
      lua_pop (L, 1);
    }

  thread_leave (lua_loader, info, &L);
  return success;
}

static void
bean_plugin_loader_lua_unload (BeanPluginLoader *loader,
                               BeanPluginInfo   *info)
{
  BeanPluginLoaderLua *lua_loader = BEAN_PLUGIN_LOADER_LUA (loader);
  BeanPluginLoaderLuaPrivate *priv = GET_PRIV (lua_loader);
  lua_State *L = priv->L;

  priv->lgi_enter_func (priv->lgi_lock);

  /* The stack should always be empty */
  g_assert_cmpint (lua_gettop (info->loader_data), ==, 0);

  /* Delete the thread's reference */
  lua_pushlightuserdata (L, info);
  lua_pushnil (L);
  lua_rawset (L, LUA_REGISTRYINDEX);

  info->loader_data = NULL;

  priv->lgi_leave_func (priv->lgi_lock);
}

static void
bean_plugin_loader_lua_garbage_collect (BeanPluginLoader *loader)
{
  BeanPluginLoaderLua *lua_loader = BEAN_PLUGIN_LOADER_LUA (loader);
  BeanPluginLoaderLuaPrivate *priv = GET_PRIV (lua_loader);
  lua_State *L = priv->L;

  priv->lgi_enter_func (priv->lgi_lock);

  bean_lua_internal_call (L, "garbage_collect", 0, LUA_TNIL);

  priv->lgi_leave_func (priv->lgi_lock);
}

static int
atpanic_handler (lua_State *L)
{
  G_BREAKPOINT ();
  return 0;
}

static gboolean
bean_plugin_loader_lua_initialize (BeanPluginLoader *loader)
{
  BeanPluginLoaderLua *lua_loader = BEAN_PLUGIN_LOADER_LUA (loader);
  BeanPluginLoaderLuaPrivate *priv = GET_PRIV (lua_loader);
  lua_State *L;

  priv->L = L = luaL_newstate ();
  if (L == NULL)
    {
      g_critical ("Failed to allocate lua_State");
      return FALSE;
    }

  /* Set before any other code is run */
  if (g_getenv ("BEAN_LUA_DEBUG") != NULL)
    lua_atpanic (L, atpanic_handler);

  luaL_openlibs (L);

  if (!bean_lua_utils_load_resource (L, "strict.lua", 0, 0) ||
      !bean_lua_utils_require (L, "lgi") ||
      !bean_lua_utils_check_version (L,
                                     LGI_MAJOR_VERSION,
                                     LGI_MINOR_VERSION,
                                     LGI_MICRO_VERSION))
    {
      /* Already warned */
      return FALSE;
    }

  lua_pushliteral (L, "lock");
  lua_rawget (L, -2);
  priv->lgi_lock = lua_touserdata (L, -1);
  lua_pop (L, 1);

  lua_pushliteral (L, "enter");
  lua_rawget (L, -2);
  priv->lgi_enter_func = lua_touserdata (L, -1);
  lua_pop (L, 1);

  lua_pushliteral (L, "leave");
  lua_rawget (L, -2);
  priv->lgi_leave_func = lua_touserdata (L, -1);
  lua_pop (L, 1);

  if (priv->lgi_lock == NULL ||
      priv->lgi_enter_func == NULL ||
      priv->lgi_leave_func == NULL)
    {
      g_warning ("Failed to find 'lgi.lock', 'lgi.enter' and 'lgi.leave'");
      return FALSE;
    }

  /* Pop lgi's module table */
  lua_pop (L, 1);

  if (!bean_lua_internal_setup (L))
    {
      /* Already warned */
      return FALSE;
    }

  /* Assert that no values were leaked to the stack */
  g_assert_cmpint (lua_gettop (L), ==, 0);

  /* Initially the lock is taken by LGI,
   * release as we are not running Lua code
   */
  priv->lgi_leave_func (priv->lgi_lock);
  return TRUE;
}

static gboolean
bean_plugin_loader_lua_is_global (BeanPluginLoader *loader)
{
  return FALSE;
}

static void
bean_plugin_loader_lua_init (BeanPluginLoaderLua *lua_loader)
{
}

static void
bean_plugin_loader_lua_finalize (GObject *object)
{
  BeanPluginLoaderLua *lua_loader = BEAN_PLUGIN_LOADER_LUA (object);
  BeanPluginLoaderLuaPrivate *priv = GET_PRIV (lua_loader);

  /* Must take the lock as Lua code will run on lua_close
   * and another thread might be running Lua code already
   */
  if (priv->lgi_enter_func != NULL)
    priv->lgi_enter_func (priv->lgi_lock);

  bean_lua_internal_shutdown (priv->L);
  g_clear_pointer (&priv->L, lua_close);

  G_OBJECT_CLASS (bean_plugin_loader_lua_parent_class)->finalize (object);
}

static void
bean_plugin_loader_lua_class_init (BeanPluginLoaderLuaClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  BeanPluginLoaderClass *loader_class = BEAN_PLUGIN_LOADER_CLASS (klass);

  quark_extension_type = g_quark_from_static_string ("bean-extension-type");

  object_class->finalize = bean_plugin_loader_lua_finalize;

  loader_class->initialize = bean_plugin_loader_lua_initialize;
  loader_class->is_global = bean_plugin_loader_lua_is_global;
  loader_class->load = bean_plugin_loader_lua_load;
  loader_class->unload = bean_plugin_loader_lua_unload;
  loader_class->create_extension = bean_plugin_loader_lua_create_extension;
  loader_class->provides_extension = bean_plugin_loader_lua_provides_extension;
  loader_class->garbage_collect = bean_plugin_loader_lua_garbage_collect;
}
