/*
 * Copyright (C) 1997, 1998, 1999, 2000 Free Software Foundation
 * All rights reserved.
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <glib/gi18n.h>
#include <string.h>

#include "bean-dirs.h"

#include "gconstructor.h"

#if defined (G_HAS_CONSTRUCTORS)
# ifdef G_DEFINE_CONSTRUCTOR_NEEDS_PRAGMA
#  pragma G_DEFINE_CONSTRUCTOR_PRAGMA_ARGS(bean_init_ctor)
# endif
G_DEFINE_CONSTRUCTOR(bean_init_ctor)
#else
# error Your platform/compiler is missing constructor support
#endif

static void
bean_init_ctor (void)
{
  gchar *locale_dir = bean_dirs_get_locale_dir ();

  bindtextdomain (GETTEXT_PACKAGE, locale_dir);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");

  g_free (locale_dir);
}
