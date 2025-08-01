/* GObject - GLib Type, Object, Parameter and Signal Library
 * Copyright (C) 1997-1999, 2000-2001 Tim Janik and Red Hat, Inc.
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, see <http://www.gnu.org/licenses/>.
 *
 * cparamspecs.h: GLib default param specs
 */


#include <glib-object.h>

#include "cparam.h"


G_BEGIN_DECLS

/* --- type macros --- */
/**
 * C_TYPE_PARAM_CHAR:
 * 
 * The #GType of #CParamSpecChar.
 */
#define	C_TYPE_PARAM_CHAR		   (c_param_spec_types[0])
/**
 * C_IS_PARAM_SPEC_CHAR:
 * @pspec: a valid #CParamSpec instance
 * 
 * Checks whether the given #CParamSpec is of type %C_TYPE_PARAM_CHAR.
 * 
 * Returns: %TRUE on success.
 */
#define C_IS_PARAM_SPEC_CHAR(pspec)        (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), C_TYPE_PARAM_CHAR))
/**
 * C_PARAM_SPEC_CHAR:
 * @pspec: a valid #CParamSpec instance
 * 
 * Cast a #CParamSpec instance into a #CParamSpecChar.
 */
#define C_PARAM_SPEC_CHAR(pspec)           (G_TYPE_CHECK_INSTANCE_CAST ((pspec), C_TYPE_PARAM_CHAR, CParamSpecChar))

/**
 * C_TYPE_PARAM_UCHAR:
 * 
 * The #GType of #CParamSpecUChar.
 */
#define	C_TYPE_PARAM_UCHAR		   (c_param_spec_types[1])
/**
 * C_IS_PARAM_SPEC_UCHAR:
 * @pspec: a valid #CParamSpec instance
 * 
 * Checks whether the given #CParamSpec is of type %C_TYPE_PARAM_UCHAR.
 * 
 * Returns: %TRUE on success.
 */
#define C_IS_PARAM_SPEC_UCHAR(pspec)       (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), C_TYPE_PARAM_UCHAR))
/**
 * C_PARAM_SPEC_UCHAR:
 * @pspec: a valid #CParamSpec instance
 * 
 * Cast a #CParamSpec instance into a #CParamSpecUChar.
 */
#define C_PARAM_SPEC_UCHAR(pspec)          (G_TYPE_CHECK_INSTANCE_CAST ((pspec), C_TYPE_PARAM_UCHAR, CParamSpecUChar))

/**
 * C_TYPE_PARAM_BOOLEAN:
 * 
 * The #GType of #CParamSpecBoolean.
 */
#define	C_TYPE_PARAM_BOOLEAN		   (c_param_spec_types[2])
/**
 * C_IS_PARAM_SPEC_BOOLEAN:
 * @pspec: a valid #CParamSpec instance
 * 
 * Checks whether the given #CParamSpec is of type %C_TYPE_PARAM_BOOLEAN.
 * 
 * Returns: %TRUE on success.
 */
#define C_IS_PARAM_SPEC_BOOLEAN(pspec)     (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), C_TYPE_PARAM_BOOLEAN))
/**
 * C_PARAM_SPEC_BOOLEAN:
 * @pspec: a valid #CParamSpec instance
 * 
 * Cast a #CParamSpec instance into a #CParamSpecBoolean.
 */
#define C_PARAM_SPEC_BOOLEAN(pspec)        (G_TYPE_CHECK_INSTANCE_CAST ((pspec), C_TYPE_PARAM_BOOLEAN, CParamSpecBoolean))

/**
 * C_TYPE_PARAM_INT:
 * 
 * The #GType of #CParamSpecInt.
 */
#define	C_TYPE_PARAM_INT		   (c_param_spec_types[3])
/**
 * C_IS_PARAM_SPEC_INT:
 * @pspec: a valid #CParamSpec instance
 * 
 * Checks whether the given #CParamSpec is of type %C_TYPE_PARAM_INT.
 * 
 * Returns: %TRUE on success.
 */
#define C_IS_PARAM_SPEC_INT(pspec)         (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), C_TYPE_PARAM_INT))
/**
 * C_PARAM_SPEC_INT:
 * @pspec: a valid #CParamSpec instance
 * 
 * Cast a #CParamSpec instance into a #CParamSpecInt.
 */
#define C_PARAM_SPEC_INT(pspec)            (G_TYPE_CHECK_INSTANCE_CAST ((pspec), C_TYPE_PARAM_INT, CParamSpecInt))

/**
 * C_TYPE_PARAM_UINT:
 * 
 * The #GType of #CParamSpecUInt.
 */
#define	C_TYPE_PARAM_UINT		   (c_param_spec_types[4])
/**
 * C_IS_PARAM_SPEC_UINT:
 * @pspec: a valid #CParamSpec instance
 * 
 * Checks whether the given #CParamSpec is of type %C_TYPE_PARAM_UINT.
 * 
 * Returns: %TRUE on success.
 */
#define C_IS_PARAM_SPEC_UINT(pspec)        (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), C_TYPE_PARAM_UINT))
/**
 * C_PARAM_SPEC_UINT:
 * @pspec: a valid #CParamSpec instance
 * 
 * Cast a #CParamSpec instance into a #CParamSpecUInt.
 */
#define C_PARAM_SPEC_UINT(pspec)           (G_TYPE_CHECK_INSTANCE_CAST ((pspec), C_TYPE_PARAM_UINT, CParamSpecUInt))

