/*
 * bean-python-internal.h
 * This file is part of libbean
 *
 * Copyright (C) 2015 - Garrett Regier
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

#ifndef __BEAN_PYTHON_INTERNAL_H__
#define __BEAN_PYTHON_INTERNAL_H__

#include <glib.h>

/* _POSIX_C_SOURCE is defined in Python.h and in limits.h included by
 * glib-object.h, so we unset it here to avoid a warning. Yep, that's bad.
 */
#undef _POSIX_C_SOURCE
#include <Python.h>

G_BEGIN_DECLS

gboolean  bean_python_internal_setup    (gboolean     already_initialized);
void      bean_python_internal_shutdown (void);

PyObject *bean_python_internal_call     (const gchar  *name,
                                         PyTypeObject *return_type,
                                         const gchar  *format,
                                         ...);

G_END_DECLS

#endif /* __BEAN_PYTHON_INTERNAL_H__ */
