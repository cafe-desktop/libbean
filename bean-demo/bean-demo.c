/*
 * bean-demo.c
 * This file is part of libbean
 *
 * Copyright (C) 2009-2010 Steve Frécinaux
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

#include <girepository.h>
#include <glib/gi18n.h>
#include <ctk/ctk.h>
#include <locale.h>

#include <libbean/bean.h>
#include <libbean-ctk/bean-ctk.h>

#include "bean-demo-window.h"

gboolean run_from_build_dir;
static CtkWidget *main_window;
static int n_windows;

static GOptionEntry demo_args[] = {
  { "run-from-build-dir", 'b', 0, G_OPTION_ARG_NONE, &run_from_build_dir,
    /* Translators: The directory in which the demo program was compiled at */
    N_("Run from build directory"), NULL },
  { NULL }
};

static void
create_new_window (void)
{
  CtkWidget *window;

  window = demo_window_new ();
  ctk_widget_show_all (window);
}

static CtkWidget *
create_main_window (void)
{
  CtkWidget *window;
  CtkWidget *box;
  CtkWidget *manager;
  CtkWidget *scrolled_window;
  CtkWidget *button_box;
  CtkWidget *button;

  ctk_window_set_default_icon_name ("libbean-plugin");

  window = ctk_window_new (GTK_WINDOW_TOPLEVEL);
  g_signal_connect (window, "delete-event", G_CALLBACK (ctk_main_quit), NULL);
  ctk_container_set_border_width (GTK_CONTAINER (window), 6);
  ctk_window_set_title (GTK_WINDOW (window), "Bean Demo");

  box = ctk_box_new (GTK_ORIENTATION_VERTICAL, 6);
  ctk_container_add (GTK_CONTAINER (window), box);

  manager = bean_ctk_plugin_manager_new (bean_engine_get_default ());
  ctk_box_pack_start (GTK_BOX (box), manager, TRUE, TRUE, 0);

  /* Always show all plugins, there are only a few */
  scrolled_window = ctk_test_find_sibling (manager,
                                           GTK_TYPE_SCROLLED_WINDOW);
  ctk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
                                  GTK_POLICY_NEVER, GTK_POLICY_NEVER);

  button_box = ctk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
  ctk_box_set_spacing (GTK_BOX (button_box), 6);
  ctk_button_box_set_layout (GTK_BUTTON_BOX (button_box), GTK_BUTTONBOX_END);
  ctk_box_pack_start (GTK_BOX (box), button_box, FALSE, FALSE, 0);

  button = ctk_button_new_with_label ("New window");
  g_signal_connect (button, "clicked", G_CALLBACK (create_new_window), NULL);
  ctk_container_add (GTK_CONTAINER (button_box), button);

  button = ctk_button_new_with_mnemonic (_("_Quit"));
  g_signal_connect (button, "clicked", G_CALLBACK (ctk_main_quit), NULL);
  ctk_container_add (GTK_CONTAINER (button_box), button);

  return window;
}

int
main (int    argc,
      char **argv)
{
  GOptionContext *option_context;
  GError *error = NULL;
  gchar *plugin_dir;
  BeanEngine *engine;

  setlocale (LC_ALL, "");
  /* Normally, we'd need to call bindtextdomain() here. But that would require
   * access to the dynamic bean_dirs_get_locale_dir() which is not available
   * via the ABI. However, libbean has a static constructor for it so we do
   * not need to call it anyway.
   */
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);

  option_context = g_option_context_new (_("— libbean demo application"));
  g_option_context_add_main_entries (option_context, demo_args, GETTEXT_PACKAGE);
  g_option_context_add_group (option_context, ctk_get_option_group (TRUE));

  if (!g_option_context_parse (option_context, &argc, &argv, &error))
    {
      g_warning ("Error while parsing arguments: %s", error->message);
      g_error_free (error);
      return -1;
    }

  g_option_context_free (option_context);

  if (run_from_build_dir)
    {
      g_debug ("Running from build directory: %s", BEAN_BUILDDIR);

      /* Use the uninstalled typelibs */
      g_irepository_prepend_search_path (BEAN_BUILDDIR "/libbean");
      g_irepository_prepend_search_path (BEAN_BUILDDIR "/libbean-ctk");

      /* Use the uninstalled plugin loaders */
      g_setenv ("BEAN_PLUGIN_LOADERS_DIR", BEAN_BUILDDIR "/loaders", TRUE);
    }

  engine = bean_engine_get_default ();
  plugin_dir = g_build_filename (g_get_user_config_dir (), "bean-demo/plugins", NULL);
  bean_engine_add_search_path (engine, plugin_dir, plugin_dir);
  g_free (plugin_dir);

  /* We don't care about leaking memory */
  g_setenv ("BEAN_ALLOW_ALL_LOADERS", "1", TRUE);
  bean_engine_enable_loader (engine, "lua5.1");
  bean_engine_enable_loader (engine, "python3");

  if (run_from_build_dir)
    bean_engine_add_search_path (engine, BEAN_BUILDDIR "/bean-demo/plugins", NULL);
  else
    bean_engine_add_search_path (engine,
                                 BEAN_LIBDIR "/bean-demo/plugins/",
                                 BEAN_PREFIX "/share/bean-demo/plugins");

  n_windows = 0;
  main_window = create_main_window ();
  ctk_widget_show_all (main_window);

  ctk_main ();

  ctk_widget_destroy (main_window);

  g_object_unref (engine);

  return 0;
}
