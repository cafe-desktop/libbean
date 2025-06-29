/*
 * introspection-callable.h
 * This file is part of libbean
 *
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "introspection-callable.h"

G_DEFINE_INTERFACE(IntrospectionCallable, introspection_callable, G_TYPE_OBJECT)

void
introspection_callable_default_init (IntrospectionCallableInterface *iface G_GNUC_UNUSED)
{
}

/**
 * introspection_callable_call_with_return:
 * @callable:
 *
 * Returns: (transfer full):
 */
gchar *
introspection_callable_call_with_return (IntrospectionCallable *callable)
{
  IntrospectionCallableInterface *iface;

  g_return_val_if_fail (INTROSPECTION_IS_CALLABLE (callable), NULL);

  iface = INTROSPECTION_CALLABLE_GET_IFACE (callable);
  g_assert (iface->call_with_return != NULL);

  return iface->call_with_return (callable);
}

/**
 * introspection_callable_call_no_args:
 * @callable:
 */
void
introspection_callable_call_no_args (IntrospectionCallable *callable)
{
  IntrospectionCallableInterface *iface;

  g_return_if_fail (INTROSPECTION_IS_CALLABLE (callable));

  iface = INTROSPECTION_CALLABLE_GET_IFACE (callable);
  g_assert (iface->call_no_args != NULL);

  iface->call_no_args (callable);
}

/**
 * introspection_callable_call_single_arg:
 * @callable:
 * @called: (out):
 */
void
introspection_callable_call_single_arg (IntrospectionCallable *callable,
                                        gboolean              *called)
{
  IntrospectionCallableInterface *iface;

  g_return_if_fail (INTROSPECTION_IS_CALLABLE (callable));

  iface = INTROSPECTION_CALLABLE_GET_IFACE (callable);
  g_assert (iface->call_single_arg != NULL);

  iface->call_single_arg (callable, called);
}

/**
 * introspection_callable_call_multi_args:
 * @callable:
 * @in: (in):
 * @out: (out):
 * @inout: (inout):
 */
void
introspection_callable_call_multi_args (IntrospectionCallable *callable,
                                        gint                   in,
                                        gint                  *out,
                                        gint                  *inout)
{
  IntrospectionCallableInterface *iface;

  g_return_if_fail (INTROSPECTION_IS_CALLABLE (callable));

  iface = INTROSPECTION_CALLABLE_GET_IFACE (callable);
  g_assert (iface->call_multi_args != NULL);

  iface->call_multi_args (callable, in, out, inout);
}
