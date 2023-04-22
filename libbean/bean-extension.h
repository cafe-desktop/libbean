/*
 * bean-extension.h
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

#ifndef __BEAN_EXTENSION_H__
#define __BEAN_EXTENSION_H__

#include <glib-object.h>
#include <girepository.h>

#include "bean-version-macros.h"

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define BEAN_TYPE_EXTENSION            (G_TYPE_OBJECT)
#define BEAN_EXTENSION(obj)            (G_OBJECT(obj))
#define BEAN_IS_EXTENSION(obj)         (G_IS_OBJECT(obj))

/**
 * BeanExtension:
 *
 * A proxy class to access the actual plugin.
 */
typedef GObject BeanExtension;

/*
 * All the public methods of BeanExtension are deprecated and should not be
 * used. Due to gi-scanner's touchiness, we also hide these legacy API from
 * GI to avoid hairy issues.
 */
#ifndef __GI_SCANNER__
#ifndef BEAN_DISABLE_DEPRECATED
BEAN_AVAILABLE_IN_ALL
GType        bean_extension_get_type        (void)  G_GNUC_CONST;

BEAN_AVAILABLE_IN_ALL
GType        bean_extension_get_extension_type
                                            (BeanExtension *exten);

BEAN_AVAILABLE_IN_ALL
gboolean     bean_extension_call            (BeanExtension *exten,
                                             const gchar   *method_name,
                                             ...);
BEAN_AVAILABLE_IN_ALL
gboolean     bean_extension_call_valist     (BeanExtension *exten,
                                             const gchar   *method_name,
                                             va_list        args);
BEAN_AVAILABLE_IN_ALL
gboolean     bean_extension_callv           (BeanExtension *exten,
                                             const gchar   *method_name,
                                             GIArgument    *args,
                                             GIArgument    *return_value);
#endif
#endif

G_END_DECLS

#endif /* __BEAN_EXTENSION_H__ */
