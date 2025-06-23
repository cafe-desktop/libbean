/*
 * bean-dirs.c
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "bean-dirs.h"

#ifdef OS_OSX
#include "bean-utils-osx.h"
#endif

#ifdef G_OS_WIN32
// inspired by gobject-introspection...

#include <libloaderapi.h>

static HMODULE libbean_dll = NULL;

#ifdef DLL_EXPORT

BOOL WINAPI DllMain (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);

BOOL WINAPI DllMain (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
  if (fdwReason == DLL_PROCESS_ATTACH)
    libbean_dll = hinstDLL;
  return TRUE;
}

#endif
#endif

gchar *
bean_dirs_get_data_dir (void)
{
  gchar *data_dir;

#ifdef G_OS_WIN32
  gchar *win32_dir;

  win32_dir = g_win32_get_package_installation_directory_of_module (libbean_dll);

  data_dir = g_build_filename (win32_dir, "share", "libbean-2.0", NULL);
  g_free (win32_dir);
#elif defined (OS_OSX)
  data_dir = bean_dirs_os_x_get_data_dir ();
#else
  data_dir = g_build_filename (DATADIR, "libbean-2.0", NULL);
#endif

  return data_dir;
}

gchar *
bean_dirs_get_lib_dir (void)
{
  gchar *lib_dir;

#ifdef G_OS_WIN32
  gchar *win32_dir;

  win32_dir = g_win32_get_package_installation_directory_of_module (libbean_dll);

  lib_dir = g_build_filename (win32_dir, "lib", "libbean-2.0", NULL);
  g_free (win32_dir);
#elif defined (OS_OSX)
  lib_dir = bean_dirs_os_x_get_lib_dir ();
#else
  lib_dir = g_build_filename (LIBDIR, "libbean-2.0", NULL);
#endif

  return lib_dir;
}

gchar *
bean_dirs_get_plugin_loader_dir (const gchar *loader_name)
{
  const gchar *env_var;
  gchar *lib_dir;
  gchar *loader_dir;

  env_var = g_getenv ("BEAN_PLUGIN_LOADERS_DIR");
  if (env_var != NULL)
    return g_build_filename (env_var, loader_name, NULL);

  lib_dir = bean_dirs_get_lib_dir ();
  loader_dir = g_build_filename (lib_dir, "loaders", NULL);

  g_free (lib_dir);

  return loader_dir;
}

gchar *
bean_dirs_get_locale_dir (void)
{
  gchar *locale_dir;

#ifdef G_OS_WIN32
  gchar *win32_dir;

  win32_dir = g_win32_get_package_installation_directory_of_module (libbean_dll);

  locale_dir = g_build_filename (win32_dir, "share", "locale", NULL);

  g_free (win32_dir);
#elif defined (OS_OSX)
  locale_dir = bean_dirs_os_x_get_locale_dir ();
#else
  locale_dir = g_build_filename (DATADIR, "locale", NULL);
#endif

  return locale_dir;
}

