/*
 * bean-plugins-info.h
 * This file is part of libbean
 *
 * Copyright (C) 2002-2005 - Paolo Maggi
 * Copyright (C) 2007 - Steve Frécinaux
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

#ifndef __BEAN_PLUGIN_INFO_H__
#define __BEAN_PLUGIN_INFO_H__

#include <gio/gio.h>

#include "bean-version-macros.h"

G_BEGIN_DECLS

#define BEAN_TYPE_PLUGIN_INFO   (bean_plugin_info_get_type ())
#define BEAN_PLUGIN_INFO(obj)   ((BeanPluginInfo *) (obj))

/**
 * BEAN_PLUGIN_INFO_ERROR:
 *
 * Error domain for BeanPluginInfo. Errors in this domain will
 * be from the BeanPluginInfoError enumeration. See #GError for
 * more information on error domains.
 */
#define BEAN_PLUGIN_INFO_ERROR bean_plugin_info_error_quark ()

/**
 * BeanPluginInfoError:
 * @BEAN_PLUGIN_INFO_ERROR_LOADING_FAILED:
 *      The plugin failed to load.
 * @BEAN_PLUGIN_INFO_ERROR_LOADER_NOT_FOUND:
 *      The plugin's loader was not found.
 * @BEAN_PLUGIN_INFO_ERROR_DEP_NOT_FOUND:
 *      A dependency of the plugin was not found.
 * @BEAN_PLUGIN_INFO_ERROR_DEP_LOADING_FAILED:
 *      A dependency of the plugin failed to load.
 *
 * These identify the various errors that can occur while
 * loading a plugin.
 */
typedef enum {
  BEAN_PLUGIN_INFO_ERROR_LOADING_FAILED,
  BEAN_PLUGIN_INFO_ERROR_LOADER_NOT_FOUND,
  BEAN_PLUGIN_INFO_ERROR_DEP_NOT_FOUND,
  BEAN_PLUGIN_INFO_ERROR_DEP_LOADING_FAILED
} BeanPluginInfoError;

/**
 * BeanPluginInfo:
 *
 * The #BeanPluginInfo structure contains only private data and should only
 * be accessed using the provided API.
 */
typedef struct _BeanPluginInfo BeanPluginInfo;

BEAN_AVAILABLE_IN_ALL
GType         bean_plugin_info_get_type         (void) G_GNUC_CONST;
BEAN_AVAILABLE_IN_ALL
GQuark        bean_plugin_info_error_quark      (void);

BEAN_AVAILABLE_IN_ALL
gboolean      bean_plugin_info_is_loaded        (const BeanPluginInfo *info);
BEAN_AVAILABLE_IN_ALL
gboolean      bean_plugin_info_is_available     (const BeanPluginInfo *info,
                                                 GError               **error);
BEAN_AVAILABLE_IN_ALL
gboolean      bean_plugin_info_is_builtin       (const BeanPluginInfo *info);
BEAN_AVAILABLE_IN_ALL
gboolean      bean_plugin_info_is_hidden        (const BeanPluginInfo *info);

BEAN_AVAILABLE_IN_ALL
const gchar  *bean_plugin_info_get_module_name  (const BeanPluginInfo *info);
BEAN_AVAILABLE_IN_ALL
const gchar  *bean_plugin_info_get_module_dir   (const BeanPluginInfo *info);
BEAN_AVAILABLE_IN_ALL
const gchar  *bean_plugin_info_get_data_dir     (const BeanPluginInfo *info);
BEAN_AVAILABLE_IN_ALL
GSettings    *bean_plugin_info_get_settings     (const BeanPluginInfo *info,
                                                 const gchar          *schema_id);
BEAN_AVAILABLE_IN_ALL
const gchar **bean_plugin_info_get_dependencies (const BeanPluginInfo *info);
BEAN_AVAILABLE_IN_ALL
gboolean      bean_plugin_info_has_dependency   (const BeanPluginInfo *info,
                                                 const gchar          *module_name);

BEAN_AVAILABLE_IN_ALL
const gchar  *bean_plugin_info_get_name         (const BeanPluginInfo *info);
BEAN_AVAILABLE_IN_ALL
const gchar  *bean_plugin_info_get_description  (const BeanPluginInfo *info);
BEAN_AVAILABLE_IN_ALL
const gchar  *bean_plugin_info_get_icon_name    (const BeanPluginInfo *info);
BEAN_AVAILABLE_IN_ALL
const gchar **bean_plugin_info_get_authors      (const BeanPluginInfo *info);
BEAN_AVAILABLE_IN_ALL
const gchar  *bean_plugin_info_get_website      (const BeanPluginInfo *info);
BEAN_AVAILABLE_IN_ALL
const gchar  *bean_plugin_info_get_copyright    (const BeanPluginInfo *info);
BEAN_AVAILABLE_IN_ALL
const gchar  *bean_plugin_info_get_version      (const BeanPluginInfo *info);
BEAN_AVAILABLE_IN_ALL
const gchar  *bean_plugin_info_get_help_uri     (const BeanPluginInfo *info);

BEAN_AVAILABLE_IN_ALL
const gchar  *bean_plugin_info_get_external_data (const BeanPluginInfo *info,
                                                  const gchar          *key);

G_END_DECLS

#endif /* __BEAN_PLUGIN_INFO_H__ */