/**
 * C_TYPE_PARAM_LONG:
 * 
 * The #GType of #CParamSpecLong.
 */
#define	C_TYPE_PARAM_LONG		   (c_param_spec_types[5])
/**
 * C_IS_PARAM_SPEC_LONG:
 * @pspec: a valid #CParamSpec instance
 * 
 * Checks whether the given #CParamSpec is of type %C_TYPE_PARAM_LONG.
 * 
 * Returns: %TRUE on success.
 */
#define C_IS_PARAM_SPEC_LONG(pspec)        (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), C_TYPE_PARAM_LONG))
/**
 * C_PARAM_SPEC_LONG:
 * @pspec: a valid #CParamSpec instance
 * 
 * Cast a #CParamSpec instance into a #CParamSpecLong.
 */
#define C_PARAM_SPEC_LONG(pspec)           (G_TYPE_CHECK_INSTANCE_CAST ((pspec), C_TYPE_PARAM_LONG, CParamSpecLong))

/**
 * C_TYPE_PARAM_ULONG:
 * 
 * The #GType of #CParamSpecULong.
 */
#define	C_TYPE_PARAM_ULONG		   (c_param_spec_types[6])
/**
 * C_IS_PARAM_SPEC_ULONG:
 * @pspec: a valid #CParamSpec instance
 * 
 * Checks whether the given #CParamSpec is of type %C_TYPE_PARAM_ULONG.
 * 
 * Returns: %TRUE on success.
 */
#define C_IS_PARAM_SPEC_ULONG(pspec)       (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), C_TYPE_PARAM_ULONG))
/**
 * C_PARAM_SPEC_ULONG:
 * @pspec: a valid #CParamSpec instance
 * 
 * Cast a #CParamSpec instance into a #CParamSpecULong.
 */
#define C_PARAM_SPEC_ULONG(pspec)          (G_TYPE_CHECK_INSTANCE_CAST ((pspec), C_TYPE_PARAM_ULONG, CParamSpecULong))

/**
 * C_TYPE_PARAM_INT64:
 * 
 * The #GType of #CParamSpecInt64.
 */
#define	C_TYPE_PARAM_INT64		   (c_param_spec_types[7])
/**
 * C_IS_PARAM_SPEC_INT64:
 * @pspec: a valid #CParamSpec instance
 * 
 * Checks whether the given #CParamSpec is of type %C_TYPE_PARAM_INT64.
 *
 * Returns: %TRUE on success.
 */
#define C_IS_PARAM_SPEC_INT64(pspec)       (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), C_TYPE_PARAM_INT64))
/**
 * C_PARAM_SPEC_INT64:
 * @pspec: a valid #CParamSpec instance
 * 
 * Cast a #CParamSpec instance into a #CParamSpecInt64.
 */
#define C_PARAM_SPEC_INT64(pspec)          (G_TYPE_CHECK_INSTANCE_CAST ((pspec), C_TYPE_PARAM_INT64, CParamSpecInt64))

/**
 * C_TYPE_PARAM_UINT64:
 * 
 * The #GType of #CParamSpecUInt64.
 */
#define	C_TYPE_PARAM_UINT64		   (c_param_spec_types[8])
/**
 * C_IS_PARAM_SPEC_UINT64:
 * @pspec: a valid #CParamSpec instance
 * 
 * Checks whether the given #CParamSpec is of type %C_TYPE_PARAM_UINT64.
 * 
 * Returns: %TRUE on success.
 */
#define C_IS_PARAM_SPEC_UINT64(pspec)      (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), C_TYPE_PARAM_UINT64))
/**
 * C_PARAM_SPEC_UINT64:
 * @pspec: a valid #CParamSpec instance
 * 
 * Cast a #CParamSpec instance into a #CParamSpecUInt64.
 */
#define C_PARAM_SPEC_UINT64(pspec)         (G_TYPE_CHECK_INSTANCE_CAST ((pspec), C_TYPE_PARAM_UINT64, CParamSpecUInt64))

/**
 * C_TYPE_PARAM_UNICHAR:
 * 
 * The #GType of #CParamSpecUnichar.
 */
#define	C_TYPE_PARAM_UNICHAR		   (c_param_spec_types[9])
/**
 * C_PARAM_SPEC_UNICHAR:
 * @pspec: a valid #CParamSpec instance
 * 
 * Cast a #CParamSpec instance into a #CParamSpecUnichar.
 */
#define C_PARAM_SPEC_UNICHAR(pspec)        (G_TYPE_CHECK_INSTANCE_CAST ((pspec), C_TYPE_PARAM_UNICHAR, CParamSpecUnichar))
/**
 * C_IS_PARAM_SPEC_UNICHAR:
 * @pspec: a valid #CParamSpec instance
 * 
 * Checks whether the given #CParamSpec is of type %C_TYPE_PARAM_UNICHAR.
 * 
 * Returns: %TRUE on success.
 */
#define C_IS_PARAM_SPEC_UNICHAR(pspec)     (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), C_TYPE_PARAM_UNICHAR))

/**
 * C_TYPE_PARAM_ENUM:
 * 
 * The #GType of #CParamSpecEnum.
 */
#define	C_TYPE_PARAM_ENUM		   (c_param_spec_types[10])
/**
 * C_IS_PARAM_SPEC_ENUM:
 * @pspec: a valid #CParamSpec instance
 * 
 * Checks whether the given #CParamSpec is of type %C_TYPE_PARAM_ENUM.
 * 
 * Returns: %TRUE on success.
 */
