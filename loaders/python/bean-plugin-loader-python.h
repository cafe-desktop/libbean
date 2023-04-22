/*
 * bean-plugin-loader-python.h
 * This file is part of libbean
 *
 * Copyright (C) 2008 - Jesse van den Kieboom
 * Copyright (C) 2009 - Steve Frécinaux
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

#ifndef __PEAS_PLUGIN_LOADER_PYTHON_H__
#define __PEAS_PLUGIN_LOADER_PYTHON_H__

#include <libbean/bean-plugin-loader.h>
#include <libbean/bean-object-module.h>

G_BEGIN_DECLS

#define PEAS_TYPE_PLUGIN_LOADER_PYTHON             (bean_plugin_loader_python_get_type ())
#define PEAS_PLUGIN_LOADER_PYTHON(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), PEAS_TYPE_PLUGIN_LOADER_PYTHON, BeanPluginLoaderPython))
#define PEAS_PLUGIN_LOADER_PYTHON_CONST(obj)       (G_TYPE_CHECK_INSTANCE_CAST ((obj), PEAS_TYPE_PLUGIN_LOADER_PYTHON, BeanPluginLoaderPython const))
#define PEAS_PLUGIN_LOADER_PYTHON_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), PEAS_TYPE_PLUGIN_LOADER_PYTHON, BeanPluginLoaderPythonClass))
#define PEAS_IS_PLUGIN_LOADER_PYTHON(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), PEAS_TYPE_PLUGIN_LOADER_PYTHON))
#define PEAS_IS_PLUGIN_LOADER_PYTHON_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), PEAS_TYPE_PLUGIN_LOADER_PYTHON))
#define PEAS_PLUGIN_LOADER_PYTHON_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), PEAS_TYPE_PLUGIN_LOADER_PYTHON, BeanPluginLoaderPythonClass))

typedef struct _BeanPluginLoaderPython         BeanPluginLoaderPython;
typedef struct _BeanPluginLoaderPythonClass    BeanPluginLoaderPythonClass;

struct _BeanPluginLoaderPython {
  BeanPluginLoader parent;
};

struct _BeanPluginLoaderPythonClass {
  BeanPluginLoaderClass parent_class;
};

GType                    bean_plugin_loader_python_get_type  (void) G_GNUC_CONST;

/* All the loaders must implement this function */
G_MODULE_EXPORT void     bean_register_types                 (BeanObjectModule *module);

G_END_DECLS

#endif /* __PEAS_PLUGIN_LOADER_PYTHON_H__ */

