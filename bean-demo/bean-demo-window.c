/*
 * bean-demo-window.c
 * This file is part of libbean
 *
 * Copyright (C) 2010 Steve Frécinaux
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

#include "bean-demo-window.h"

G_DEFINE_TYPE (DemoWindow, demo_window, GTK_TYPE_WINDOW)

static void
on_extension_added (BeanExtensionSet *set,
                    BeanPluginInfo   *info,
                    BeanExtension    *exten,
                    DemoWindow       *dw)
{
  bean_activatable_activate (BEAN_ACTIVATABLE (exten));
}

static void
on_extension_removed (BeanExtensionSet *set,
                      BeanPluginInfo   *info,
                      BeanExtension    *exten,
                      DemoWindow       *dw)
{
  bean_activatable_deactivate (BEAN_ACTIVATABLE (exten));
}

static void
demo_window_init (DemoWindow *dw)
{
  DemoWindowClass *klass = DEMO_WINDOW_GET_CLASS (dw);
  gchar *label;

  dw->box = ctk_box_new (GTK_ORIENTATION_VERTICAL, 6);
  ctk_box_set_homogeneous (GTK_BOX (dw->box), TRUE);
  ctk_container_add (GTK_CONTAINER (dw), dw->box);

  label = g_strdup_printf ("Bean Window %d", ++(klass->n_windows));
  ctk_window_set_title (GTK_WINDOW (dw), label);
  g_free (label);

  dw->exten_set = bean_extension_set_new (bean_engine_get_default (),
                                          BEAN_TYPE_ACTIVATABLE,
                                          "object", dw,
                                          NULL);

  bean_extension_set_foreach (dw->exten_set,
                              (BeanExtensionSetForeachFunc) on_extension_added,
                              dw);

  g_signal_connect (dw->exten_set, "extension-added", G_CALLBACK (on_extension_added), dw);
  g_signal_connect (dw->exten_set, "extension-removed", G_CALLBACK (on_extension_removed), dw);
}

static void
demo_window_dispose (GObject *object)
{
  DemoWindow *dw = DEMO_WINDOW (object);

  g_clear_object (&dw->exten_set);

  G_OBJECT_CLASS (demo_window_parent_class)->dispose (object);
}

static void
demo_window_class_init (DemoWindowClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->dispose = demo_window_dispose;

  klass->n_windows = 0;
}

CtkWidget *
demo_window_new (void)
{
  return GTK_WIDGET (g_object_new (DEMO_TYPE_WINDOW, NULL));
}
