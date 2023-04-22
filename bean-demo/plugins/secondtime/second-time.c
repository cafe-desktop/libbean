/*
 * second-time.c
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

#include <glib.h>
#include <glib-object.h>
#include <gmodule.h>
#include <gtk/gtk.h>

#include <libbean/bean.h>

#include "second-time.h"

static void bean_activatable_iface_init     (BeanActivatableInterface    *iface);

G_DEFINE_DYNAMIC_TYPE_EXTENDED (BeanDemoSecondTime,
                                beandemo_second_time,
                                PEAS_TYPE_EXTENSION_BASE,
                                0,
                                G_IMPLEMENT_INTERFACE_DYNAMIC (PEAS_TYPE_ACTIVATABLE,
                                                               bean_activatable_iface_init))

enum {
  PROP_0,
  PROP_OBJECT
};

static void
beandemo_second_time_set_property (GObject      *object,
                                   guint         prop_id,
                                   const GValue *value,
                                   GParamSpec   *pspec)
{
  BeanDemoSecondTime *plugin = PEASDEMO_SECOND_TIME (object);

  switch (prop_id)
    {
    case PROP_OBJECT:
      plugin->window = GTK_WIDGET (g_value_dup_object (value));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
beandemo_second_time_get_property (GObject    *object,
                                   guint       prop_id,
                                   GValue     *value,
                                   GParamSpec *pspec)
{
  BeanDemoSecondTime *plugin = PEASDEMO_SECOND_TIME (object);

  switch (prop_id)
    {
    case PROP_OBJECT:
      g_value_set_object (value, plugin->window);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}


static void
beandemo_second_time_init (BeanDemoSecondTime *plugin)
{
  g_debug ("%s", G_STRFUNC);
}

static void
beandemo_second_time_finalize (GObject *object)
{
  BeanDemoSecondTime *plugin = PEASDEMO_SECOND_TIME (object);

  g_debug ("%s", G_STRFUNC);

  g_object_unref (plugin->label);
  g_object_unref (plugin->window);

  G_OBJECT_CLASS (beandemo_second_time_parent_class)->finalize (object);
}

static GtkBox *
get_box (GtkWidget *window)
{
  return GTK_BOX (gtk_bin_get_child (GTK_BIN (window)));
}

static void
beandemo_second_time_activate (BeanActivatable *activatable)
{
  BeanDemoSecondTime *plugin = PEASDEMO_SECOND_TIME (activatable);

  g_debug ("%s", G_STRFUNC);

  plugin->label = gtk_label_new ("A second time!");
  gtk_box_pack_start (get_box (plugin->window), plugin->label, 1, 1, 0);
  gtk_widget_show (plugin->label);
  g_object_ref (plugin->label);
}

static void
beandemo_second_time_deactivate (BeanActivatable *activatable)
{
  BeanDemoSecondTime *plugin = PEASDEMO_SECOND_TIME (activatable);

  g_debug ("%s", G_STRFUNC);

  gtk_container_remove (GTK_CONTAINER (get_box (plugin->window)), plugin->label);
}

static void
beandemo_second_time_class_init (BeanDemoSecondTimeClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = beandemo_second_time_set_property;
  object_class->get_property = beandemo_second_time_get_property;
  object_class->finalize = beandemo_second_time_finalize;

  g_object_class_override_property (object_class, PROP_OBJECT, "object");
}

static void
bean_activatable_iface_init (BeanActivatableInterface *iface)
{
  iface->activate = beandemo_second_time_activate;
  iface->deactivate = beandemo_second_time_deactivate;
}

static void
beandemo_second_time_class_finalize (BeanDemoSecondTimeClass *klass)
{
}

G_MODULE_EXPORT void
bean_register_types (BeanObjectModule *module)
{
  beandemo_second_time_register_type (G_TYPE_MODULE (module));

  bean_object_module_register_extension_type (module,
                                              PEAS_TYPE_ACTIVATABLE,
                                              PEASDEMO_TYPE_SECOND_TIME);
}
