/*
 * bean-plugin-loader.h
 * This file is part of libbean
 *
 * Copyright (C) 2008 - Jesse van den Kieboom
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

#ifndef __BEAN_PLUGIN_LOADER_H__
#define __BEAN_PLUGIN_LOADER_H__

#include <glib-object.h>
#include <gmodule.h>
#include "bean-plugin-info.h"
#include "bean-extension.h"

G_BEGIN_DECLS

#define BEAN_TYPE_PLUGIN_LOADER                (bean_plugin_loader_get_type ())
#define BEAN_PLUGIN_LOADER(obj)                (G_TYPE_CHECK_INSTANCE_CAST ((obj), BEAN_TYPE_PLUGIN_LOADER, BeanPluginLoader))
#define BEAN_PLUGIN_LOADER_CLASS(klass)        (G_TYPE_CHECK_CLASS_CAST((klass), BEAN_TYPE_PLUGIN_LOADER, BeanPluginLoaderClass))
#define BEAN_IS_PLUGIN_LOADER(obj)             (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BEAN_TYPE_PLUGIN_LOADER))
#define BEAN_IS_PLUGIN_LOADER_CLASS(klass)     (G_TYPE_CHECK_CLASS_TYPE ((klass), BEAN_TYPE_PLUGIN_LOADER))
#define BEAN_PLUGIN_LOADER_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS ((obj), BEAN_TYPE_PLUGIN_LOADER, BeanPluginLoaderClass))

typedef struct _BeanPluginLoader      BeanPluginLoader;
typedef struct _BeanPluginLoaderClass BeanPluginLoaderClass;

struct _BeanPluginLoader {
  GObject parent;
};

struct _BeanPluginLoaderClass {
  GObjectClass parent;

  gboolean       (*initialize)            (BeanPluginLoader *loader);
  gboolean       (*is_global)             (BeanPluginLoader *loader);

  gboolean       (*load)                  (BeanPluginLoader *loader,
                                           BeanPluginInfo   *info);
  void           (*unload)                (BeanPluginLoader *loader,
                                           BeanPluginInfo   *info);
  gboolean       (*provides_extension)    (BeanPluginLoader *loader,
                                           BeanPluginInfo   *info,
                                           GType             ext_type);
  BeanExtension *(*create_extension)      (BeanPluginLoader *loader,
                                           BeanPluginInfo   *info,
                                           GType             ext_type,
                                           guint             n_parameters,
                                           GParameter       *parameters);

  void           (*garbage_collect)       (BeanPluginLoader *loader);
};

BEAN_AVAILABLE_IN_ALL
GType         bean_plugin_loader_get_type             (void)  G_GNUC_CONST;

BEAN_AVAILABLE_IN_ALL
gboolean      bean_plugin_loader_initialize           (BeanPluginLoader *loader);
BEAN_AVAILABLE_IN_ALL
gboolean      bean_plugin_loader_is_global            (BeanPluginLoader *loader);

BEAN_AVAILABLE_IN_ALL
gboolean      bean_plugin_loader_load                 (BeanPluginLoader *loader,
                                                       BeanPluginInfo   *info);
BEAN_AVAILABLE_IN_ALL
void          bean_plugin_loader_unload               (BeanPluginLoader *loader,
                                                       BeanPluginInfo   *info);

BEAN_AVAILABLE_IN_ALL
gboolean      bean_plugin_loader_provides_extension   (BeanPluginLoader *loader,
                                                       BeanPluginInfo   *info,
                                                       GType             ext_type);
BEAN_AVAILABLE_IN_ALL
BeanExtension *bean_plugin_loader_create_extension    (BeanPluginLoader *loader,
                                                       BeanPluginInfo   *info,
                                                       GType             ext_type,
                                                       guint             n_parameters,
                                                       GParameter       *parameters);
BEAN_AVAILABLE_IN_ALL
void          bean_plugin_loader_garbage_collect      (BeanPluginLoader *loader);

G_END_DECLS

#endif /* __BEAN_PLUGIN_LOADER_H__ */