#define C_IS_PARAM_SPEC_ENUM(pspec)        (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), C_TYPE_PARAM_ENUM))
/**
 * C_PARAM_SPEC_ENUM:
 * @pspec: a valid #CParamSpec instance
 * 
 * Cast a #CParamSpec instance into a #CParamSpecEnum.
 */
#define C_PARAM_SPEC_ENUM(pspec)           (G_TYPE_CHECK_INSTANCE_CAST ((pspec), C_TYPE_PARAM_ENUM, CParamSpecEnum))

/**
 * C_TYPE_PARAM_FLAGS:
 * 
 * The #GType of #CParamSpecFlags.
 */
#define	C_TYPE_PARAM_FLAGS		   (c_param_spec_types[11])
/**
 * C_IS_PARAM_SPEC_FLAGS:
 * @pspec: a valid #CParamSpec instance
 * 
 * Checks whether the given #CParamSpec is of type %C_TYPE_PARAM_FLAGS.
 * 
 * Returns: %TRUE on success.
 */
#define C_IS_PARAM_SPEC_FLAGS(pspec)       (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), C_TYPE_PARAM_FLAGS))
/**
 * C_PARAM_SPEC_FLAGS:
 * @pspec: a valid #CParamSpec instance
 * 
 * Cast a #CParamSpec instance into a #CParamSpecFlags.
 */
#define C_PARAM_SPEC_FLAGS(pspec)          (G_TYPE_CHECK_INSTANCE_CAST ((pspec), C_TYPE_PARAM_FLAGS, CParamSpecFlags))

/**
 * C_TYPE_PARAM_FLOAT:
 * 
 * The #GType of #CParamSpecFloat.
 */
#define	C_TYPE_PARAM_FLOAT		   (c_param_spec_types[12])
/**
 * C_IS_PARAM_SPEC_FLOAT:
 * @pspec: a valid #CParamSpec instance
 * 
 * Checks whether the given #CParamSpec is of type %C_TYPE_PARAM_FLOAT.
 * 
 * Returns: %TRUE on success.
 */
#define C_IS_PARAM_SPEC_FLOAT(pspec)       (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), C_TYPE_PARAM_FLOAT))
/**
 * C_PARAM_SPEC_FLOAT:
 * @pspec: a valid #CParamSpec instance
 * 
 * Cast a #CParamSpec instance into a #CParamSpecFloat.
 */
#define C_PARAM_SPEC_FLOAT(pspec)          (G_TYPE_CHECK_INSTANCE_CAST ((pspec), C_TYPE_PARAM_FLOAT, CParamSpecFloat))

/**
 * C_TYPE_PARAM_DOUBLE:
 * 
 * The #GType of #CParamSpecDouble.
 */
#define	C_TYPE_PARAM_DOUBLE		   (c_param_spec_types[13])
/**
 * C_IS_PARAM_SPEC_DOUBLE:
 * @pspec: a valid #CParamSpec instance
 * 
 * Checks whether the given #CParamSpec is of type %C_TYPE_PARAM_DOUBLE.
 * 
 * Returns: %TRUE on success.
 */
#define C_IS_PARAM_SPEC_DOUBLE(pspec)      (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), C_TYPE_PARAM_DOUBLE))
/**
 * C_PARAM_SPEC_DOUBLE:
 * @pspec: a valid #CParamSpec instance
 * 
 * Cast a #CParamSpec instance into a #CParamSpecDouble.
 */
#define C_PARAM_SPEC_DOUBLE(pspec)         (G_TYPE_CHECK_INSTANCE_CAST ((pspec), C_TYPE_PARAM_DOUBLE, CParamSpecDouble))

/**
 * C_TYPE_PARAM_STRING:
 * 
 * The #GType of #CParamSpecString.
 */
#define	C_TYPE_PARAM_STRING		   (c_param_spec_types[14])
/**
 * C_IS_PARAM_SPEC_STRING:
 * @pspec: a valid #CParamSpec instance
 * 
 * Checks whether the given #CParamSpec is of type %C_TYPE_PARAM_STRING.
 * 
 * Returns: %TRUE on success.
 */
#define C_IS_PARAM_SPEC_STRING(pspec)      (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), C_TYPE_PARAM_STRING))
/**
 * C_PARAM_SPEC_STRING:
 * @pspec: a valid #CParamSpec instance
 * 
 * Casts a #CParamSpec instance into a #CParamSpecString.
 */
#define C_PARAM_SPEC_STRING(pspec)         (G_TYPE_CHECK_INSTANCE_CAST ((pspec), C_TYPE_PARAM_STRING, CParamSpecString))

/**
 * C_TYPE_PARAM_PARAM:
 * 
 * The #GType of #CParamSpecParam.
 */
#define	C_TYPE_PARAM_PARAM		   (c_param_spec_types[15])
/**
 * C_IS_PARAM_SPEC_PARAM:
 * @pspec: a valid #CParamSpec instance
 * 
 * Checks whether the given #CParamSpec is of type %C_TYPE_PARAM_PARAM.
 * 
 * Returns: %TRUE on success.
 */
#define C_IS_PARAM_SPEC_PARAM(pspec)       (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), C_TYPE_PARAM_PARAM))
/**
 * C_PARAM_SPEC_PARAM:
 * @pspec: a valid #CParamSpec instance
 * 
 * Casts a #CParamSpec instance into a #CParamSpecParam.
 */
