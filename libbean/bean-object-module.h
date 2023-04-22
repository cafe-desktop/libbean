/*
 * bean-object-module.h
 * This file is part of libbean
 *
 * Copyright (C) 2003 Marco Pesenti Gritti
 * Copyright (C) 2003, 2004 Christian Persch
 * Copyright (C) 2005-2007 Paolo Maggi
 * Copyright (C) 2008 Jesse van den Kieboom
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

#ifndef __PEAS_OBJECT_MODULE_H__
#define __PEAS_OBJECT_MODULE_H__

#include <glib-object.h>
#include <gmodule.h>

#include "bean-version-macros.h"

G_BEGIN_DECLS

#define PEAS_TYPE_OBJECT_MODULE             (bean_object_module_get_type ())
#define PEAS_OBJECT_MODULE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), PEAS_TYPE_OBJECT_MODULE, BeanObjectModule))
#define PEAS_OBJECT_MODULE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), PEAS_TYPE_OBJECT_MODULE, BeanObjectModuleClass))
#define PEAS_IS_OBJECT_MODULE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), PEAS_TYPE_OBJECT_MODULE))
#define PEAS_IS_OBJECT_MODULE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), PEAS_TYPE_OBJECT_MODULE))
#define PEAS_OBJECT_MODULE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), PEAS_TYPE_OBJECT_MODULE, BeanObjectModuleClass))

typedef struct _BeanObjectModule         BeanObjectModule;
typedef struct _BeanObjectModuleClass    BeanObjectModuleClass;
typedef struct _BeanObjectModulePrivate  BeanObjectModulePrivate;

G_GNUC_BEGIN_IGNORE_DEPRECATIONS
/**
 * BeanFactoryFunc:
 * @n_parameters: The number of paramteters.
 * @parameters: (array length=n_parameters): The parameters.
 * @user_data: Optional data to be passed to the function, or %NULL.
 *
 * A #BeanFactoryFunc is a factory function which will instanciate a new
 * extension of a given type. g_object_newv() is such a function.
 *
 * It is used with bean_object_module_register_extension_factory().
 *
 * Return value: (transfer full): The created object.
 */
typedef GObject *(*BeanFactoryFunc)   (guint          n_parameters,
                                       GParameter    *parameters,
                                       gpointer       user_data);
G_GNUC_END_IGNORE_DEPRECATIONS

/**
 * BeanObjectModule:
 *
 * The #BeanObjectModule structure contains only private data and should only
 * be accessed using the provided API.
 */
struct _BeanObjectModule {
  GTypeModule parent;

  BeanObjectModulePrivate *priv;
};

/**
 * BeanObjectModuleClass:
 * @parent_class: The parent class.
 *
 * The class structure for #BeanObjectModule.
 */
struct _BeanObjectModuleClass {
  GTypeModuleClass parent_class;

  /*< private >*/
  gpointer padding[8];
};

PEAS_AVAILABLE_IN_ALL
GType               bean_object_module_get_type               (void) G_GNUC_CONST;
PEAS_AVAILABLE_IN_ALL
BeanObjectModule   *bean_object_module_new                    (const gchar      *module_name,
                                                               const gchar      *path,
                                                               gboolean          resident);
PEAS_AVAILABLE_IN_ALL
BeanObjectModule   *bean_object_module_new_full               (const gchar      *module_name,
                                                               const gchar      *path,
                                                               gboolean          resident,
                                                               gboolean          local_linkage);
PEAS_AVAILABLE_IN_ALL
BeanObjectModule   *bean_object_module_new_embedded           (const gchar      *module_name,
                                                               const gchar      *symbol);

G_GNUC_BEGIN_IGNORE_DEPRECATIONS
PEAS_AVAILABLE_IN_ALL
GObject            *bean_object_module_create_object          (BeanObjectModule *module,
                                                               GType             exten_type,
                                                               guint             n_parameters,
                                                               GParameter       *parameters);
G_GNUC_END_IGNORE_DEPRECATIONS
PEAS_AVAILABLE_IN_ALL
gboolean            bean_object_module_provides_object        (BeanObjectModule *module,
                                                               GType             exten_type);

PEAS_AVAILABLE_IN_ALL
const gchar        *bean_object_module_get_path               (BeanObjectModule *module);
PEAS_AVAILABLE_IN_ALL
const gchar        *bean_object_module_get_module_name        (BeanObjectModule *module);
PEAS_AVAILABLE_IN_ALL
const gchar        *bean_object_module_get_symbol             (BeanObjectModule *module);

PEAS_AVAILABLE_IN_ALL
GModule            *bean_object_module_get_library            (BeanObjectModule *module);

PEAS_AVAILABLE_IN_ALL
void                bean_object_module_register_extension_factory
                                                              (BeanObjectModule *module,
                                                               GType             exten_type,
                                                               BeanFactoryFunc   factory_func,
                                                               gpointer          user_data,
                                                               GDestroyNotify    destroy_func);
PEAS_AVAILABLE_IN_ALL
void                bean_object_module_register_extension_type
                                                              (BeanObjectModule *module,
                                                               GType             exten_type,
                                                               GType             impl_type);

G_END_DECLS

#endif /* __PEAS_OBJECT_MODULE_H__ */
