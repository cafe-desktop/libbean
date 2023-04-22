/*
 * beandemo-hello-world-configurable.h
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

#ifndef __PEASDEMO_HELLO_WORLD_CONFIGURABLE_H__
#define __PEASDEMO_HELLO_WORLD_CONFIGURABLE_H__

#include <gtk/gtk.h>
#include <libbean/bean.h>

G_BEGIN_DECLS

#define PEASDEMO_TYPE_HELLO_WORLD_CONFIGURABLE         (beandemo_hello_world_configurable_get_type ())
#define PEASDEMO_HELLO_WORLD_CONFIGURABLE(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), PEASDEMO_TYPE_HELLO_WORLD_CONFIGURABLE, BeanDemoHelloWorldConfigurable))
#define PEASDEMO_HELLO_WORLD_CONFIGURABLE_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), PEASDEMO_TYPE_HELLO_WORLD_CONFIGURABLE, BeanDemoHelloWorldConfigurable))
#define PEASDEMO_IS_HELLO_WORLD_CONFIGURABLE(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), PEASDEMO_TYPE_HELLO_WORLD_CONFIGURABLE))
#define PEASDEMO_IS_HELLO_WORLD_CONFIGURABLE_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), PEASDEMO_TYPE_HELLO_WORLD_CONFIGURABLE))
#define PEASDEMO_HELLO_WORLD_CONFIGURABLE_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), PEASDEMO_TYPE_HELLO_WORLD_CONFIGURABLE, BeanDemoHelloWorldConfigurableClass))

typedef struct _BeanDemoHelloWorldConfigurable      BeanDemoHelloWorldConfigurable;
typedef struct _BeanDemoHelloWorldConfigurableClass BeanDemoHelloWorldConfigurableClass;

struct _BeanDemoHelloWorldConfigurable {
  BeanExtensionBase parent;
};

struct _BeanDemoHelloWorldConfigurableClass {
  BeanExtensionBaseClass parent_class;
};

GType   beandemo_hello_world_configurable_get_type  (void) G_GNUC_CONST;
void    beandemo_hello_world_configurable_register  (GTypeModule *module);

G_END_DECLS

#endif /* __PEASDEMO_HELLO_WORLD_CONFIGURABLE_H__ */