#define C_PARAM_SPEC_PARAM(pspec)          (G_TYPE_CHECK_INSTANCE_CAST ((pspec), C_TYPE_PARAM_PARAM, CParamSpecParam))

/**
 * C_TYPE_PARAM_BOXED:
 * 
 * The #GType of #CParamSpecBoxed.
 */
#define	C_TYPE_PARAM_BOXED		   (c_param_spec_types[16])
/**
 * C_IS_PARAM_SPEC_BOXED:
 * @pspec: a valid #CParamSpec instance
 * 
 * Checks whether the given #CParamSpec is of type %C_TYPE_PARAM_BOXED.
 * 
 * Returns: %TRUE on success.
 */
#define C_IS_PARAM_SPEC_BOXED(pspec)       (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), C_TYPE_PARAM_BOXED))
/**
 * C_PARAM_SPEC_BOXED:
 * @pspec: a valid #CParamSpec instance
 * 
 * Cast a #CParamSpec instance into a #CParamSpecBoxed.
 */
#define C_PARAM_SPEC_BOXED(pspec)          (G_TYPE_CHECK_INSTANCE_CAST ((pspec), C_TYPE_PARAM_BOXED, CParamSpecBoxed))

/**
 * C_TYPE_PARAM_POINTER:
 * 
 * The #GType of #CParamSpecPointer.
 */
#define	C_TYPE_PARAM_POINTER		   (c_param_spec_types[17])
/**
 * C_IS_PARAM_SPEC_POINTER:
 * @pspec: a valid #CParamSpec instance
 * 
 * Checks whether the given #CParamSpec is of type %C_TYPE_PARAM_POINTER.
 * 
 * Returns: %TRUE on success.
 */
#define C_IS_PARAM_SPEC_POINTER(pspec)     (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), C_TYPE_PARAM_POINTER))
/**
 * C_PARAM_SPEC_POINTER:
 * @pspec: a valid #CParamSpec instance
 * 
 * Casts a #CParamSpec instance into a #CParamSpecPointer.
 */
#define C_PARAM_SPEC_POINTER(pspec)        (G_TYPE_CHECK_INSTANCE_CAST ((pspec), C_TYPE_PARAM_POINTER, CParamSpecPointer))

/**
 * C_TYPE_PARAM_VALUE_ARRAY:
 * 
 * The #GType of #CParamSpecValueArray.
 *
 * Deprecated: 2.32: Use #GArray instead of #GValueArray
 */
#define	C_TYPE_PARAM_VALUE_ARRAY	   (c_param_spec_types[18]) GOBJECT_DEPRECATED_MACRO_IN_2_32
/**
 * C_IS_PARAM_SPEC_VALUE_ARRAY:
 * @pspec: a valid #CParamSpec instance
 * 
 * Checks whether the given #CParamSpec is of type %C_TYPE_PARAM_VALUE_ARRAY.
 * 
 * Returns: %TRUE on success.
 *
 * Deprecated: 2.32: Use #GArray instead of #GValueArray
 */
#define C_IS_PARAM_SPEC_VALUE_ARRAY(pspec) (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), C_TYPE_PARAM_VALUE_ARRAY)) GOBJECT_DEPRECATED_MACRO_IN_2_32
/**
 * C_PARAM_SPEC_VALUE_ARRAY:
 * @pspec: a valid #CParamSpec instance
 * 
 * Cast a #CParamSpec instance into a #CParamSpecValueArray.
 *
 * Deprecated: 2.32: Use #GArray instead of #GValueArray
 */
#define C_PARAM_SPEC_VALUE_ARRAY(pspec)    (G_TYPE_CHECK_INSTANCE_CAST ((pspec), C_TYPE_PARAM_VALUE_ARRAY, CParamSpecValueArray)) GOBJECT_DEPRECATED_MACRO_IN_2_32

/**
 * C_TYPE_PARAM_OBJECT:
 * 
 * The #GType of #CParamSpecObject.
 */
#define	C_TYPE_PARAM_OBJECT		   (c_param_spec_types[19])
/**
 * C_IS_PARAM_SPEC_OBJECT:
 * @pspec: a valid #CParamSpec instance
 * 
 * Checks whether the given #CParamSpec is of type %C_TYPE_PARAM_OBJECT.
 * 
 * Returns: %TRUE on success.
 */
#define C_IS_PARAM_SPEC_OBJECT(pspec)      (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), C_TYPE_PARAM_OBJECT))
/**
 * C_PARAM_SPEC_OBJECT:
 * @pspec: a valid #CParamSpec instance
 * 
 * Casts a #CParamSpec instance into a #CParamSpecObject.
 */
#define C_PARAM_SPEC_OBJECT(pspec)         (G_TYPE_CHECK_INSTANCE_CAST ((pspec), C_TYPE_PARAM_OBJECT, CParamSpecObject))

/**
 * C_TYPE_PARAM_OVERRIDE:
 * 
 * The #GType of #CParamSpecOverride.
 * 
 * Since: 2.4
 */
#define	C_TYPE_PARAM_OVERRIDE		   (c_param_spec_types[20])
/**
 * C_IS_PARAM_SPEC_OVERRIDE:
 * @pspec: a #CParamSpec
 * 
 * Checks whether the given #CParamSpec is of type %C_TYPE_PARAM_OVERRIDE.
 * 
 * Since: 2.4
 * Returns: %TRUE on success.
 */
