/*
 * bean-dirs.h
 * This file is part of libbean
 *
 * Copyright (C) 2008 Ignacio Casal Quinteiro
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


#ifndef __BEAN_DIRS_H__
#define __BEAN_DIRS_H__

#include <glib.h>

G_BEGIN_DECLS

gchar  *bean_dirs_get_data_dir          (void);
gchar  *bean_dirs_get_lib_dir           (void);
gchar  *bean_dirs_get_plugin_loader_dir (const gchar *loader_name);
gchar  *bean_dirs_get_locale_dir        (void);

G_END_DECLS

#endif /* __BEAN_DIRS_H__ */
