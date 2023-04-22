/*
 * bean-version-macros.h
 * This file is part of libbean
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

#ifndef __BEAN_VERSION_MACROS_H__
#define __BEAN_VERSION_MACROS_H__

#include <glib.h>

#include "bean-version.h"

#ifndef _BEAN_EXTERN
# define _BEAN_EXTERN extern
#endif

#ifdef BEAN_DISABLE_DEPRECATION_WARNINGS
# define BEAN_DEPRECATED _BEAN_EXTERN
# define BEAN_DEPRECATED_FOR(f) _BEAN_EXTERN
# define BEAN_UNAVAILABLE(maj,min) _BEAN_EXTERN
#else
# define BEAN_DEPRECATED G_DEPRECATED _BEAN_EXTERN
# define BEAN_DEPRECATED_FOR(f) G_DEPRECATED_FOR(f) _BEAN_EXTERN
# define BEAN_UNAVAILABLE(maj,min) G_UNAVAILABLE(maj,min) _BEAN_EXTERN
#endif

#define BEAN_VERSION_1_22 (G_ENCODE_VERSION (1, 22))
#define BEAN_VERSION_1_24 (G_ENCODE_VERSION (1, 24))

#if (BEAN_MINOR_VERSION == 99)
# define BEAN_VERSION_CUR_STABLE (G_ENCODE_VERSION (BEAN_MAJOR_VERSION + 1, 0))
#elif (BEAN_MINOR_VERSION % 2)
# define BEAN_VERSION_CUR_STABLE (G_ENCODE_VERSION (BEAN_MAJOR_VERSION, BEAN_MINOR_VERSION + 1))
#else
# define BEAN_VERSION_CUR_STABLE (G_ENCODE_VERSION (BEAN_MAJOR_VERSION, BEAN_MINOR_VERSION))
#endif

#if (BEAN_MINOR_VERSION == 99)
# define BEAN_VERSION_PREV_STABLE (G_ENCODE_VERSION (BEAN_MAJOR_VERSION + 1, 0))
#elif (BEAN_MINOR_VERSION % 2)
# define BEAN_VERSION_PREV_STABLE (G_ENCODE_VERSION (BEAN_MAJOR_VERSION, BEAN_MINOR_VERSION - 1))
#else
# define BEAN_VERSION_PREV_STABLE (G_ENCODE_VERSION (BEAN_MAJOR_VERSION, BEAN_MINOR_VERSION - 2))
#endif

/**
 * BEAN_VERSION_MIN_REQUIRED:
 *
 * A macro that should be defined by the user prior to including
 * the bean.h header.
 *
 * The definition should be one of the predefined BEAN version
 * macros: %BEAN_VERSION_1_22, ...
 *
 * This macro defines the lower bound for the Bean API to use.
 *
 * If a function has been deprecated in a newer version of Bean,
 * it is possible to use this symbol to avoid the compiler warnings
 * without disabling warning for every deprecated function.
 *
 * Since: 1.24
 */
#ifndef BEAN_VERSION_MIN_REQUIRED
# define BEAN_VERSION_MIN_REQUIRED (BEAN_VERSION_CUR_STABLE)
#endif

/**
 * BEAN_VERSION_MAX_ALLOWED:
 *
 * A macro that should be defined by the user prior to including
 * the bean.h header.

 * The definition should be one of the predefined Bean version
 * macros: %BEAN_VERSION_1_0, %BEAN_VERSION_1_2,...
 *
 * This macro defines the upper bound for the BEAN API to use.
 *
 * If a function has been introduced in a newer version of Bean,
 * it is possible to use this symbol to get compiler warnings when
 * trying to use that function.
 *
 * Since: 1.24
 */
#ifndef BEAN_VERSION_MAX_ALLOWED
# if BEAN_VERSION_MIN_REQUIRED > BEAN_VERSION_PREV_STABLE
#  define BEAN_VERSION_MAX_ALLOWED (BEAN_VERSION_MIN_REQUIRED)
# else
#  define BEAN_VERSION_MAX_ALLOWED (BEAN_VERSION_CUR_STABLE)
# endif
#endif

#if BEAN_VERSION_MAX_ALLOWED < BEAN_VERSION_MIN_REQUIRED
#error "BEAN_VERSION_MAX_ALLOWED must be >= BEAN_VERSION_MIN_REQUIRED"
#endif
#if BEAN_VERSION_MIN_REQUIRED < BEAN_VERSION_1_22
#error "BEAN_VERSION_MIN_REQUIRED must be >= BEAN_VERSION_1_22"
#endif

#define BEAN_AVAILABLE_IN_ALL                   _BEAN_EXTERN

#if BEAN_VERSION_MIN_REQUIRED >= BEAN_VERSION_1_22
# define BEAN_DEPRECATED_IN_1_22                BEAN_DEPRECATED
# define BEAN_DEPRECATED_IN_1_22_FOR(f)         BEAN_DEPRECATED_FOR(f)
#else
# define BEAN_DEPRECATED_IN_1_22                _BEAN_EXTERN
# define BEAN_DEPRECATED_IN_1_22_FOR(f)         _BEAN_EXTERN
#endif

#if BEAN_VERSION_MAX_ALLOWED < BEAN_VERSION_1_22
# define BEAN_AVAILABLE_IN_1_22                 BEAN_UNAVAILABLE(1, 22)
#else
# define BEAN_AVAILABLE_IN_1_22                 _BEAN_EXTERN
#endif

#if BEAN_VERSION_MIN_REQUIRED >= BEAN_VERSION_1_24
# define BEAN_DEPRECATED_IN_1_24                BEAN_DEPRECATED
# define BEAN_DEPRECATED_IN_1_24_FOR(f)         BEAN_DEPRECATED_FOR(f)
#else
# define BEAN_DEPRECATED_IN_1_24                _BEAN_EXTERN
# define BEAN_DEPRECATED_IN_1_24_FOR(f)         _BEAN_EXTERN
#endif

#if BEAN_VERSION_MAX_ALLOWED < BEAN_VERSION_1_24
# define BEAN_AVAILABLE_IN_1_24                 BEAN_UNAVAILABLE(1, 24)
#else
# define BEAN_AVAILABLE_IN_1_24                 _BEAN_EXTERN
#endif

#endif /* __BEAN_VERSION_MACROS_H__ */
