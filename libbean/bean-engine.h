/*
 * bean-engine.h
 * This file is part of libbean
 *
 * Copyright (C) 2002-2005 - Paolo Maggi
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

#ifndef __PEAS_ENGINE_H__
#define __PEAS_ENGINE_H__

#include <glib.h>

#include "bean-plugin-info.h"
#include "bean-extension.h"
#include "bean-version-macros.h"

G_BEGIN_DECLS

#define PEAS_TYPE_ENGINE              (bean_engine_get_type ())
#define PEAS_ENGINE(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), PEAS_TYPE_ENGINE, BeanEngine))
#define PEAS_ENGINE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), PEAS_TYPE_ENGINE, BeanEngineClass))
#define PEAS_IS_ENGINE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), PEAS_TYPE_ENGINE))
#define PEAS_IS_ENGINE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), PEAS_TYPE_ENGINE))
#define PEAS_ENGINE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), PEAS_TYPE_ENGINE, BeanEngineClass))

typedef struct _BeanEngine         BeanEngine;
typedef struct _BeanEngineClass    BeanEngineClass;
typedef struct _BeanEnginePrivate  BeanEnginePrivate;

/**
 * BeanEngine:
 *
 * The #BeanEngine structure contains only private data and should only be
 * accessed using the provided API.
 */
struct _BeanEngine {
  GObject parent;

  /*< private > */
  BeanEnginePrivate *priv;
};

/**
 * BeanEngineClass:
 * @parent_class: The parent class.
 * @load_plugin: Signal class handler for the #BeanEngine::load-plugin signal.
 * @unload_plugin: Signal class handler for the #BeanEngine::unload-plugin signal.
 *
 * Class structure for #BeanEngine.
 */
struct _BeanEngineClass {
  GObjectClass parent_class;

  void     (*load_plugin)                 (BeanEngine     *engine,
                                           BeanPluginInfo *info);

  void     (*unload_plugin)               (BeanEngine     *engine,
                                           BeanPluginInfo *info);

  /*< private >*/
  gpointer padding[8];
};

PEAS_AVAILABLE_IN_ALL
GType             bean_engine_get_type            (void) G_GNUC_CONST;
PEAS_AVAILABLE_IN_ALL
BeanEngine       *bean_engine_new                 (void);
PEAS_AVAILABLE_IN_ALL
BeanEngine       *bean_engine_new_with_nonglobal_loaders
                                                  (void);
PEAS_AVAILABLE_IN_ALL
BeanEngine       *bean_engine_get_default         (void);

PEAS_AVAILABLE_IN_ALL
void              bean_engine_add_search_path     (BeanEngine      *engine,
                                                   const gchar     *module_dir,
                                                   const gchar     *data_dir);
PEAS_AVAILABLE_IN_ALL
void              bean_engine_prepend_search_path (BeanEngine      *engine,
                                                   const gchar     *module_dir,
                                                   const gchar     *data_dir);

/* plugin management */
PEAS_AVAILABLE_IN_ALL
void              bean_engine_enable_loader       (BeanEngine      *engine,
                                                   const gchar     *loader_name);
PEAS_AVAILABLE_IN_ALL
void              bean_engine_rescan_plugins      (BeanEngine      *engine);
PEAS_AVAILABLE_IN_ALL
const GList      *bean_engine_get_plugin_list     (BeanEngine      *engine);
PEAS_AVAILABLE_IN_ALL
gchar           **bean_engine_get_loaded_plugins  (BeanEngine      *engine);
PEAS_AVAILABLE_IN_ALL
void              bean_engine_set_loaded_plugins  (BeanEngine      *engine,
                                                   const gchar    **plugin_names);
PEAS_AVAILABLE_IN_ALL
BeanPluginInfo   *bean_engine_get_plugin_info     (BeanEngine      *engine,
                                                   const gchar     *plugin_name);

/* plugin loading and unloading */
PEAS_AVAILABLE_IN_ALL
gboolean          bean_engine_load_plugin         (BeanEngine      *engine,
                                                   BeanPluginInfo  *info);
PEAS_AVAILABLE_IN_ALL
gboolean          bean_engine_unload_plugin       (BeanEngine      *engine,
                                                   BeanPluginInfo  *info);
PEAS_AVAILABLE_IN_ALL
void              bean_engine_garbage_collect     (BeanEngine      *engine);

PEAS_AVAILABLE_IN_ALL
gboolean          bean_engine_provides_extension  (BeanEngine      *engine,
                                                   BeanPluginInfo  *info,
                                                   GType            extension_type);


G_GNUC_BEGIN_IGNORE_DEPRECATIONS
PEAS_AVAILABLE_IN_ALL
BeanExtension    *bean_engine_create_extensionv   (BeanEngine      *engine,
                                                   BeanPluginInfo  *info,
                                                   GType            extension_type,
                                                   guint            n_parameters,
                                                   GParameter      *parameters);
G_GNUC_END_IGNORE_DEPRECATIONS

PEAS_AVAILABLE_IN_1_24
BeanExtension    *bean_engine_create_extension_with_properties
                                                  (BeanEngine     *engine,
                                                   BeanPluginInfo *info,
                                                   GType           extension_type,
                                                   guint           n_properties,
                                                   const gchar   **prop_names,
                                                   const GValue   *prop_values);

PEAS_AVAILABLE_IN_ALL
BeanExtension    *bean_engine_create_extension_valist
                                                  (BeanEngine      *engine,
                                                   BeanPluginInfo  *info,
                                                   GType            extension_type,
                                                   const gchar     *first_property,
                                                   va_list          var_args);
PEAS_AVAILABLE_IN_ALL
BeanExtension    *bean_engine_create_extension    (BeanEngine      *engine,
                                                   BeanPluginInfo  *info,
                                                   GType            extension_type,
                                                   const gchar     *first_property,
                                                   ...);


G_END_DECLS

#endif /* __PEAS_ENGINE_H__ */