#define C_IS_PARAM_SPEC_OVERRIDE(pspec)    (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), C_TYPE_PARAM_OVERRIDE))
/**
 * C_PARAM_SPEC_OVERRIDE:
 * @pspec: a #CParamSpec
 * 
 * Casts a #CParamSpec into a #CParamSpecOverride.
 * 
 * Since: 2.4
 */
#define C_PARAM_SPEC_OVERRIDE(pspec)       (G_TYPE_CHECK_INSTANCE_CAST ((pspec), C_TYPE_PARAM_OVERRIDE, CParamSpecOverride))

/**
 * C_TYPE_PARAM_GTYPE:
 * 
 * The #GType of #CParamSpecGType.
 * 
 * Since: 2.10
 */
#define	C_TYPE_PARAM_GTYPE		   (c_param_spec_types[21])
/**
 * C_IS_PARAM_SPEC_GTYPE:
 * @pspec: a #CParamSpec
 * 
 * Checks whether the given #CParamSpec is of type %C_TYPE_PARAM_GTYPE.
 * 
 * Since: 2.10
 * Returns: %TRUE on success. 
 */
#define C_IS_PARAM_SPEC_GTYPE(pspec)       (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), C_TYPE_PARAM_GTYPE))
/**
 * C_PARAM_SPEC_GTYPE:
 * @pspec: a #CParamSpec
 * 
 * Casts a #CParamSpec into a #CParamSpecGType.
 * 
 * Since: 2.10
 */
#define C_PARAM_SPEC_GTYPE(pspec)          (G_TYPE_CHECK_INSTANCE_CAST ((pspec), C_TYPE_PARAM_GTYPE, CParamSpecGType))

/**
 * C_TYPE_PARAM_VARIANT:
 *
 * The #GType of #CParamSpecVariant.
 *
 * Since: 2.26
 */
#define C_TYPE_PARAM_VARIANT                (c_param_spec_types[22])
/**
 * C_IS_PARAM_SPEC_VARIANT:
 * @pspec: a #CParamSpec
 *
 * Checks whether the given #CParamSpec is of type %C_TYPE_PARAM_VARIANT.
 *
 * Returns: %TRUE on success
 *
 * Since: 2.26
 */
#define C_IS_PARAM_SPEC_VARIANT(pspec)      (G_TYPE_CHECK_INSTANCE_TYPE ((pspec), C_TYPE_PARAM_VARIANT))
/**
 * C_PARAM_SPEC_VARIANT:
 * @pspec: a #CParamSpec
 *
 * Casts a #CParamSpec into a #CParamSpecVariant.
 *
 * Since: 2.26
 */
#define C_PARAM_SPEC_VARIANT(pspec)         (G_TYPE_CHECK_INSTANCE_CAST ((pspec), C_TYPE_PARAM_VARIANT, CParamSpecVariant))

/* --- typedefs & structures --- */
typedef struct _CParamSpecChar       CParamSpecChar;
typedef struct _CParamSpecUChar      CParamSpecUChar;
typedef struct _CParamSpecBoolean    CParamSpecBoolean;
typedef struct _CParamSpecInt        CParamSpecInt;
typedef struct _CParamSpecUInt       CParamSpecUInt;
typedef struct _CParamSpecLong       CParamSpecLong;
typedef struct _CParamSpecULong      CParamSpecULong;
typedef struct _CParamSpecInt64      CParamSpecInt64;
typedef struct _CParamSpecUInt64     CParamSpecUInt64;
typedef struct _CParamSpecUnichar    CParamSpecUnichar;
typedef struct _CParamSpecEnum       CParamSpecEnum;
typedef struct _CParamSpecFlags      CParamSpecFlags;
typedef struct _CParamSpecFloat      CParamSpecFloat;
typedef struct _CParamSpecDouble     CParamSpecDouble;
typedef struct _CParamSpecString     CParamSpecString;
typedef struct _CParamSpecParam      CParamSpecParam;
typedef struct _CParamSpecBoxed      CParamSpecBoxed;
typedef struct _CParamSpecPointer    CParamSpecPointer;
typedef struct _CParamSpecValueArray CParamSpecValueArray;
typedef struct _CParamSpecObject     CParamSpecObject;
typedef struct _CParamSpecOverride   CParamSpecOverride;
typedef struct _CParamSpecGType      CParamSpecGType;
typedef struct _CParamSpecVariant    CParamSpecVariant;

/**
 * CParamSpecChar:
 * @parent_instance: private #CParamSpec portion
 * @minimum: minimum value for the property specified
 * @maximum: maximum value for the property specified
 * @default_value: default value for the property specified
 * 
 * A #CParamSpec derived structure that contains the meta data for character properties.
 */
struct _CParamSpecChar
{
  CParamSpec    parent_instance;
  
  gint8         minimum;
  gint8         maximum;
  gint8         default_value;
};
/**
 * CParamSpecUChar:
 * @parent_instance: private #CParamSpec portion
 * @minimum: minimum value for the property specified
 * @maximum: maximum value for the property specified
 * @default_value: default value for the property specified
 * 
 * A #CParamSpec derived structure that contains the meta data for unsigned character properties.
 */
struct _CParamSpecUChar
{
  CParamSpec    parent_instance;
  
  guint8        minimum;
  guint8        maximum;
  guint8        default_value;
};
/**
 * CParamSpecBoolean:
 * @parent_instance: private #CParamSpec portion
 * @default_value: default value for the property specified
 * 
 * A #CParamSpec derived structure that contains the meta data for boolean properties.
 */
