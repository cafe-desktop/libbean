/*
 * bean-plugin-loader-lua.h
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

#ifndef __BEAN_PLUGIN_LOADER_LUA_H__
#define __BEAN_PLUGIN_LOADER_LUA_H__

#include <libbean/bean-plugin-loader.h>
#include <libbean/bean-object-module.h>

G_BEGIN_DECLS

#define BEAN_TYPE_PLUGIN_LOADER_LUA             (bean_plugin_loader_lua_get_type ())
#define BEAN_PLUGIN_LOADER_LUA(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), BEAN_TYPE_PLUGIN_LOADER_LUA, BeanPluginLoaderLua))
#define BEAN_PLUGIN_LOADER_LUA_CONST(obj)       (G_TYPE_CHECK_INSTANCE_CAST ((obj), BEAN_TYPE_PLUGIN_LOADER_LUA, BeanPluginLoaderLua const))
#define BEAN_PLUGIN_LOADER_LUA_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), BEAN_TYPE_PLUGIN_LOADER_LUA, BeanPluginLoaderLuaClass))
#define BEAN_IS_PLUGIN_LOADER_LUA(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BEAN_TYPE_PLUGIN_LOADER_LUA))
#define BEAN_IS_PLUGIN_LOADER_LUA_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), BEAN_TYPE_PLUGIN_LOADER_LUA))
#define BEAN_PLUGIN_LOADER_LUA_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), BEAN_TYPE_PLUGIN_LOADER_LUA, BeanPluginLoaderLuaClass))

typedef struct _BeanPluginLoaderLua         BeanPluginLoaderLua;
typedef struct _BeanPluginLoaderLuaClass    BeanPluginLoaderLuaClass;

struct _BeanPluginLoaderLua {
  BeanPluginLoader parent;
};

struct _BeanPluginLoaderLuaClass {
  BeanPluginLoaderClass parent_class;
};

GType                    bean_plugin_loader_lua_get_type  (void) G_GNUC_CONST;

/* All the loaders must implement this function */
G_MODULE_EXPORT void     bean_register_types              (BeanObjectModule *module);

G_END_DECLS

#endif /* __BEAN_PLUGIN_LOADER_LUA_H__ */

