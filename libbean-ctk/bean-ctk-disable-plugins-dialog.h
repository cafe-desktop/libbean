/*
 * bean-gtk-disable-plugins-dialog.h
 * This file is part of libbean
 *
 * Copyright (C) 2011 Garrett Regier
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

#ifndef __BEAN_GTK_DISABLE_PLUGINS_DIALOG_H__
#define __BEAN_GTK_DISABLE_PLUGINS_DIALOG_H__

#include <gtk/gtk.h>
#include <libbean/bean-plugin-info.h>

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define BEAN_GTK_TYPE_DISABLE_PLUGINS_DIALOG            (bean_gtk_disable_plugins_dialog_get_type())
#define BEAN_GTK_DISABLE_PLUGINS_DIALOG(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), BEAN_GTK_TYPE_DISABLE_PLUGINS_DIALOG, BeanGtkDisablePluginsDialog))
#define BEAN_GTK_DISABLE_PLUGINS_DIALOG_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), BEAN_GTK_TYPE_DISABLE_PLUGINS_DIALOG, BeanGtkDisablePluginsDialogClass))
#define BEAN_GTK_IS_DISABLE_PLUGINS_DIALOG(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), BEAN_GTK_TYPE_DISABLE_PLUGINS_DIALOG))
#define BEAN_GTK_IS_DISABLE_PLUGINS_DIALOG_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), BEAN_GTK_TYPE_DISABLE_PLUGINS_DIALOG))
#define BEAN_GTK_DISABLE_PLUGINS_DIALOG_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), BEAN_GTK_TYPE_DISABLE_PLUGINS_DIALOG, BeanGtkDisablePluginsDialogClass))

typedef struct _BeanGtkDisablePluginsDialog         BeanGtkDisablePluginsDialog;
typedef struct _BeanGtkDisablePluginsDialogClass    BeanGtkDisablePluginsDialogClass;

struct _BeanGtkDisablePluginsDialog {
  GtkMessageDialog parent;
};

struct _BeanGtkDisablePluginsDialogClass {
  GtkMessageDialogClass parent_class;
};

GType      bean_gtk_disable_plugins_dialog_get_type (void) G_GNUC_CONST;

GtkWidget *bean_gtk_disable_plugins_dialog_new      (GtkWindow      *parent,
                                                     BeanPluginInfo *info,
                                                     GList          *dep_plugins);

G_END_DECLS

#endif /* __BEAN_GTK_DISABLE_PLUGINS_DIALOG_H__  */