struct _CParamSpecBoolean
{
  CParamSpec    parent_instance;
  
  gboolean      default_value;
};
/**
 * CParamSpecInt:
 * @parent_instance: private #CParamSpec portion
 * @minimum: minimum value for the property specified
 * @maximum: maximum value for the property specified
 * @default_value: default value for the property specified
 * 
 * A #CParamSpec derived structure that contains the meta data for integer properties.
 */
struct _CParamSpecInt
{
  CParamSpec    parent_instance;
  
  gint          minimum;
  gint          maximum;
  gint          default_value;
};
/**
 * CParamSpecUInt:
 * @parent_instance: private #CParamSpec portion
 * @minimum: minimum value for the property specified
 * @maximum: maximum value for the property specified
 * @default_value: default value for the property specified
 * 
 * A #CParamSpec derived structure that contains the meta data for unsigned integer properties.
 */
struct _CParamSpecUInt
{
  CParamSpec    parent_instance;
  
  guint         minimum;
  guint         maximum;
  guint         default_value;
};
/**
 * CParamSpecLong:
 * @parent_instance: private #CParamSpec portion
 * @minimum: minimum value for the property specified
 * @maximum: maximum value for the property specified
 * @default_value: default value for the property specified
 * 
 * A #CParamSpec derived structure that contains the meta data for long integer properties.
 */
struct _CParamSpecLong
{
  CParamSpec    parent_instance;
  
  glong         minimum;
  glong         maximum;
  glong         default_value;
};
/**
 * CParamSpecULong:
 * @parent_instance: private #CParamSpec portion
 * @minimum: minimum value for the property specified
 * @maximum: maximum value for the property specified
 * @default_value: default value for the property specified
 * 
 * A #CParamSpec derived structure that contains the meta data for unsigned long integer properties.
 */
struct _CParamSpecULong
{
  CParamSpec    parent_instance;
  
  gulong        minimum;
  gulong        maximum;
  gulong        default_value;
};
/**
 * CParamSpecInt64:
 * @parent_instance: private #CParamSpec portion
 * @minimum: minimum value for the property specified
 * @maximum: maximum value for the property specified
 * @default_value: default value for the property specified
 * 
 * A #CParamSpec derived structure that contains the meta data for 64bit integer properties.
 */
struct _CParamSpecInt64
{
  CParamSpec    parent_instance;
  
  gint64        minimum;
  gint64        maximum;
  gint64        default_value;
};
/**
 * CParamSpecUInt64:
 * @parent_instance: private #CParamSpec portion
 * @minimum: minimum value for the property specified
 * @maximum: maximum value for the property specified
 * @default_value: default value for the property specified
 * 
 * A #CParamSpec derived structure that contains the meta data for unsigned 64bit integer properties.
 */
struct _CParamSpecUInt64
{
  CParamSpec    parent_instance;
  
  guint64       minimum;
  guint64       maximum;
  guint64       default_value;
};
/**
 * CParamSpecUnichar:
 * @parent_instance: private #CParamSpec portion
 * @default_value: default value for the property specified
 * 
 * A #CParamSpec derived structure that contains the meta data for unichar (unsigned integer) properties.
 */
struct _CParamSpecUnichar
{
  CParamSpec    parent_instance;
  
  gunichar      default_value;
};
/**
 * CParamSpecEnum:
 * @parent_instance: private #CParamSpec portion
 * @enum_class: the #GEnumClass for the enum
 * @default_value: default value for the property specified
 * 
 * A #CParamSpec derived structure that contains the meta data for enum 
 * properties.
 */
struct _CParamSpecEnum
{
  CParamSpec    parent_instance;
  
  GEnumClass   *enum_class;
  gint          default_value;
};
/**
 * CParamSpecFlags:
 * @parent_instance: private #CParamSpec portion
 * @flags_class: the #GFlagsClass for the flags
 * @default_value: default value for the property specified
 * 
 * A #CParamSpec derived structure that contains the meta data for flags
 * properties.
 */
struct _CParamSpecFlags
{
  CParamSpec    parent_instance;
  
  GFlagsClass  *flags_class;
  guint         default_value;
};
/**
 * CParamSpecFloat:
 * @parent_instance: private #CParamSpec portion
 * @minimum: minimum value for the property specified
 * @maximum: maximum value for the property specified
 * @default_value: default value for the property specified
 * @epsilon: values closer than @epsilon will be considered identical
 *  by c_param_values_cmp(); the default value is 1e-30.
 * 
 * A #CParamSpec derived structure that contains the meta data for float properties.
 */
struct _CParamSpecFloat
{
  CParamSpec    parent_instance;
  
  gfloat        minimum;
  gfloat        maximum;
  gfloat        default_value;
  gfloat        epsilon;
};
/**
 * CParamSpecDouble:
 * @parent_instance: private #CParamSpec portion
 * @minimum: minimum value for the property specified
 * @maximum: maximum value for the property specified
 * @default_value: default value for the property specified
 * @epsilon: values closer than @epsilon will be considered identical
 *  by c_param_values_cmp(); the default value is 1e-90.
 * 
 * A #CParamSpec derived structure that contains the meta data for double properties.
 */
struct _CParamSpecDouble
{
  CParamSpec    parent_instance;
  
