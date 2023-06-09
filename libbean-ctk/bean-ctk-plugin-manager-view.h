/*
 * bean-plugin-manager-view.h
 * This file is part of libbean
 *
 * Copyright (C) 2002 Paolo Maggi and James Willcox
 * Copyright (C) 2003-2006 Paolo Maggi, Paolo Borelli
 * Copyright (C) 2007-2009 Paolo Maggi, Paolo Borelli, Steve Frécinaux
 * Copyright (C) 2010 Garrett Regier
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

#ifndef __BEAN_CTK_PLUGIN_MANAGER_VIEW_H__
#define __BEAN_CTK_PLUGIN_MANAGER_VIEW_H__

#include <ctk/ctk.h>
#include <libbean/bean-engine.h>

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define BEAN_CTK_TYPE_PLUGIN_MANAGER_VIEW             (bean_ctk_plugin_manager_view_get_type())
#define BEAN_CTK_PLUGIN_MANAGER_VIEW(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), BEAN_CTK_TYPE_PLUGIN_MANAGER_VIEW, BeanCtkPluginManagerView))
#define BEAN_CTK_PLUGIN_MANAGER_VIEW_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), BEAN_CTK_TYPE_PLUGIN_MANAGER_VIEW, BeanCtkPluginManagerViewClass))
#define BEAN_CTK_IS_PLUGIN_MANAGER_VIEW(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), BEAN_CTK_TYPE_PLUGIN_MANAGER_VIEW))
#define BEAN_CTK_IS_PLUGIN_MANAGER_VIEW_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), BEAN_CTK_TYPE_PLUGIN_MANAGER_VIEW))
#define BEAN_CTK_PLUGIN_MANAGER_VIEW_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), BEAN_CTK_TYPE_PLUGIN_MANAGER_VIEW, BeanCtkPluginManagerViewClass))

typedef struct _BeanCtkPluginManagerView        BeanCtkPluginManagerView;
typedef struct _BeanCtkPluginManagerViewClass   BeanCtkPluginManagerViewClass;
typedef struct _BeanCtkPluginManagerViewPrivate BeanCtkPluginManagerViewPrivate;

/**
 * BeanCtkPluginManagerView:
 *
 * The #BeanCtkPluginManagerView structure contains only private data
 * and should only be accessed using the provided API.
 */
struct _BeanCtkPluginManagerView {
  CtkTreeView parent;

  /*< private > */
  BeanCtkPluginManagerViewPrivate *priv;
};

/**
 * BeanCtkPluginManagerViewClass:
 * @parent_class: The parent class.
 * @populate_popup: Signal class handler for the
 *                  #BeanCtkPluginManagerView::populate-popup signal.
 *
 * The class structure for #BeanCtkPluginManagerView.
 */
struct _BeanCtkPluginManagerViewClass {
  CtkTreeViewClass parent_class;

  void  (*populate_popup)   (BeanCtkPluginManagerView *view,
                             CtkMenu                  *menu);

  /*< private >*/
  gpointer padding[8];
};

BEAN_AVAILABLE_IN_ALL
GType           bean_ctk_plugin_manager_view_get_type             (void) G_GNUC_CONST;
BEAN_AVAILABLE_IN_ALL
CtkWidget      *bean_ctk_plugin_manager_view_new                  (BeanEngine               *engine);

#ifndef BEAN_DISABLE_DEPRECATED
BEAN_AVAILABLE_IN_ALL
void            bean_ctk_plugin_manager_view_set_show_builtin     (BeanCtkPluginManagerView *view,
                                                                   gboolean                  show_builtin);
BEAN_AVAILABLE_IN_ALL
gboolean        bean_ctk_plugin_manager_view_get_show_builtin     (BeanCtkPluginManagerView *view);
#endif

BEAN_AVAILABLE_IN_ALL
void            bean_ctk_plugin_manager_view_set_selected_plugin  (BeanCtkPluginManagerView *view,
                                                                   BeanPluginInfo           *info);
BEAN_AVAILABLE_IN_ALL
BeanPluginInfo *bean_ctk_plugin_manager_view_get_selected_plugin  (BeanCtkPluginManagerView *view);

G_END_DECLS

#endif /* __BEAN_CTK_PLUGIN_MANAGER_VIEW_H__  */
