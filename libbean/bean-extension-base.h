/*
 * bean-extension-base.h
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

#ifndef __PEAS_EXTENSION_BASE_H__
#define __PEAS_EXTENSION_BASE_H__

#include <glib-object.h>

#include "bean-plugin-info.h"
#include "bean-version-macros.h"

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define PEAS_TYPE_EXTENSION_BASE            (bean_extension_base_get_type())
#define PEAS_EXTENSION_BASE(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), PEAS_TYPE_EXTENSION_BASE, BeanExtensionBase))
#define PEAS_EXTENSION_BASE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), PEAS_TYPE_EXTENSION_BASE, BeanExtensionBaseClass))
#define PEAS_IS_EXTENSION_BASE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), PEAS_TYPE_EXTENSION_BASE))
#define PEAS_IS_EXTENSION_BASE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), PEAS_TYPE_EXTENSION_BASE))
#define PEAS_EXTENSION_BASE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), PEAS_TYPE_EXTENSION_BASE, BeanExtensionBaseClass))

/**
 * BeanExtensionBase:
 *
 * Base class for C extensions.
 */
typedef struct _BeanExtensionBase        BeanExtensionBase;
typedef struct _BeanExtensionBaseClass   BeanExtensionBaseClass;
typedef struct _BeanExtensionBasePrivate BeanExtensionBasePrivate;

struct _BeanExtensionBase {
  GObject parent;

  BeanExtensionBasePrivate *priv;
};

/**
 * BeanExtensionBaseClass:
 * @parent_class: The parent class.
 *
 * The class structure of #BeanExtensionBase.
 */
struct _BeanExtensionBaseClass {
  GObjectClass parent_class;

  /*< private >*/
  gpointer padding[8];
};

/*
 * Public methods
 */
PEAS_AVAILABLE_IN_ALL
GType            bean_extension_base_get_type         (void)  G_GNUC_CONST;

PEAS_AVAILABLE_IN_ALL
BeanPluginInfo  *bean_extension_base_get_plugin_info  (BeanExtensionBase *extbase);
PEAS_AVAILABLE_IN_ALL
gchar           *bean_extension_base_get_data_dir     (BeanExtensionBase *extbase);

G_END_DECLS

#endif /* __PEAS_EXTENSION_BASE_H__ */
