/*
 * bean-activatable.c
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

#include "config.h"

#include "bean-activatable.h"

/**
 * SECTION:bean-activatable
 * @short_description: Interface for activatable plugins.
 * @see_also: #BeanExtensionSet
 *
 * #BeanActivatable is an interface which should be implemented by plugins
 * that should be activated on an object of a certain type (depending on the
 * application). For instance, in a typical windowed application,
 * #BeanActivatable plugin instances could be bound to individual toplevel
 * windows.
 *
 * It is typical to use #BeanActivatable along with #BeanExtensionSet in order
 * to activate and deactivate extensions automatically when plugins are loaded
 * or unloaded.
 *
 * You can also use the code of this interface as a base for your own
 * extension types, as illustrated by gedit's %GeditWindowActivatable and
 * %GeditDocumentActivatable interfaces.
 **/

G_DEFINE_INTERFACE(BeanActivatable, bean_activatable, G_TYPE_OBJECT)

static void
bean_activatable_default_init (BeanActivatableInterface *iface)
{
  /**
   * BeanActivatable:object:
   *
   * The object property contains the targetted object for this
   * #BeanActivatable instance, for example a toplevel window in a typical
   * windowed application. It is set at construction time and won't change.
   */
  g_object_interface_install_property (iface,
                                       g_param_spec_object ("object",
                                                            "Object",
                                                            "Object",
                                                            G_TYPE_OBJECT,
                                                            G_PARAM_READWRITE |
                                                            G_PARAM_CONSTRUCT_ONLY |
                                                            G_PARAM_STATIC_STRINGS));
}

/**
 * bean_activatable_activate:
 * @activatable: A #BeanActivatable.
 *
 * Activates the extension on the targetted object.
 *
 * On activation, the extension should hook itself to the object
 * where it makes sense.
 */
void
bean_activatable_activate (BeanActivatable *activatable)
{
  BeanActivatableInterface *iface;

  g_return_if_fail (BEAN_IS_ACTIVATABLE (activatable));

  iface = BEAN_ACTIVATABLE_GET_IFACE (activatable);
  g_return_if_fail (iface->activate != NULL);

  iface->activate (activatable);
}

/**
 * bean_activatable_deactivate:
 * @activatable: A #BeanActivatable.
 *
 * Deactivates the extension on the targetted object.
 *
 * On deactivation, an extension should remove itself from all the hooks it
 * used and should perform any cleanup required, so it can be unreffed safely
 * and without any more effect on the host application.
 */
void
bean_activatable_deactivate (BeanActivatable *activatable)
{
  BeanActivatableInterface *iface;

  g_return_if_fail (BEAN_IS_ACTIVATABLE (activatable));

  iface = BEAN_ACTIVATABLE_GET_IFACE (activatable);
  g_return_if_fail (iface->deactivate != NULL);

  iface->deactivate (activatable);
}

/**
 * bean_activatable_update_state:
 * @activatable: A #BeanActivatable.
 *
 * Triggers an update of the extension internal state to take into account
 * state changes in the targetted object, due to some event or user action.
 */
void
bean_activatable_update_state (BeanActivatable *activatable)
{
  BeanActivatableInterface *iface;

  g_return_if_fail (BEAN_IS_ACTIVATABLE (activatable));

  iface = BEAN_ACTIVATABLE_GET_IFACE (activatable);
  if (iface->update_state != NULL)
    iface->update_state (activatable);
}

