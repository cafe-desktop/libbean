/*
 * bean-extension-set.h
 * This file is part of libbean
 *
 * Copyright (C) 2010 - Steve Frécinaux
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

#ifndef __BEAN_EXTENSION_SET_H__
#define __BEAN_EXTENSION_SET_H__

#include <glib-object.h>

#include "bean-engine.h"
#include "bean-version-macros.h"

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define BEAN_TYPE_EXTENSION_SET            (bean_extension_set_get_type())
#define BEAN_EXTENSION_SET(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), BEAN_TYPE_EXTENSION_SET, BeanExtensionSet))
#define BEAN_EXTENSION_SET_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), BEAN_TYPE_EXTENSION_SET, BeanExtensionSetClass))
#define BEAN_IS_EXTENSION_SET(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), BEAN_TYPE_EXTENSION_SET))
#define BEAN_IS_EXTENSION_SET_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), BEAN_TYPE_EXTENSION_SET))
#define BEAN_EXTENSION_SET_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), BEAN_TYPE_EXTENSION_SET, BeanExtensionSetClass))

typedef struct _BeanExtensionSet         BeanExtensionSet;
typedef struct _BeanExtensionSetClass    BeanExtensionSetClass;
typedef struct _BeanExtensionSetPrivate  BeanExtensionSetPrivate;

/**
 * BeanExtensionSet:
 *
 * The #BeanExtensionSet structure contains only private data and should only
 * be accessed using the provided API.
 */
struct _BeanExtensionSet {
  GObject parent;

  BeanExtensionSetPrivate *priv;
};

/**
 * BeanExtensionSetClass:
 * @parent_class: The parent class.
 * @call: The VFunc for bean_extension_set_call().
 * @extension_added: Signal class handler for the
 *                   #BeanExtensionSet::extension-added signal.
 * @extension_removed: Signal class handler for the
 *                   #BeanExtensionSet::extension-removed signal.
 *
 * The class structure for #BeanExtensionSet.
 */
struct _BeanExtensionSetClass {
  GObjectClass parent_class;

  /* Virtual public methods */
#ifndef BEAN_DISABLE_DEPRECATED
  gboolean   (*call)                      (BeanExtensionSet *set,
                                           const gchar      *method_name,
                                           GIArgument       *args);
#else
  /*< private >*/
  gpointer __DEPRECATED_call;
#endif

  /*< public >*/
  /* Signals */
  void       (*extension_added)           (BeanExtensionSet *set,
                                           BeanPluginInfo   *info,
                                           BeanExtension    *exten);
  void       (*extension_removed)         (BeanExtensionSet *set,
                                           BeanPluginInfo   *info,
                                           BeanExtension    *exten);

  /*< private >*/
  gpointer padding[8];
};

/**
 * BeanExtensionSetForeachFunc:
 * @set: A #BeanExtensionSet.
 * @info: A #BeanPluginInfo.
 * @exten: A #BeanExtension.
 * @data: Optional data passed to the function.
 *
 * This function is passed to bean_extension_set_foreach() and
 * will be called for each extension in @set.
 *
 * Since: 1.2
 */
typedef void (*BeanExtensionSetForeachFunc) (BeanExtensionSet *set,
                                             BeanPluginInfo   *info,
                                             BeanExtension    *exten,
                                             gpointer          data);

/*
 * Public methods
 */
BEAN_AVAILABLE_IN_ALL
GType              bean_extension_set_get_type    (void)  G_GNUC_CONST;

#ifndef __GI_SCANNER__
#ifndef BEAN_DISABLE_DEPRECATED
BEAN_AVAILABLE_IN_ALL
gboolean           bean_extension_set_call        (BeanExtensionSet *set,
                                                   const gchar      *method_name,
                                                   ...);
BEAN_AVAILABLE_IN_ALL
gboolean           bean_extension_set_call_valist (BeanExtensionSet *set,
                                                   const gchar      *method_name,
                                                   va_list           va_args);
BEAN_AVAILABLE_IN_ALL
gboolean           bean_extension_set_callv       (BeanExtensionSet *set,
                                                   const gchar      *method_name,
                                                   GIArgument       *args);
#endif
#endif

BEAN_AVAILABLE_IN_ALL
void               bean_extension_set_foreach     (BeanExtensionSet *set,
                                                   BeanExtensionSetForeachFunc func,
                                                   gpointer          data);

BEAN_AVAILABLE_IN_ALL
BeanExtension     *bean_extension_set_get_extension (BeanExtensionSet *set,
                                                     BeanPluginInfo   *info);

BEAN_AVAILABLE_IN_ALL
BeanExtensionSet  *bean_extension_set_newv        (BeanEngine       *engine,
                                                   GType             exten_type,
                                                   guint             n_parameters,
                                                   GParameter       *parameters);

BEAN_AVAILABLE_IN_1_24
BeanExtensionSet  *bean_extension_set_new_with_properties (BeanEngine    *engine,
                                                           GType          exten_type,
                                                           guint          n_properties,
                                                           const gchar  **prop_names,
                                                           const GValue  *prop_values);
BEAN_AVAILABLE_IN_ALL
BeanExtensionSet  *bean_extension_set_new_valist  (BeanEngine       *engine,
                                                   GType             exten_type,
                                                   const gchar      *first_property,
                                                   va_list           var_args);
BEAN_AVAILABLE_IN_ALL
BeanExtensionSet  *bean_extension_set_new         (BeanEngine       *engine,
                                                   GType             exten_type,
                                                   const gchar      *first_property,
                                                   ...);

G_END_DECLS

#endif /* __BEAN_EXTENSION_SET_H__ */
