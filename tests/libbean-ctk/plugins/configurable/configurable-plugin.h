/*
 * configurable-plugin.h
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

#ifndef __TESTING_CONFIGURABLE_PLUGIN_H__
#define __TESTING_CONFIGURABLE_PLUGIN_H__

#include <libbean/bean.h>

G_BEGIN_DECLS

#define TESTING_TYPE_CONFIGURABLE_PLUGIN         (testing_configurable_plugin_get_type ())
#define TESTING_CONFIGURABLE_PLUGIN(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), TESTING_TYPE_CONFIGURABLE_PLUGIN, TestingConfigurablePlugin))
#define TESTING_CONFIGURABLE_PLUGIN_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), TESTING_TYPE_CONFIGURABLE_PLUGIN, TestingConfigurablePlugin))
#define TESTING_IS_CONFIGURABLE_PLUGIN(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), TESTING_TYPE_CONFIGURABLE_PLUGIN))
#define TESTING_IS_CONFIGURABLE_PLUGIN_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), TESTING_TYPE_CONFIGURABLE_PLUGIN))
#define TESTING_CONFIGURABLE_PLUGIN_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), TESTING_TYPE_CONFIGURABLE_PLUGIN, TestingConfigurablePluginClass))

typedef struct _TestingConfigurablePlugin         TestingConfigurablePlugin;
typedef struct _TestingConfigurablePluginClass    TestingConfigurablePluginClass;

struct _TestingConfigurablePlugin {
  BeanExtensionBase parent_instance;
};

struct _TestingConfigurablePluginClass {
  BeanExtensionBaseClass parent_class;
};

GType                 testing_configurable_plugin_get_type (void) G_GNUC_CONST;
G_MODULE_EXPORT void  bean_register_types                  (BeanObjectModule *module);

G_END_DECLS

#endif /* __TESTING_CONFIGURABLE_PLUGIN_H__ */
