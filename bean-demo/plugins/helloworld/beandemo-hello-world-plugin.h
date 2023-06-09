/*
 * beandemo-hello-world-plugin.h
 * This file is part of libbean
 *
 * Copyright (C) 2009-2010 Steve Frécinaux
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

#ifndef __BEANDEMO_HELLO_WORLD_PLUGIN_H__
#define __BEANDEMO_HELLO_WORLD_PLUGIN_H__

#include <ctk/ctk.h>
#include <libbean/bean.h>

G_BEGIN_DECLS

#define BEANDEMO_TYPE_HELLO_WORLD_PLUGIN         (beandemo_hello_world_plugin_get_type ())
#define BEANDEMO_HELLO_WORLD_PLUGIN(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), BEANDEMO_TYPE_HELLO_WORLD_PLUGIN, BeanDemoHelloWorldPlugin))
#define BEANDEMO_HELLO_WORLD_PLUGIN_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), BEANDEMO_TYPE_HELLO_WORLD_PLUGIN, BeanDemoHelloWorldPlugin))
#define BEANDEMO_IS_HELLO_WORLD_PLUGIN(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), BEANDEMO_TYPE_HELLO_WORLD_PLUGIN))
#define BEANDEMO_IS_HELLO_WORLD_PLUGIN_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), BEANDEMO_TYPE_HELLO_WORLD_PLUGIN))
#define BEANDEMO_HELLO_WORLD_PLUGIN_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), BEANDEMO_TYPE_HELLO_WORLD_PLUGIN, BeanDemoHelloWorldPluginClass))

typedef struct _BeanDemoHelloWorldPlugin       BeanDemoHelloWorldPlugin;
typedef struct _BeanDemoHelloWorldPluginClass  BeanDemoHelloWorldPluginClass;

struct _BeanDemoHelloWorldPlugin {
  BeanExtensionBase parent_instance;

  CtkWidget *window;
  CtkWidget *label;
};

struct _BeanDemoHelloWorldPluginClass {
  BeanExtensionBaseClass parent_class;
};

GType                 beandemo_hello_world_plugin_get_type        (void) G_GNUC_CONST;
G_MODULE_EXPORT void  bean_register_types                         (BeanObjectModule *module);

G_END_DECLS

#endif /* __BEANDEMO_HELLO_WORLD_PLUGIN_H__ */
