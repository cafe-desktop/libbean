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

#ifndef __PEAS_PLUGIN_INFO_H__
#define __PEAS_PLUGIN_INFO_H__

#include <gio/gio.h>

#include "bean-version-macros.h"

G_BEGIN_DECLS

#define PEAS_TYPE_PLUGIN_INFO   (bean_plugin_info_get_type ())
#define PEAS_PLUGIN_INFO(obj)   ((BeanPluginInfo *) (obj))

/**
 * PEAS_PLUGIN_INFO_ERROR:
 *
 * Error domain for BeanPluginInfo. Errors in this domain will
 * be from the BeanPluginInfoError enumeration. See #GError for
 * more information on error domains.
 */
#define PEAS_PLUGIN_INFO_ERROR bean_plugin_info_error_quark ()

/**
 * BeanPluginInfoError:
 * @PEAS_PLUGIN_INFO_ERROR_LOADING_FAILED:
 *      The plugin failed to load.
 * @PEAS_PLUGIN_INFO_ERROR_LOADER_NOT_FOUND:
 *      The plugin's loader was not found.
 * @PEAS_PLUGIN_INFO_ERROR_DEP_NOT_FOUND:
 *      A dependency of the plugin was not found.
 * @PEAS_PLUGIN_INFO_ERROR_DEP_LOADING_FAILED:
 *      A dependency of the plugin failed to load.
 *
 * These identify the various errors that can occur while
 * loading a plugin.
 */
typedef enum {
  PEAS_PLUGIN_INFO_ERROR_LOADING_FAILED,
  PEAS_PLUGIN_INFO_ERROR_LOADER_NOT_FOUND,
  PEAS_PLUGIN_INFO_ERROR_DEP_NOT_FOUND,
  PEAS_PLUGIN_INFO_ERROR_DEP_LOADING_FAILED
} BeanPluginInfoError;

/**
 * BeanPluginInfo:
 *
 * The #BeanPluginInfo structure contains only private data and should only
 * be accessed using the provided API.
 */
typedef struct _BeanPluginInfo BeanPluginInfo;

PEAS_AVAILABLE_IN_ALL
GType         bean_plugin_info_get_type         (void) G_GNUC_CONST;
PEAS_AVAILABLE_IN_ALL
GQuark        bean_plugin_info_error_quark      (void);

PEAS_AVAILABLE_IN_ALL
gboolean      bean_plugin_info_is_loaded        (const BeanPluginInfo *info);
PEAS_AVAILABLE_IN_ALL
gboolean      bean_plugin_info_is_available     (const BeanPluginInfo *info,
                                                 GError               **error);
PEAS_AVAILABLE_IN_ALL
gboolean      bean_plugin_info_is_builtin       (const BeanPluginInfo *info);
PEAS_AVAILABLE_IN_ALL
gboolean      bean_plugin_info_is_hidden        (const BeanPluginInfo *info);

PEAS_AVAILABLE_IN_ALL
const gchar  *bean_plugin_info_get_module_name  (const BeanPluginInfo *info);
PEAS_AVAILABLE_IN_ALL
const gchar  *bean_plugin_info_get_module_dir   (const BeanPluginInfo *info);
PEAS_AVAILABLE_IN_ALL
const gchar  *bean_plugin_info_get_data_dir     (const BeanPluginInfo *info);
PEAS_AVAILABLE_IN_ALL
GSettings    *bean_plugin_info_get_settings     (const BeanPluginInfo *info,
                                                 const gchar          *schema_id);
PEAS_AVAILABLE_IN_ALL
const gchar **bean_plugin_info_get_dependencies (const BeanPluginInfo *info);
PEAS_AVAILABLE_IN_ALL
gboolean      bean_plugin_info_has_dependency   (const BeanPluginInfo *info,
                                                 const gchar          *module_name);

PEAS_AVAILABLE_IN_ALL
const gchar  *bean_plugin_info_get_name         (const BeanPluginInfo *info);
PEAS_AVAILABLE_IN_ALL
const gchar  *bean_plugin_info_get_description  (const BeanPluginInfo *info);
PEAS_AVAILABLE_IN_ALL
const gchar  *bean_plugin_info_get_icon_name    (const BeanPluginInfo *info);
PEAS_AVAILABLE_IN_ALL
const gchar **bean_plugin_info_get_authors      (const BeanPluginInfo *info);
PEAS_AVAILABLE_IN_ALL
const gchar  *bean_plugin_info_get_website      (const BeanPluginInfo *info);
PEAS_AVAILABLE_IN_ALL
const gchar  *bean_plugin_info_get_copyright    (const BeanPluginInfo *info);
PEAS_AVAILABLE_IN_ALL
const gchar  *bean_plugin_info_get_version      (const BeanPluginInfo *info);
PEAS_AVAILABLE_IN_ALL
const gchar  *bean_plugin_info_get_help_uri     (const BeanPluginInfo *info);

PEAS_AVAILABLE_IN_ALL
const gchar  *bean_plugin_info_get_external_data (const BeanPluginInfo *info,
                                                  const gchar          *key);

G_END_DECLS

#endif /* __PEAS_PLUGIN_INFO_H__ */