  gdouble       minimum;
  gdouble       maximum;
  gdouble       default_value;
  gdouble       epsilon;
};
/**
 * CParamSpecString:
 * @parent_instance: private #CParamSpec portion
 * @default_value: default value for the property specified
 * @cset_first: a string containing the allowed values for the first byte
 * @cset_nth: a string containing the allowed values for the subsequent bytes
 * @substitutor: the replacement byte for bytes which don't match @cset_first or @cset_nth.
 * @null_fold_if_empty: replace empty string by %NULL
 * @ensure_non_null: replace %NULL strings by an empty string
 * 
 * A #CParamSpec derived structure that contains the meta data for string
 * properties.
 */
struct _CParamSpecString
{
  CParamSpec    parent_instance;
  
  gchar        *default_value;
  gchar        *cset_first;
  gchar        *cset_nth;
  gchar         substitutor;
  guint         null_fold_if_empty : 1;
  guint         ensure_non_null : 1;
};
/**
 * CParamSpecParam:
 * @parent_instance: private #CParamSpec portion
 * 
 * A #CParamSpec derived structure that contains the meta data for %C_TYPE_PARAM
 * properties.
 */
struct _CParamSpecParam
{
  CParamSpec    parent_instance;
};
/**
 * CParamSpecBoxed:
 * @parent_instance: private #CParamSpec portion
 * 
 * A #CParamSpec derived structure that contains the meta data for boxed properties.
 */
struct _CParamSpecBoxed
{
  CParamSpec    parent_instance;
};
/**
 * CParamSpecPointer:
 * @parent_instance: private #CParamSpec portion
 * 
 * A #CParamSpec derived structure that contains the meta data for pointer properties.
 */
struct _CParamSpecPointer
{
  CParamSpec    parent_instance;
};
/**
 * CParamSpecValueArray:
 * @parent_instance: private #CParamSpec portion
 * @element_spec: a #CParamSpec describing the elements contained in arrays of this property, may be %NULL
 * @fixed_n_elements: if greater than 0, arrays of this property will always have this many elements
 * 
 * A #CParamSpec derived structure that contains the meta data for #GValueArray properties.
 */
struct _CParamSpecValueArray
{
  CParamSpec    parent_instance;
  CParamSpec   *element_spec;
  guint		fixed_n_elements;
};
/**
 * CParamSpecObject:
 * @parent_instance: private #CParamSpec portion
 * 
 * A #CParamSpec derived structure that contains the meta data for object properties.
 */
struct _CParamSpecObject
{
  CParamSpec    parent_instance;
};
/**
 * CParamSpecOverride:
 *
 * A #CParamSpec derived structure that redirects operations to
 * other types of #CParamSpec.
 * 
 * All operations other than getting or setting the value are redirected,
 * including accessing the nick and blurb, validating a value, and so
 * forth.
 *
 * See c_param_spec_get_redirect_target() for retrieving the overridden
 * property. #CParamSpecOverride is used in implementing
 * g_object_class_override_property(), and will not be directly useful
 * unless you are implementing a new base type similar to GObject.
 * 
 * Since: 2.4
 */
struct _CParamSpecOverride
{
  /*< private >*/
  CParamSpec    parent_instance;
  CParamSpec   *overridden;
};
/**
 * CParamSpecGType:
 * @parent_instance: private #CParamSpec portion
 * @is_a_type: a #GType whose subtypes can occur as values
 * 
 * A #CParamSpec derived structure that contains the meta data for #GType properties.
 * 
 * Since: 2.10
 */
struct _CParamSpecGType
{
  CParamSpec    parent_instance;
  GType         is_a_type;
};
/**
 * CParamSpecVariant:
 * @parent_instance: private #CParamSpec portion
 * @type: a #GVariantType, or %NULL
 * @default_value: a #GVariant, or %NULL
 *
 * A #CParamSpec derived structure that contains the meta data for #GVariant properties.
 *
 * When comparing values with c_param_values_cmp(), scalar values with the same
 * type will be compared with g_variant_compare(). Other non-%NULL variants will
 * be checked for equality with g_variant_equal(), and their sort order is
 * otherwise undefined. %NULL is ordered before non-%NULL variants. Two %NULL
 * values compare equal.
 *
 * Since: 2.26
 */
struct _CParamSpecVariant
{
  CParamSpec    parent_instance;
  GVariantType *type;
  GVariant     *default_value;

  /*< private >*/
  gpointer      padding[4];
};

/* --- CParamSpec prototypes --- */
GOBJECT_AVAILABLE_IN_ALL
CParamSpec*	c_param_spec_char	 (const gchar	 *name,
					  const gchar	 *nick,
					  const gchar	 *blurb,
					  gint8		  minimum,
					  gint8		  maximum,
					  gint8		  default_value,
					  CParamFlags	  flags);
GOBJECT_AVAILABLE_IN_ALL
CParamSpec*	c_param_spec_uchar	 (const gchar	 *name,
					  const gchar	 *nick,
					  const gchar	 *blurb,
					  guint8	  minimum,
					  guint8	  maximum,
					  guint8	  default_value,
					  CParamFlags	  flags);
GOBJECT_AVAILABLE_IN_ALL
CParamSpec*	c_param_spec_boolean	 (const gchar	 *name,
					  const gchar	 *nick,
					  const gchar	 *blurb,
					  gboolean	  default_value,
					  CParamFlags	  flags);
GOBJECT_AVAILABLE_IN_ALL
CParamSpec*	c_param_spec_int	 (const gchar	 *name,
					  const gchar	 *nick,
					  const gchar	 *blurb,
					  gint		  minimum,
					  gint		  maximum,
					  gint		  default_value,
					  CParamFlags	  flags);
