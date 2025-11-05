/*
 * bean-extension.c
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

#include "bean-extension.h"

/**
 * SECTION:bean-extension
 * @short_description: Proxy for extensions.
 * @see_also: #BeanExtensionSet
 *
 * #BeanExtension is a proxy class used to access actual extensions
 * implemented using various languages.  As such, the application writer will
 * use #BeanExtension instances to call methods on extension provided by
 * loaded plugins.
 *
 * To properly use the proxy instances, you will need GObject-introspection
 * data for the #GType you want to use as an extension point.
 * For instance, if you wish to use #BeanActivatable, you will need to
 * put the following code excerpt in the engine initialization code, in order
 * to load the required "Bean" typelib:
 *
 * |[
 * g_irepository_require (g_irepository_get_default (),
 *                        "Bean", "2.0", 0, NULL);
 * ]|
 *
 * You should proceed the same way for any namespace which provides types
 * you want to use as extension points. GObject-introspection data is required
 * for all the supported languages, even for C.
 *
 * #BeanExtension does not provide any way to access the underlying object.
 * The main reason is that some loaders may not rely on proper GObject
 * inheritance for the definition of extensions, and hence it would not be
 * possible for libbean to provide a functional GObject instance at all.
 * Another reason is that it makes reference counting issues easier to deal
 * with.
 *
 * See bean_extension_call() for more information.
 **/
