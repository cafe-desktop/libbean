/*
 * bean-activatable.h
 * This file is part of libbean
 *
 * Copyright (C) 2010 - Steve Frécinaux
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

#ifndef __BEAN_ACTIVATABLE_H__
#define __BEAN_ACTIVATABLE_H__

#include <glib-object.h>

#include "bean-version-macros.h"

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define BEAN_TYPE_ACTIVATABLE             (bean_activatable_get_type ())
#define BEAN_ACTIVATABLE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), BEAN_TYPE_ACTIVATABLE, BeanActivatable))
#define BEAN_ACTIVATABLE_IFACE(obj)       (G_TYPE_CHECK_CLASS_CAST ((obj), BEAN_TYPE_ACTIVATABLE, BeanActivatableInterface))
#define BEAN_IS_ACTIVATABLE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BEAN_TYPE_ACTIVATABLE))
#define BEAN_ACTIVATABLE_GET_IFACE(obj)   (G_TYPE_INSTANCE_GET_INTERFACE ((obj), BEAN_TYPE_ACTIVATABLE, BeanActivatableInterface))

/**
 * BeanActivatable:
 *
 * Interface for activatable plugins.
 */
typedef struct _BeanActivatable           BeanActivatable; /* dummy typedef */
typedef struct _BeanActivatableInterface  BeanActivatableInterface;

/**
 * BeanActivatableInterface:
 * @g_iface: The parent interface.
 * @activate: Activates the plugin.
 * @deactivate: Deactivates the plugin.
 * @update_state: Updates the plugin's internal state to take account of
 *  a change in the target object's state.
 *
 * Provides an interface for activatable plugins.
 */
struct _BeanActivatableInterface {
  GTypeInterface g_iface;

  /* Virtual public methods */
  void        (*activate)                 (BeanActivatable *activatable);
  void        (*deactivate)               (BeanActivatable *activatable);
  void        (*update_state)             (BeanActivatable *activatable);
};

/*
 * Public methods
 */
BEAN_AVAILABLE_IN_ALL
GType             bean_activatable_get_type       (void)  G_GNUC_CONST;

BEAN_AVAILABLE_IN_ALL
void              bean_activatable_activate       (BeanActivatable *activatable);
BEAN_AVAILABLE_IN_ALL
void              bean_activatable_deactivate     (BeanActivatable *activatable);
BEAN_AVAILABLE_IN_ALL
void              bean_activatable_update_state   (BeanActivatable *activatable);

G_END_DECLS

#endif /* __BEAN_ACTIVATABLE_H__ */