GOBJECT_AVAILABLE_IN_ALL
CParamSpec*	c_param_spec_uint	 (const gchar	 *name,
					  const gchar	 *nick,
					  const gchar	 *blurb,
					  guint		  minimum,
					  guint		  maximum,
					  guint		  default_value,
					  CParamFlags	  flags);
GOBJECT_AVAILABLE_IN_ALL
CParamSpec*	c_param_spec_long	 (const gchar	 *name,
					  const gchar	 *nick,
					  const gchar	 *blurb,
					  glong		  minimum,
					  glong		  maximum,
					  glong		  default_value,
					  CParamFlags	  flags);
GOBJECT_AVAILABLE_IN_ALL
CParamSpec*	c_param_spec_ulong	 (const gchar	 *name,
					  const gchar	 *nick,
					  const gchar	 *blurb,
					  gulong	  minimum,
					  gulong	  maximum,
					  gulong	  default_value,
					  CParamFlags	  flags);
GOBJECT_AVAILABLE_IN_ALL
CParamSpec*	c_param_spec_int64	 (const gchar	 *name,
					  const gchar	 *nick,
					  const gchar	 *blurb,
					  gint64       	  minimum,
					  gint64       	  maximum,
					  gint64       	  default_value,
					  CParamFlags	  flags);
GOBJECT_AVAILABLE_IN_ALL
CParamSpec*	c_param_spec_uint64	 (const gchar	 *name,
					  const gchar	 *nick,
					  const gchar	 *blurb,
					  guint64	  minimum,
					  guint64	  maximum,
					  guint64	  default_value,
					  CParamFlags	  flags);
GOBJECT_AVAILABLE_IN_ALL
CParamSpec*	c_param_spec_unichar      (const gchar    *name,
				          const gchar    *nick,
				          const gchar    *blurb,
				          gunichar	  default_value,
				          CParamFlags     flags);
GOBJECT_AVAILABLE_IN_ALL
CParamSpec*	c_param_spec_enum	 (const gchar	 *name,
					  const gchar	 *nick,
					  const gchar	 *blurb,
					  GType		  enum_type,
					  gint		  default_value,
					  CParamFlags	  flags);
GOBJECT_AVAILABLE_IN_ALL
CParamSpec*	c_param_spec_flags	 (const gchar	 *name,
					  const gchar	 *nick,
					  const gchar	 *blurb,
					  GType		  flags_type,
					  guint		  default_value,
					  CParamFlags	  flags);
GOBJECT_AVAILABLE_IN_ALL
CParamSpec*	c_param_spec_float	 (const gchar	 *name,
					  const gchar	 *nick,
					  const gchar	 *blurb,
					  gfloat	  minimum,
					  gfloat	  maximum,
					  gfloat	  default_value,
					  CParamFlags	  flags);
GOBJECT_AVAILABLE_IN_ALL
CParamSpec*	c_param_spec_double	 (const gchar	 *name,
					  const gchar	 *nick,
					  const gchar	 *blurb,
					  gdouble	  minimum,
					  gdouble	  maximum,
					  gdouble	  default_value,
					  CParamFlags	  flags);
GOBJECT_AVAILABLE_IN_ALL
CParamSpec*	c_param_spec_string	 (const gchar	 *name,
					  const gchar	 *nick,
					  const gchar	 *blurb,
					  const gchar	 *default_value,
					  CParamFlags	  flags);
GOBJECT_AVAILABLE_IN_ALL
CParamSpec*	c_param_spec_param	 (const gchar	 *name,
					  const gchar	 *nick,
					  const gchar	 *blurb,
					  GType		  param_type,
					  CParamFlags	  flags);
GOBJECT_AVAILABLE_IN_ALL
CParamSpec*	c_param_spec_boxed	 (const gchar	 *name,
					  const gchar	 *nick,
					  const gchar	 *blurb,
					  GType		  boxed_type,
					  CParamFlags	  flags);
GOBJECT_AVAILABLE_IN_ALL
CParamSpec*	c_param_spec_pointer	 (const gchar	 *name,
					  const gchar	 *nick,
					  const gchar	 *blurb,
					  CParamFlags	  flags);
GOBJECT_AVAILABLE_IN_ALL
CParamSpec*	c_param_spec_value_array (const gchar	 *name,
					  const gchar	 *nick,
					  const gchar	 *blurb,
					  CParamSpec	 *element_spec,
					  CParamFlags	  flags);
GOBJECT_AVAILABLE_IN_ALL
CParamSpec*	c_param_spec_object	 (const gchar	 *name,
					  const gchar	 *nick,
					  const gchar	 *blurb,
					  GType		  object_type,
					  CParamFlags	  flags);
GOBJECT_AVAILABLE_IN_ALL
CParamSpec*	c_param_spec_override    (const gchar    *name,
					  CParamSpec     *overridden);
GOBJECT_AVAILABLE_IN_ALL
CParamSpec*	c_param_spec_gtype	 (const gchar	 *name,
					  const gchar	 *nick,
					  const gchar	 *blurb,
					  GType           is_a_type,
					  CParamFlags	  flags);
GOBJECT_AVAILABLE_IN_ALL
CParamSpec*	c_param_spec_variant	 (const gchar        *name,
					  const gchar        *nick,
					  const gchar	     *blurb,
					  const GVariantType *type,
					  GVariant           *default_value,
					  CParamFlags         flags);

GOBJECT_VAR GType *c_param_spec_types;

G_END_DECLS
