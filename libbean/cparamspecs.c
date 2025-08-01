/* GObject - GLib Type, Object, Parameter and Signal Library
 * Copyright (C) 1997-1999, 2000-2001 Tim Janik and Red Hat, Inc.
 * Copyright (C) 2010 Christian Persch
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

/*
 * MT safe
 */


#include "config.h"

#include <string.h>

#include "cparamspecs.h"


#define	G_FLOAT_EPSILON		(1e-30f)
#define	G_DOUBLE_EPSILON	(1e-90)


/* --- param spec functions --- */
static void
param_char_init (CParamSpec *pspec)
{
  CParamSpecChar *cspec = C_PARAM_SPEC_CHAR (pspec);
  
  cspec->minimum = 0x7f;
  cspec->maximum = 0x80;
  cspec->default_value = 0;
}

static void
param_char_set_default (CParamSpec *pspec,
			GValue	   *value)
{
  value->data[0].v_int = C_PARAM_SPEC_CHAR (pspec)->default_value;
}

static gboolean
param_char_is_valid (CParamSpec   *pspec,
                     const GValue *value)
{
  CParamSpecChar *cspec = C_PARAM_SPEC_CHAR (pspec);
  gint oval = value->data[0].v_int;
  
  return cspec->minimum <= oval && oval <= cspec->maximum;
}

static gboolean
param_char_validate (CParamSpec *pspec,
		     GValue     *value)
{
  CParamSpecChar *cspec = C_PARAM_SPEC_CHAR (pspec);
  gint oval = value->data[0].v_int;
  
  value->data[0].v_int = CLAMP (value->data[0].v_int, cspec->minimum, cspec->maximum);
  
  return value->data[0].v_int != oval;
}

static void
param_uchar_init (CParamSpec *pspec)
{
  CParamSpecUChar *uspec = C_PARAM_SPEC_UCHAR (pspec);
  
  uspec->minimum = 0;
  uspec->maximum = 0xff;
  uspec->default_value = 0;
}

static void
param_uchar_set_default (CParamSpec *pspec,
			 GValue	    *value)
{
  value->data[0].v_uint = C_PARAM_SPEC_UCHAR (pspec)->default_value;
}

static gboolean
param_uchar_is_valid (CParamSpec   *pspec,
		      const GValue *value)
{
  CParamSpecUChar *uspec = C_PARAM_SPEC_UCHAR (pspec);
  guint oval = value->data[0].v_uint;
  
  return uspec->minimum <= oval && oval <= uspec->maximum;
}

static gboolean
param_uchar_validate (CParamSpec *pspec,
		      GValue     *value)
{
  CParamSpecUChar *uspec = C_PARAM_SPEC_UCHAR (pspec);
  guint oval = value->data[0].v_uint;
  
  value->data[0].v_uint = CLAMP (value->data[0].v_uint, uspec->minimum, uspec->maximum);
  
  return value->data[0].v_uint != oval;
}

static void
param_boolean_set_default (CParamSpec *pspec,
			   GValue     *value)
{
  value->data[0].v_int = C_PARAM_SPEC_BOOLEAN (pspec)->default_value;
}

static gboolean
param_boolean_is_valid (CParamSpec   *pspec,
                        const GValue *value)
{
  int oval = value->data[0].v_int;

  return oval == FALSE || oval == TRUE;
}

static gboolean
param_boolean_validate (CParamSpec *pspec,
			GValue     *value)
{
  gint oval = value->data[0].v_int;
  
  value->data[0].v_int = value->data[0].v_int != FALSE;
  
  return value->data[0].v_int != oval;
}

static void
param_int_init (CParamSpec *pspec)
{
  CParamSpecInt *ispec = C_PARAM_SPEC_INT (pspec);
  
  ispec->minimum = 0x7fffffff;
  ispec->maximum = 0x80000000;
  ispec->default_value = 0;
}

static void
param_int_set_default (CParamSpec *pspec,
		       GValue     *value)
{
  value->data[0].v_int = C_PARAM_SPEC_INT (pspec)->default_value;
}

static gboolean
param_int_is_valid (CParamSpec   *pspec,
                    const GValue *value)
{
  CParamSpecInt *ispec = C_PARAM_SPEC_INT (pspec);
  int oval = value->data[0].v_int;

  return ispec->minimum <= oval && oval <= ispec->maximum;
}

static gboolean
param_int_validate (CParamSpec *pspec,
		    GValue     *value)
{
  CParamSpecInt *ispec = C_PARAM_SPEC_INT (pspec);
  gint oval = value->data[0].v_int;
  
  value->data[0].v_int = CLAMP (value->data[0].v_int, ispec->minimum, ispec->maximum);
  
  return value->data[0].v_int != oval;
}

static gint
param_int_values_cmp (CParamSpec   *pspec,
		      const GValue *value1,
		      const GValue *value2)
{
  if (value1->data[0].v_int < value2->data[0].v_int)
    return -1;
  else
    return value1->data[0].v_int > value2->data[0].v_int;
}

static void
param_uint_init (CParamSpec *pspec)
{
  CParamSpecUInt *uspec = C_PARAM_SPEC_UINT (pspec);
  
  uspec->minimum = 0;
  uspec->maximum = 0xffffffff;
  uspec->default_value = 0;
}

static void
param_uint_set_default (CParamSpec *pspec,
			GValue     *value)
{
  value->data[0].v_uint = C_PARAM_SPEC_UINT (pspec)->default_value;
}

static gboolean
param_uint_is_valid (CParamSpec   *pspec,
		     const GValue *value)
{
  CParamSpecUInt *uspec = C_PARAM_SPEC_UINT (pspec);
  guint oval = value->data[0].v_uint;
  
  return uspec->minimum <= oval && oval <= uspec->maximum;
}

static gboolean
param_uint_validate (CParamSpec *pspec,
		     GValue     *value)
{
  CParamSpecUInt *uspec = C_PARAM_SPEC_UINT (pspec);
  guint oval = value->data[0].v_uint;
  
  value->data[0].v_uint = CLAMP (value->data[0].v_uint, uspec->minimum, uspec->maximum);
  
  return value->data[0].v_uint != oval;
}

static gint
param_uint_values_cmp (CParamSpec   *pspec,
		       const GValue *value1,
		       const GValue *value2)
{
  if (value1->data[0].v_uint < value2->data[0].v_uint)
    return -1;
  else
    return value1->data[0].v_uint > value2->data[0].v_uint;
}

static void
param_long_init (CParamSpec *pspec)
{
  CParamSpecLong *lspec = C_PARAM_SPEC_LONG (pspec);
  
#if SIZEOF_LONG == 4
  lspec->minimum = 0x7fffffff;
  lspec->maximum = 0x80000000;
#else /* SIZEOF_LONG != 4 (8) */
  lspec->minimum = 0x7fffffffffffffff;
  lspec->maximum = 0x8000000000000000;
#endif
  lspec->default_value = 0;
}

static void
param_long_set_default (CParamSpec *pspec,
			GValue     *value)
{
  value->data[0].v_long = C_PARAM_SPEC_LONG (pspec)->default_value;
}

static gboolean
param_long_is_valid (CParamSpec   *pspec,
		     const GValue *value)
{
  CParamSpecLong *lspec = C_PARAM_SPEC_LONG (pspec);
  glong oval = value->data[0].v_long;
  
  return lspec->minimum <= oval && oval <= lspec->maximum;
}

static gboolean
param_long_validate (CParamSpec *pspec,
		     GValue     *value)
{
  CParamSpecLong *lspec = C_PARAM_SPEC_LONG (pspec);
  glong oval = value->data[0].v_long;
  
  value->data[0].v_long = CLAMP (value->data[0].v_long, lspec->minimum, lspec->maximum);
  
  return value->data[0].v_long != oval;
}

static gint
param_long_values_cmp (CParamSpec   *pspec,
		       const GValue *value1,
		       const GValue *value2)
{
  if (value1->data[0].v_long < value2->data[0].v_long)
    return -1;
  else
    return value1->data[0].v_long > value2->data[0].v_long;
}

static void
param_ulong_init (CParamSpec *pspec)
{
  CParamSpecULong *uspec = C_PARAM_SPEC_ULONG (pspec);
  
  uspec->minimum = 0;
#if SIZEOF_LONG == 4
  uspec->maximum = 0xffffffff;
#else /* SIZEOF_LONG != 4 (8) */
  uspec->maximum = 0xffffffffffffffff;
#endif
  uspec->default_value = 0;
}

static void
param_ulong_set_default (CParamSpec *pspec,
			 GValue     *value)
{
  value->data[0].v_ulong = C_PARAM_SPEC_ULONG (pspec)->default_value;
}

static gboolean
param_ulong_is_valid (CParamSpec   *pspec,
                      const GValue *value)
{
  CParamSpecULong *uspec = C_PARAM_SPEC_ULONG (pspec);
  gulong oval = value->data[0].v_ulong;
  
  return uspec->minimum <= oval && oval <= uspec->maximum;
}

static gboolean
param_ulong_validate (CParamSpec *pspec,
		      GValue     *value)
{
  CParamSpecULong *uspec = C_PARAM_SPEC_ULONG (pspec);
  gulong oval = value->data[0].v_ulong;
  
  value->data[0].v_ulong = CLAMP (value->data[0].v_ulong, uspec->minimum, uspec->maximum);
  
  return value->data[0].v_ulong != oval;
}

static gint
param_ulong_values_cmp (CParamSpec   *pspec,
			const GValue *value1,
			const GValue *value2)
{
  if (value1->data[0].v_ulong < value2->data[0].v_ulong)
    return -1;
  else
    return value1->data[0].v_ulong > value2->data[0].v_ulong;
}

static void
param_int64_init (CParamSpec *pspec)
{
  CParamSpecInt64 *lspec = C_PARAM_SPEC_INT64 (pspec);
  
  lspec->minimum = G_MININT64;
  lspec->maximum = G_MAXINT64;
  lspec->default_value = 0;
}

static void
param_int64_set_default (CParamSpec *pspec,
			GValue     *value)
{
  value->data[0].v_int64 = C_PARAM_SPEC_INT64 (pspec)->default_value;
}

static gboolean
param_int64_is_valid (CParamSpec   *pspec,
                      const GValue *value)
{
  CParamSpecInt64 *lspec = C_PARAM_SPEC_INT64 (pspec);
  gint64 oval = value->data[0].v_int64;
  
  return lspec->minimum <= oval && oval <= lspec->maximum;
}

static gboolean
param_int64_validate (CParamSpec *pspec,
		     GValue     *value)
{
  CParamSpecInt64 *lspec = C_PARAM_SPEC_INT64 (pspec);
  gint64 oval = value->data[0].v_int64;
  
  value->data[0].v_int64 = CLAMP (value->data[0].v_int64, lspec->minimum, lspec->maximum);
  
  return value->data[0].v_int64 != oval;
}

static gint
param_int64_values_cmp (CParamSpec   *pspec,
		       const GValue *value1,
		       const GValue *value2)
{
  if (value1->data[0].v_int64 < value2->data[0].v_int64)
    return -1;
  else
    return value1->data[0].v_int64 > value2->data[0].v_int64;
}

static void
param_uint64_init (CParamSpec *pspec)
{
  CParamSpecUInt64 *uspec = C_PARAM_SPEC_UINT64 (pspec);
  
  uspec->minimum = 0;
  uspec->maximum = G_MAXUINT64;
  uspec->default_value = 0;
}

static void
param_uint64_set_default (CParamSpec *pspec,
			 GValue     *value)
{
  value->data[0].v_uint64 = C_PARAM_SPEC_UINT64 (pspec)->default_value;
}

static gboolean
param_uint64_is_valid (CParamSpec   *pspec,
                       const GValue *value)
{
  CParamSpecUInt64 *uspec = C_PARAM_SPEC_UINT64 (pspec);
  guint64 oval = value->data[0].v_uint64;
  
  return uspec->minimum <= oval && oval <= uspec->maximum;
}

static gboolean
param_uint64_validate (CParamSpec *pspec,
		      GValue     *value)
{
  CParamSpecUInt64 *uspec = C_PARAM_SPEC_UINT64 (pspec);
  guint64 oval = value->data[0].v_uint64;
  
  value->data[0].v_uint64 = CLAMP (value->data[0].v_uint64, uspec->minimum, uspec->maximum);
  
  return value->data[0].v_uint64 != oval;
}

static gint
param_uint64_values_cmp (CParamSpec   *pspec,
			const GValue *value1,
			const GValue *value2)
{
  if (value1->data[0].v_uint64 < value2->data[0].v_uint64)
    return -1;
  else
    return value1->data[0].v_uint64 > value2->data[0].v_uint64;
}

static void
param_unichar_init (CParamSpec *pspec)
{
  CParamSpecUnichar *uspec = C_PARAM_SPEC_UNICHAR (pspec);
  
  uspec->default_value = 0;
}

static void
param_unichar_set_default (CParamSpec *pspec,
			 GValue     *value)
{
  value->data[0].v_uint = C_PARAM_SPEC_UNICHAR (pspec)->default_value;
}

static gboolean
param_unichar_is_valid (CParamSpec   *pspec,
                        const GValue *value)
{
  return g_unichar_validate (value->data[0].v_uint);
}

static gboolean
param_unichar_validate (CParamSpec *pspec,
		        GValue     *value)
{
  gunichar oval = value->data[0].v_uint;
  gboolean changed = FALSE;

  if (!g_unichar_validate (oval))
    {
      value->data[0].v_uint = 0;
      changed = TRUE;
    }

  return changed;
}

static gint
param_unichar_values_cmp (CParamSpec   *pspec,
			const GValue *value1,
			const GValue *value2)
{
  if (value1->data[0].v_uint < value2->data[0].v_uint)
    return -1;
  else
    return value1->data[0].v_uint > value2->data[0].v_uint;
}

static void
param_enum_init (CParamSpec *pspec)
{
  CParamSpecEnum *espec = C_PARAM_SPEC_ENUM (pspec);
  
  espec->enum_class = NULL;
  espec->default_value = 0;
}

static void
param_enum_finalize (CParamSpec *pspec)
{
  CParamSpecEnum *espec = C_PARAM_SPEC_ENUM (pspec);
  CParamSpecClass *parent_class = g_type_class_peek (g_type_parent (C_TYPE_PARAM_ENUM));
  
  if (espec->enum_class)
    {
      g_type_class_unref (espec->enum_class);
      espec->enum_class = NULL;
    }
  
  parent_class->finalize (pspec);
}

static void
param_enum_set_default (CParamSpec *pspec,
			GValue     *value)
{
  value->data[0].v_long = C_PARAM_SPEC_ENUM (pspec)->default_value;
}

static gboolean
param_enum_is_valid (CParamSpec   *pspec,
                     const GValue *value)
{
  CParamSpecEnum *espec = C_PARAM_SPEC_ENUM (pspec);
  glong oval = value->data[0].v_long;
  
  return g_enum_get_value (espec->enum_class, oval) != NULL;
}

static gboolean
param_enum_validate (CParamSpec *pspec,
		     GValue     *value)
{
  CParamSpecEnum *espec = C_PARAM_SPEC_ENUM (pspec);
  glong oval = value->data[0].v_long;
  
  if (!espec->enum_class ||
      !g_enum_get_value (espec->enum_class, value->data[0].v_long))
    value->data[0].v_long = espec->default_value;
  
  return value->data[0].v_long != oval;
}

static void
param_flags_init (CParamSpec *pspec)
{
  CParamSpecFlags *fspec = C_PARAM_SPEC_FLAGS (pspec);
  
  fspec->flags_class = NULL;
  fspec->default_value = 0;
}

static void
param_flags_finalize (CParamSpec *pspec)
{
  CParamSpecFlags *fspec = C_PARAM_SPEC_FLAGS (pspec);
  CParamSpecClass *parent_class = g_type_class_peek (g_type_parent (C_TYPE_PARAM_FLAGS));
  
  if (fspec->flags_class)
    {
      g_type_class_unref (fspec->flags_class);
      fspec->flags_class = NULL;
    }
  
  parent_class->finalize (pspec);
}

static void
param_flags_set_default (CParamSpec *pspec,
			 GValue     *value)
{
  value->data[0].v_ulong = C_PARAM_SPEC_FLAGS (pspec)->default_value;
}

static gboolean
param_flags_is_valid (CParamSpec   *pspec,
                      const GValue *value)
{
  CParamSpecFlags *fspec = C_PARAM_SPEC_FLAGS (pspec);
  gulong oval = value->data[0].v_ulong;
  
  return (oval & ~fspec->flags_class->mask) == 0;
}
static gboolean
param_flags_validate (CParamSpec *pspec,
		      GValue     *value)
{
  CParamSpecFlags *fspec = C_PARAM_SPEC_FLAGS (pspec);
  gulong oval = value->data[0].v_ulong;
  
  if (fspec->flags_class)
    value->data[0].v_ulong &= fspec->flags_class->mask;
  else
    value->data[0].v_ulong = fspec->default_value;
  
  return value->data[0].v_ulong != oval;
}

static void
param_float_init (CParamSpec *pspec)
{
  CParamSpecFloat *fspec = C_PARAM_SPEC_FLOAT (pspec);
  
  fspec->minimum = -G_MAXFLOAT;
  fspec->maximum = G_MAXFLOAT;
  fspec->default_value = 0;
  fspec->epsilon = G_FLOAT_EPSILON;
}

static void
param_float_set_default (CParamSpec *pspec,
			 GValue     *value)
{
  value->data[0].v_float = C_PARAM_SPEC_FLOAT (pspec)->default_value;
}

static gboolean
param_float_is_valid (CParamSpec   *pspec,
                      const GValue *value)
{
  CParamSpecFloat *fspec = C_PARAM_SPEC_FLOAT (pspec);
  gfloat oval = value->data[0].v_float;
  
  return fspec->minimum <= oval && oval <= fspec->maximum;
}

static gboolean
param_float_validate (CParamSpec *pspec,
		      GValue     *value)
{
  CParamSpecFloat *fspec = C_PARAM_SPEC_FLOAT (pspec);
  gfloat oval = value->data[0].v_float;
  
  value->data[0].v_float = CLAMP (value->data[0].v_float, fspec->minimum, fspec->maximum);
  
  return value->data[0].v_float != oval;
}

static gint
param_float_values_cmp (CParamSpec   *pspec,
			const GValue *value1,
			const GValue *value2)
{
  gfloat epsilon = C_PARAM_SPEC_FLOAT (pspec)->epsilon;
  
  if (value1->data[0].v_float < value2->data[0].v_float)
    return - (value2->data[0].v_float - value1->data[0].v_float > epsilon);
  else
    return value1->data[0].v_float - value2->data[0].v_float > epsilon;
}

static void
param_double_init (CParamSpec *pspec)
{
  CParamSpecDouble *dspec = C_PARAM_SPEC_DOUBLE (pspec);
  
  dspec->minimum = -G_MAXDOUBLE;
  dspec->maximum = G_MAXDOUBLE;
  dspec->default_value = 0;
  dspec->epsilon = G_DOUBLE_EPSILON;
}

static void
param_double_set_default (CParamSpec *pspec,
			  GValue     *value)
{
  value->data[0].v_double = C_PARAM_SPEC_DOUBLE (pspec)->default_value;
}

static gboolean
param_double_is_valid (CParamSpec   *pspec,
                       const GValue *value)
{
  CParamSpecDouble *dspec = C_PARAM_SPEC_DOUBLE (pspec);
  gdouble oval = value->data[0].v_double;
  
  return dspec->minimum <= oval && oval <= dspec->maximum;
}

static gboolean
param_double_validate (CParamSpec *pspec,
		       GValue     *value)
{
  CParamSpecDouble *dspec = C_PARAM_SPEC_DOUBLE (pspec);
  gdouble oval = value->data[0].v_double;
  
  value->data[0].v_double = CLAMP (value->data[0].v_double, dspec->minimum, dspec->maximum);
  
  return value->data[0].v_double != oval;
}

static gint
param_double_values_cmp (CParamSpec   *pspec,
			 const GValue *value1,
			 const GValue *value2)
{
  gdouble epsilon = C_PARAM_SPEC_DOUBLE (pspec)->epsilon;
  
  if (value1->data[0].v_double < value2->data[0].v_double)
    return - (value2->data[0].v_double - value1->data[0].v_double > epsilon);
  else
    return value1->data[0].v_double - value2->data[0].v_double > epsilon;
}

static void
param_string_init (CParamSpec *pspec)
{
  CParamSpecString *sspec = C_PARAM_SPEC_STRING (pspec);
  
  sspec->default_value = NULL;
  sspec->cset_first = NULL;
  sspec->cset_nth = NULL;
  sspec->substitutor = '_';
  sspec->null_fold_if_empty = FALSE;
  sspec->ensure_non_null = FALSE;
}

static void
param_string_finalize (CParamSpec *pspec)
{
  CParamSpecString *sspec = C_PARAM_SPEC_STRING (pspec);
  CParamSpecClass *parent_class = g_type_class_peek (g_type_parent (C_TYPE_PARAM_STRING));
  
  g_free (sspec->default_value);
  g_free (sspec->cset_first);
  g_free (sspec->cset_nth);
  sspec->default_value = NULL;
  sspec->cset_first = NULL;
  sspec->cset_nth = NULL;
  
  parent_class->finalize (pspec);
}

static void
param_string_set_default (CParamSpec *pspec,
			  GValue     *value)
{
  value->data[0].v_pointer = g_strdup (C_PARAM_SPEC_STRING (pspec)->default_value);
}

static gboolean
param_string_validate (CParamSpec *pspec,
		       GValue     *value)
{
  CParamSpecString *sspec = C_PARAM_SPEC_STRING (pspec);
  gchar *string = value->data[0].v_pointer;
  guint changed = 0;
  
  if (string && string[0])
    {
      gchar *s;
      
      if (sspec->cset_first && !strchr (sspec->cset_first, string[0]))
	{
          if (value->data[1].v_uint & G_VALUE_NOCOPY_CONTENTS)
            {
              value->data[0].v_pointer = g_strdup (string);
              string = value->data[0].v_pointer;
              value->data[1].v_uint &= ~G_VALUE_NOCOPY_CONTENTS;
            }
	  string[0] = sspec->substitutor;
	  changed++;
	}
      if (sspec->cset_nth)
	for (s = string + 1; *s; s++)
	  if (!strchr (sspec->cset_nth, *s))
	    {
              if (value->data[1].v_uint & G_VALUE_NOCOPY_CONTENTS)
                {
                  value->data[0].v_pointer = g_strdup (string);
                  s = (gchar*) value->data[0].v_pointer + (s - string);
                  string = value->data[0].v_pointer;
                  value->data[1].v_uint &= ~G_VALUE_NOCOPY_CONTENTS;
                }
	      *s = sspec->substitutor;
	      changed++;
	    }
    }
  if (sspec->null_fold_if_empty && string && string[0] == 0)
    {
      if (!(value->data[1].v_uint & G_VALUE_NOCOPY_CONTENTS))
        g_free (value->data[0].v_pointer);
      else
        value->data[1].v_uint &= ~G_VALUE_NOCOPY_CONTENTS;
      value->data[0].v_pointer = NULL;
      changed++;
      string = value->data[0].v_pointer;
    }
  if (sspec->ensure_non_null && !string)
    {
      value->data[1].v_uint &= ~G_VALUE_NOCOPY_CONTENTS;
      value->data[0].v_pointer = g_strdup ("");
      changed++;
      string = value->data[0].v_pointer;
    }

  return changed;
}

static gboolean
param_string_is_valid (CParamSpec   *pspec,
                       const GValue *value)
{
  CParamSpecString *sspec = C_PARAM_SPEC_STRING (pspec);
  gboolean ret = TRUE;

  if (sspec->cset_first != NULL || sspec->cset_nth != NULL ||
      sspec->ensure_non_null || sspec->null_fold_if_empty)
    {
      GValue tmp_value = G_VALUE_INIT;

      g_value_init (&tmp_value, G_VALUE_TYPE (value));
      g_value_copy (value, &tmp_value);

      ret = !param_string_validate (pspec, &tmp_value);

      g_value_unset (&tmp_value);
    }

  return ret;
}

static gint
param_string_values_cmp (CParamSpec   *pspec,
			 const GValue *value1,
			 const GValue *value2)
{
  if (!value1->data[0].v_pointer)
    return value2->data[0].v_pointer != NULL ? -1 : 0;
  else if (!value2->data[0].v_pointer)
    return value1->data[0].v_pointer != NULL;
  else
    return strcmp (value1->data[0].v_pointer, value2->data[0].v_pointer);
}

static void
param_param_init (CParamSpec *pspec)
{
  /* CParamSpecParam *spec = C_PARAM_SPEC_PARAM (pspec); */
}

static void
param_param_set_default (CParamSpec *pspec,
			 GValue     *value)
{
  value->data[0].v_pointer = NULL;
}

static gboolean
param_param_is_valid (CParamSpec   *pspec,
                      const GValue *value)
{
  CParamSpec *param = value->data[0].v_pointer;

  if (param == NULL)
    return FALSE;

  return g_value_type_compatible (C_PARAM_SPEC_TYPE (param), C_PARAM_SPEC_VALUE_TYPE (pspec));
}

static gboolean
param_param_validate (CParamSpec *pspec,
		      GValue     *value)
{
  /* CParamSpecParam *spec = C_PARAM_SPEC_PARAM (pspec); */
  CParamSpec *param = value->data[0].v_pointer;
  guint changed = 0;
  
  if (param && !g_value_type_compatible (C_PARAM_SPEC_TYPE (param), C_PARAM_SPEC_VALUE_TYPE (pspec)))
    {
      c_param_spec_unref (param);
      value->data[0].v_pointer = NULL;
      changed++;
    }
  
  return changed;
}

static void
param_boxed_init (CParamSpec *pspec)
{
  /* CParamSpecBoxed *bspec = C_PARAM_SPEC_BOXED (pspec); */
}

static void
param_boxed_set_default (CParamSpec *pspec,
			 GValue     *value)
{
  value->data[0].v_pointer = NULL;
}

static gint
param_boxed_values_cmp (CParamSpec    *pspec,
			 const GValue *value1,
			 const GValue *value2)
{
  guint8 *p1 = value1->data[0].v_pointer;
  guint8 *p2 = value2->data[0].v_pointer;

  /* not much to compare here, try to at least provide stable lesser/greater result */

  return p1 < p2 ? -1 : p1 > p2;
}

static void
param_pointer_init (CParamSpec *pspec)
{
  /* CParamSpecPointer *spec = C_PARAM_SPEC_POINTER (pspec); */
}

static void
param_pointer_set_default (CParamSpec *pspec,
			   GValue     *value)
{
  value->data[0].v_pointer = NULL;
}

static gint
param_pointer_values_cmp (CParamSpec   *pspec,
			  const GValue *value1,
			  const GValue *value2)
{
  guint8 *p1 = value1->data[0].v_pointer;
  guint8 *p2 = value2->data[0].v_pointer;

  /* not much to compare here, try to at least provide stable lesser/greater result */

  return p1 < p2 ? -1 : p1 > p2;
}

static void
param_value_array_init (CParamSpec *pspec)
{
  CParamSpecValueArray *aspec = C_PARAM_SPEC_VALUE_ARRAY (pspec);

  aspec->element_spec = NULL;
  aspec->fixed_n_elements = 0; /* disable */
}

static inline guint
value_array_ensure_size (GValueArray *value_array,
			 guint        fixed_n_elements)
{
  guint changed = 0;

  if (fixed_n_elements)
    {
      while (value_array->n_values < fixed_n_elements)
	{
	  g_value_array_append (value_array, NULL);
	  changed++;
	}
      while (value_array->n_values > fixed_n_elements)
	{
	  g_value_array_remove (value_array, value_array->n_values - 1);
	  changed++;
	}
    }
  return changed;
}

static void
param_value_array_finalize (CParamSpec *pspec)
{
  CParamSpecValueArray *aspec = C_PARAM_SPEC_VALUE_ARRAY (pspec);
  CParamSpecClass *parent_class = g_type_class_peek (g_type_parent (C_TYPE_PARAM_VALUE_ARRAY));

  if (aspec->element_spec)
    {
      c_param_spec_unref (aspec->element_spec);
      aspec->element_spec = NULL;
    }

  parent_class->finalize (pspec);
}

static void
param_value_array_set_default (CParamSpec *pspec,
			       GValue     *value)
{
  CParamSpecValueArray *aspec = C_PARAM_SPEC_VALUE_ARRAY (pspec);

  if (!value->data[0].v_pointer && aspec->fixed_n_elements)
    value->data[0].v_pointer = g_value_array_new (aspec->fixed_n_elements);

  if (value->data[0].v_pointer)
    {
      /* g_value_reset (value);  already done */
      value_array_ensure_size (value->data[0].v_pointer, aspec->fixed_n_elements);
    }
}

static gboolean
param_value_array_validate (CParamSpec *pspec,
			    GValue     *value)
{
  CParamSpecValueArray *aspec = C_PARAM_SPEC_VALUE_ARRAY (pspec);
  GValueArray *value_array = value->data[0].v_pointer;
  guint changed = 0;

  if (!value->data[0].v_pointer && aspec->fixed_n_elements)
    value_array = value->data[0].v_pointer = g_value_array_new (aspec->fixed_n_elements);

  if (value->data[0].v_pointer)
    {
      /* ensure array size validity */
      changed += value_array_ensure_size (value_array, aspec->fixed_n_elements);
      
      /* ensure array values validity against a present element spec */
      if (aspec->element_spec)
	{
	  CParamSpec *element_spec = aspec->element_spec;
	  guint i;
	  
	  for (i = 0; i < value_array->n_values; i++)
	    {
	      GValue *element = value_array->values + i;
	      
	      /* need to fixup value type, or ensure that the array value is initialized at all */
	      if (!g_value_type_compatible (G_VALUE_TYPE (element), C_PARAM_SPEC_VALUE_TYPE (element_spec)))
		{
		  if (G_VALUE_TYPE (element) != 0)
		    g_value_unset (element);
		  g_value_init (element, C_PARAM_SPEC_VALUE_TYPE (element_spec));
		  c_param_value_set_default (element_spec, element);
		  changed++;
		}
              else
                {
	          /* validate array value against element_spec */
	          changed += c_param_value_validate (element_spec, element);
                }
	    }
	}
    }

  return changed;
}

static gint
param_value_array_values_cmp (CParamSpec   *pspec,
			      const GValue *value1,
			      const GValue *value2)
{
  CParamSpecValueArray *aspec = C_PARAM_SPEC_VALUE_ARRAY (pspec);
  GValueArray *value_array1 = value1->data[0].v_pointer;
  GValueArray *value_array2 = value2->data[0].v_pointer;

  if (!value_array1 || !value_array2)
    return value_array2 ? -1 : value_array1 != value_array2;

  if (value_array1->n_values != value_array2->n_values)
    return value_array1->n_values < value_array2->n_values ? -1 : 1;
  else if (!aspec->element_spec)
    {
      /* we need an element specification for comparisons, so there's not much
       * to compare here, try to at least provide stable lesser/greater result
       */
      return value_array1->n_values < value_array2->n_values ? -1 : value_array1->n_values > value_array2->n_values;
    }
  else /* value_array1->n_values == value_array2->n_values */
    {
      guint i;

      for (i = 0; i < value_array1->n_values; i++)
	{
	  GValue *element1 = value_array1->values + i;
	  GValue *element2 = value_array2->values + i;
	  gint cmp;

	  /* need corresponding element types, provide stable result otherwise */
	  if (G_VALUE_TYPE (element1) != G_VALUE_TYPE (element2))
	    return G_VALUE_TYPE (element1) < G_VALUE_TYPE (element2) ? -1 : 1;
	  cmp = c_param_values_cmp (aspec->element_spec, element1, element2);
	  if (cmp)
	    return cmp;
	}
      return 0;
    }
}

static void
param_object_init (CParamSpec *pspec)
{
  /* CParamSpecObject *ospec = C_PARAM_SPEC_OBJECT (pspec); */
}

static void
param_object_set_default (CParamSpec *pspec,
			  GValue     *value)
{
  value->data[0].v_pointer = NULL;
}

static gboolean
param_object_is_valid (CParamSpec   *pspec,
                       const GValue *value)
{
  CParamSpecObject *ospec = C_PARAM_SPEC_OBJECT (pspec);
  GObject *object = value->data[0].v_pointer;

  return object &&
         g_value_type_compatible (G_OBJECT_TYPE (object), C_PARAM_SPEC_VALUE_TYPE (ospec));
}

static gboolean
param_object_validate (CParamSpec *pspec,
		       GValue     *value)
{
  CParamSpecObject *ospec = C_PARAM_SPEC_OBJECT (pspec);
  GObject *object = value->data[0].v_pointer;
  guint changed = 0;
  
  if (object && !g_value_type_compatible (G_OBJECT_TYPE (object), C_PARAM_SPEC_VALUE_TYPE (ospec)))
    {
      g_object_unref (object);
      value->data[0].v_pointer = NULL;
      changed++;
    }
  
  return changed;
}

static gint
param_object_values_cmp (CParamSpec   *pspec,
			 const GValue *value1,
			 const GValue *value2)
{
  guint8 *p1 = value1->data[0].v_pointer;
  guint8 *p2 = value2->data[0].v_pointer;

  /* not much to compare here, try to at least provide stable lesser/greater result */

  return p1 < p2 ? -1 : p1 > p2;
}

static void
param_override_init (CParamSpec *pspec)
{
  /* CParamSpecOverride *ospec = C_PARAM_SPEC_OVERRIDE (pspec); */
}

static void
param_override_finalize (CParamSpec *pspec)
{
  CParamSpecOverride *ospec = C_PARAM_SPEC_OVERRIDE (pspec);
  CParamSpecClass *parent_class = g_type_class_peek (g_type_parent (C_TYPE_PARAM_OVERRIDE));
  
  if (ospec->overridden)
    {
      c_param_spec_unref (ospec->overridden);
      ospec->overridden = NULL;
    }
  
  parent_class->finalize (pspec);
}

static void
param_override_set_default (CParamSpec *pspec,
			    GValue     *value)
{
  CParamSpecOverride *ospec = C_PARAM_SPEC_OVERRIDE (pspec);

  c_param_value_set_default (ospec->overridden, value);
}

static gboolean
param_override_is_valid (CParamSpec   *pspec,
			 const GValue *value)
{
  CParamSpecOverride *ospec = C_PARAM_SPEC_OVERRIDE (pspec);

  return c_param_value_is_valid (ospec->overridden, value);
}

static gboolean
param_override_validate (CParamSpec *pspec,
			 GValue     *value)
{
  CParamSpecOverride *ospec = C_PARAM_SPEC_OVERRIDE (pspec);
  
  return c_param_value_validate (ospec->overridden, value);
}

static gint
param_override_values_cmp (CParamSpec   *pspec,
			   const GValue *value1,
			   const GValue *value2)
{
  CParamSpecOverride *ospec = C_PARAM_SPEC_OVERRIDE (pspec);

  return c_param_values_cmp (ospec->overridden, value1, value2);
}

static void
param_gtype_init (CParamSpec *pspec)
{
}

static void
param_gtype_set_default (CParamSpec *pspec,
			 GValue     *value)
{
  CParamSpecGType *tspec = C_PARAM_SPEC_GTYPE (pspec);

  value->data[0].v_pointer = GTYPE_TO_POINTER (tspec->is_a_type);
}

static gboolean
param_gtype_is_valid (CParamSpec   *pspec,
                      const GValue *value)
{
  CParamSpecGType *tspec = C_PARAM_SPEC_GTYPE (pspec);
  GType gtype = GPOINTER_TO_TYPE (value->data[0].v_pointer);
  
  return tspec->is_a_type == G_TYPE_NONE ||
         g_type_is_a (gtype, tspec->is_a_type);
}

static gboolean
param_gtype_validate (CParamSpec *pspec,
		      GValue     *value)
{
  CParamSpecGType *tspec = C_PARAM_SPEC_GTYPE (pspec);
  GType gtype = GPOINTER_TO_TYPE (value->data[0].v_pointer);
  guint changed = 0;
  
  if (tspec->is_a_type != G_TYPE_NONE && !g_type_is_a (gtype, tspec->is_a_type))
    {
      value->data[0].v_pointer = GTYPE_TO_POINTER (tspec->is_a_type);
      changed++;
    }
  
  return changed;
}

static gint
param_gtype_values_cmp (CParamSpec   *pspec,
			const GValue *value1,
			const GValue *value2)
{
  GType p1 = GPOINTER_TO_TYPE (value1->data[0].v_pointer);
  GType p2 = GPOINTER_TO_TYPE (value2->data[0].v_pointer);

  /* not much to compare here, try to at least provide stable lesser/greater result */

  return p1 < p2 ? -1 : p1 > p2;
}

static void
param_variant_init (CParamSpec *pspec)
{
  CParamSpecVariant *vspec = C_PARAM_SPEC_VARIANT (pspec);

  vspec->type = NULL;
  vspec->default_value = NULL;
}

static void
param_variant_finalize (CParamSpec *pspec)
{
  CParamSpecVariant *vspec = C_PARAM_SPEC_VARIANT (pspec);
  CParamSpecClass *parent_class = g_type_class_peek (g_type_parent (C_TYPE_PARAM_VARIANT));

  if (vspec->default_value)
    g_variant_unref (vspec->default_value);
  g_variant_type_free (vspec->type);

  parent_class->finalize (pspec);
}

static void
param_variant_set_default (CParamSpec *pspec,
                           GValue     *value)
{
  value->data[0].v_pointer = C_PARAM_SPEC_VARIANT (pspec)->default_value;
  value->data[1].v_uint |= G_VALUE_NOCOPY_CONTENTS;
}

static gboolean
param_variant_is_valid (CParamSpec   *pspec,
                        const GValue *value)
{
  CParamSpecVariant *vspec = C_PARAM_SPEC_VARIANT (pspec);
  GVariant *variant = value->data[0].v_pointer;

  if (variant == NULL)
    return vspec->default_value == NULL;
  else
    return g_variant_is_of_type (variant, vspec->type);
}

static gboolean
param_variant_validate (CParamSpec *pspec,
                        GValue     *value)
{
  CParamSpecVariant *vspec = C_PARAM_SPEC_VARIANT (pspec);
  GVariant *variant = value->data[0].v_pointer;

  if ((variant == NULL && vspec->default_value != NULL) ||
      (variant != NULL && !g_variant_is_of_type (variant, vspec->type)))
    {
      c_param_value_set_default (pspec, value);
      return TRUE;
    }

  return FALSE;
}

/* g_variant_compare() can only be used with scalar types. */
static gboolean
variant_is_incomparable (GVariant *v)
{
  GVariantClass v_class = g_variant_classify (v);

  return (v_class == G_VARIANT_CLASS_HANDLE ||
          v_class == G_VARIANT_CLASS_VARIANT ||
          v_class ==  G_VARIANT_CLASS_MAYBE||
          v_class == G_VARIANT_CLASS_ARRAY ||
          v_class ==  G_VARIANT_CLASS_TUPLE ||
          v_class == G_VARIANT_CLASS_DICT_ENTRY);
}

static gint
param_variant_values_cmp (CParamSpec   *pspec,
                          const GValue *value1,
                          const GValue *value2)
{
  GVariant *v1 = value1->data[0].v_pointer;
  GVariant *v2 = value2->data[0].v_pointer;

  if (v1 == NULL && v2 == NULL)
    return 0;
  else if (v1 == NULL && v2 != NULL)
    return -1;
  else if (v1 != NULL && v2 == NULL)
    return 1;

  if (!g_variant_type_equal (g_variant_get_type (v1), g_variant_get_type (v2)) ||
      variant_is_incomparable (v1) ||
      variant_is_incomparable (v2))
    return g_variant_equal (v1, v2) ? 0 : (v1 < v2 ? -1 : 1);

  return g_variant_compare (v1, v2);
}

/* --- type initialization --- */

#define set_is_valid_vfunc(type,func) { \
  CParamSpecClass *class = g_type_class_ref (type); \
  class->value_is_valid = func; \
  g_type_class_unref (class); \
}

GType *c_param_spec_types = NULL;

void
_c_param_spec_types_init (void)	
{
  const guint n_types = 23;
  GType type, *spec_types;
#ifndef G_DISABLE_ASSERT
  GType *spec_types_bound;
#endif

  c_param_spec_types = g_new0 (GType, n_types);
  spec_types = c_param_spec_types;
#ifndef G_DISABLE_ASSERT
  spec_types_bound = c_param_spec_types + n_types;
#endif
  
  /* C_TYPE_PARAM_CHAR
   */
  {
    const CParamSpecTypeInfo pspec_info = {
      sizeof (CParamSpecChar),	/* instance_size */
      16,			/* n_preallocs */
      param_char_init,		/* instance_init */
      G_TYPE_CHAR,		/* value_type */
      NULL,			/* finalize */
      param_char_set_default,	/* value_set_default */
      param_char_validate,	/* value_validate */
      param_int_values_cmp,	/* values_cmp */
    };
    type = c_param_type_register_static (g_intern_static_string ("CParamChar"), &pspec_info);
    set_is_valid_vfunc (type, param_char_is_valid);
    *spec_types++ = type;
    g_assert (type == C_TYPE_PARAM_CHAR);
  }
  
  /* C_TYPE_PARAM_UCHAR
   */
  {
    const CParamSpecTypeInfo pspec_info = {
      sizeof (CParamSpecUChar), /* instance_size */
      16,                       /* n_preallocs */
      param_uchar_init,         /* instance_init */
      G_TYPE_UCHAR,		/* value_type */
      NULL,			/* finalize */
      param_uchar_set_default,	/* value_set_default */
      param_uchar_validate,	/* value_validate */
      param_uint_values_cmp,	/* values_cmp */
    };
    type = c_param_type_register_static (g_intern_static_string ("CParamUChar"), &pspec_info);
    set_is_valid_vfunc (type, param_uchar_is_valid);
    *spec_types++ = type;
    g_assert (type == C_TYPE_PARAM_UCHAR);
  }
  
  /* C_TYPE_PARAM_BOOLEAN
   */
  {
    const CParamSpecTypeInfo pspec_info = {
      sizeof (CParamSpecBoolean), /* instance_size */
      16,                         /* n_preallocs */
      NULL,			  /* instance_init */
      G_TYPE_BOOLEAN,             /* value_type */
      NULL,                       /* finalize */
      param_boolean_set_default,  /* value_set_default */
      param_boolean_validate,     /* value_validate */
      param_int_values_cmp,       /* values_cmp */
    };
    type = c_param_type_register_static (g_intern_static_string ("CParamBoolean"), &pspec_info);
    set_is_valid_vfunc (type, param_boolean_is_valid);
    *spec_types++ = type;
    g_assert (type == C_TYPE_PARAM_BOOLEAN);
  }
  
  /* C_TYPE_PARAM_INT
   */
  {
    const CParamSpecTypeInfo pspec_info = {
      sizeof (CParamSpecInt),   /* instance_size */
      16,                       /* n_preallocs */
      param_int_init,           /* instance_init */
      G_TYPE_INT,		/* value_type */
      NULL,			/* finalize */
      param_int_set_default,	/* value_set_default */
      param_int_validate,	/* value_validate */
      param_int_values_cmp,	/* values_cmp */
    };
    type = c_param_type_register_static (g_intern_static_string ("CParamInt"), &pspec_info);
    set_is_valid_vfunc (type, param_int_is_valid);
    *spec_types++ = type;
    g_assert (type == C_TYPE_PARAM_INT);
  }
  
  /* C_TYPE_PARAM_UINT
   */
  {
    const CParamSpecTypeInfo pspec_info = {
      sizeof (CParamSpecUInt),  /* instance_size */
      16,                       /* n_preallocs */
      param_uint_init,          /* instance_init */
      G_TYPE_UINT,		/* value_type */
      NULL,			/* finalize */
      param_uint_set_default,	/* value_set_default */
      param_uint_validate,	/* value_validate */
      param_uint_values_cmp,	/* values_cmp */
    };
    type = c_param_type_register_static (g_intern_static_string ("CParamUInt"), &pspec_info);
    set_is_valid_vfunc (type, param_uint_is_valid);
    *spec_types++ = type;
    g_assert (type == C_TYPE_PARAM_UINT);
  }
  
  /* C_TYPE_PARAM_LONG
   */
  {
    const CParamSpecTypeInfo pspec_info = {
      sizeof (CParamSpecLong),  /* instance_size */
      16,                       /* n_preallocs */
      param_long_init,          /* instance_init */
      G_TYPE_LONG,		/* value_type */
      NULL,			/* finalize */
      param_long_set_default,	/* value_set_default */
      param_long_validate,	/* value_validate */
      param_long_values_cmp,	/* values_cmp */
    };
    type = c_param_type_register_static (g_intern_static_string ("CParamLong"), &pspec_info);
    set_is_valid_vfunc (type, param_long_is_valid);
    *spec_types++ = type;
    g_assert (type == C_TYPE_PARAM_LONG);
  }
  
  /* C_TYPE_PARAM_ULONG
   */
  {
    const CParamSpecTypeInfo pspec_info = {
      sizeof (CParamSpecULong), /* instance_size */
      16,                       /* n_preallocs */
      param_ulong_init,         /* instance_init */
      G_TYPE_ULONG,		/* value_type */
      NULL,			/* finalize */
      param_ulong_set_default,	/* value_set_default */
      param_ulong_validate,	/* value_validate */
      param_ulong_values_cmp,	/* values_cmp */
    };
    type = c_param_type_register_static (g_intern_static_string ("CParamULong"), &pspec_info);
    set_is_valid_vfunc (type, param_ulong_is_valid);
    *spec_types++ = type;
    g_assert (type == C_TYPE_PARAM_ULONG);
  }

  /* C_TYPE_PARAM_INT64
   */
  {
    const CParamSpecTypeInfo pspec_info = {
      sizeof (CParamSpecInt64),  /* instance_size */
      16,                       /* n_preallocs */
      param_int64_init,         /* instance_init */
      G_TYPE_INT64,		/* value_type */
      NULL,			/* finalize */
      param_int64_set_default,	/* value_set_default */
      param_int64_validate,	/* value_validate */
      param_int64_values_cmp,	/* values_cmp */
    };
    type = c_param_type_register_static (g_intern_static_string ("CParamInt64"), &pspec_info);
    set_is_valid_vfunc (type, param_int64_is_valid);
    *spec_types++ = type;
    g_assert (type == C_TYPE_PARAM_INT64);
  }
  
  /* C_TYPE_PARAM_UINT64
   */
  {
    const CParamSpecTypeInfo pspec_info = {
      sizeof (CParamSpecUInt64), /* instance_size */
      16,                       /* n_preallocs */
      param_uint64_init,        /* instance_init */
      G_TYPE_UINT64,		/* value_type */
      NULL,			/* finalize */
      param_uint64_set_default,	/* value_set_default */
      param_uint64_validate,	/* value_validate */
      param_uint64_values_cmp,	/* values_cmp */
    };
    type = c_param_type_register_static (g_intern_static_string ("CParamUInt64"), &pspec_info);
    set_is_valid_vfunc (type, param_uint64_is_valid);
    *spec_types++ = type;
    g_assert (type == C_TYPE_PARAM_UINT64);
  }

  /* C_TYPE_PARAM_UNICHAR
   */
  {
    const CParamSpecTypeInfo pspec_info = {
      sizeof (CParamSpecUnichar), /* instance_size */
      16,                        /* n_preallocs */
      param_unichar_init,	 /* instance_init */
      G_TYPE_UINT,		 /* value_type */
      NULL,			 /* finalize */
      param_unichar_set_default, /* value_set_default */
      param_unichar_validate,	 /* value_validate */
      param_unichar_values_cmp,	 /* values_cmp */
    };
    type = c_param_type_register_static (g_intern_static_string ("CParamUnichar"), &pspec_info);
    set_is_valid_vfunc (type, param_unichar_is_valid);
    *spec_types++ = type;
    g_assert (type == C_TYPE_PARAM_UNICHAR);
  }

 /* C_TYPE_PARAM_ENUM
   */
  {
    const CParamSpecTypeInfo pspec_info = {
      sizeof (CParamSpecEnum),  /* instance_size */
      16,                       /* n_preallocs */
      param_enum_init,          /* instance_init */
      G_TYPE_ENUM,		/* value_type */
      param_enum_finalize,	/* finalize */
      param_enum_set_default,	/* value_set_default */
      param_enum_validate,	/* value_validate */
      param_long_values_cmp,	/* values_cmp */
    };
    type = c_param_type_register_static (g_intern_static_string ("CParamEnum"), &pspec_info);
    set_is_valid_vfunc (type, param_enum_is_valid);
    *spec_types++ = type;
    g_assert (type == C_TYPE_PARAM_ENUM);
  }
  
  /* C_TYPE_PARAM_FLAGS
   */
  {
    const CParamSpecTypeInfo pspec_info = {
      sizeof (CParamSpecFlags),	/* instance_size */
      16,			/* n_preallocs */
      param_flags_init,		/* instance_init */
      G_TYPE_FLAGS,		/* value_type */
      param_flags_finalize,	/* finalize */
      param_flags_set_default,	/* value_set_default */
      param_flags_validate,	/* value_validate */
      param_ulong_values_cmp,	/* values_cmp */
    };
    type = c_param_type_register_static (g_intern_static_string ("CParamFlags"), &pspec_info);
    set_is_valid_vfunc (type, param_flags_is_valid);
    *spec_types++ = type;
    g_assert (type == C_TYPE_PARAM_FLAGS);
  }
  
  /* C_TYPE_PARAM_FLOAT
   */
  {
    const CParamSpecTypeInfo pspec_info = {
      sizeof (CParamSpecFloat), /* instance_size */
      16,                       /* n_preallocs */
      param_float_init,         /* instance_init */
      G_TYPE_FLOAT,		/* value_type */
      NULL,			/* finalize */
      param_float_set_default,	/* value_set_default */
      param_float_validate,	/* value_validate */
      param_float_values_cmp,	/* values_cmp */
    };
    type = c_param_type_register_static (g_intern_static_string ("CParamFloat"), &pspec_info);
    set_is_valid_vfunc (type, param_float_is_valid);
    *spec_types++ = type;
    g_assert (type == C_TYPE_PARAM_FLOAT);
  }
  
  /* C_TYPE_PARAM_DOUBLE
   */
  {
    const CParamSpecTypeInfo pspec_info = {
      sizeof (CParamSpecDouble),	/* instance_size */
      16,				/* n_preallocs */
      param_double_init,		/* instance_init */
      G_TYPE_DOUBLE,			/* value_type */
      NULL,				/* finalize */
      param_double_set_default,		/* value_set_default */
      param_double_validate,		/* value_validate */
      param_double_values_cmp,		/* values_cmp */
    };
    type = c_param_type_register_static (g_intern_static_string ("CParamDouble"), &pspec_info);
    set_is_valid_vfunc (type, param_double_is_valid);
    *spec_types++ = type;
    g_assert (type == C_TYPE_PARAM_DOUBLE);
  }
  
  /* C_TYPE_PARAM_STRING
   */
  {
    const CParamSpecTypeInfo pspec_info = {
      sizeof (CParamSpecString),	/* instance_size */
      16,				/* n_preallocs */
      param_string_init,		/* instance_init */
      G_TYPE_STRING,			/* value_type */
      param_string_finalize,		/* finalize */
      param_string_set_default,		/* value_set_default */
      param_string_validate,		/* value_validate */
      param_string_values_cmp,		/* values_cmp */
    };
    type = c_param_type_register_static (g_intern_static_string ("CParamString"), &pspec_info);
    set_is_valid_vfunc (type, param_string_is_valid);
    *spec_types++ = type;
    g_assert (type == C_TYPE_PARAM_STRING);
  }
  
  /* C_TYPE_PARAM_PARAM
   */
  {
    const CParamSpecTypeInfo pspec_info = {
      sizeof (CParamSpecParam),	/* instance_size */
      16,			/* n_preallocs */
      param_param_init,		/* instance_init */
      C_TYPE_PARAM,		/* value_type */
      NULL,			/* finalize */
      param_param_set_default,	/* value_set_default */
      param_param_validate,	/* value_validate */
      param_pointer_values_cmp,	/* values_cmp */
    };
    type = c_param_type_register_static (g_intern_static_string ("CParamParam"), &pspec_info);
    set_is_valid_vfunc (type, param_param_is_valid);
    *spec_types++ = type;
    g_assert (type == C_TYPE_PARAM_PARAM);
  }
  
  /* C_TYPE_PARAM_BOXED
   */
  {
    const CParamSpecTypeInfo pspec_info = {
      sizeof (CParamSpecBoxed),	/* instance_size */
      4,			/* n_preallocs */
      param_boxed_init,		/* instance_init */
      G_TYPE_BOXED,		/* value_type */
      NULL,			/* finalize */
      param_boxed_set_default,	/* value_set_default */
      NULL,                   	/* value_validate */
      param_boxed_values_cmp,	/* values_cmp */
    };
    type = c_param_type_register_static (g_intern_static_string ("CParamBoxed"), &pspec_info);
    *spec_types++ = type;
    g_assert (type == C_TYPE_PARAM_BOXED);
  }

  /* C_TYPE_PARAM_POINTER
   */
  {
    const CParamSpecTypeInfo pspec_info = {
      sizeof (CParamSpecPointer),  /* instance_size */
      0,                           /* n_preallocs */
      param_pointer_init,	   /* instance_init */
      G_TYPE_POINTER,  		   /* value_type */
      NULL,			   /* finalize */
      param_pointer_set_default,   /* value_set_default */
      NULL,
      param_pointer_values_cmp,	   /* values_cmp */
    };
    type = c_param_type_register_static (g_intern_static_string ("CParamPointer"), &pspec_info);
    *spec_types++ = type;
    g_assert (type == C_TYPE_PARAM_POINTER);
  }
  
  /* C_TYPE_PARAM_VALUE_ARRAY
   */
  {
    /* const */ CParamSpecTypeInfo pspec_info = {
      sizeof (CParamSpecValueArray),	/* instance_size */
      0,				/* n_preallocs */
      param_value_array_init,		/* instance_init */
      0xdeadbeef,			/* value_type, assigned further down */
      param_value_array_finalize,	/* finalize */
      param_value_array_set_default,	/* value_set_default */
      param_value_array_validate,	/* value_validate */
      param_value_array_values_cmp,	/* values_cmp */
    };
    pspec_info.value_type = G_TYPE_VALUE_ARRAY;
    type = c_param_type_register_static (g_intern_static_string ("CParamValueArray"), &pspec_info);
    *spec_types++ = type;
    g_assert (type == C_TYPE_PARAM_VALUE_ARRAY);
  }

  /* C_TYPE_PARAM_OBJECT
   */
  {
    const CParamSpecTypeInfo pspec_info = {
      sizeof (CParamSpecObject), /* instance_size */
      16,                        /* n_preallocs */
      param_object_init,	 /* instance_init */
      G_TYPE_OBJECT,		 /* value_type */
      NULL,			 /* finalize */
      param_object_set_default,	 /* value_set_default */
      param_object_validate,	 /* value_validate */
      param_object_values_cmp,	 /* values_cmp */
    };
    type = c_param_type_register_static (g_intern_static_string ("CParamObject"), &pspec_info);
    set_is_valid_vfunc (type, param_object_is_valid);
    *spec_types++ = type;
    g_assert (type == C_TYPE_PARAM_OBJECT);
  }

  /* C_TYPE_PARAM_OVERRIDE
   */
  {
    const CParamSpecTypeInfo pspec_info = {
      sizeof (CParamSpecOverride), /* instance_size */
      16,                        /* n_preallocs */
      param_override_init,	 /* instance_init */
      G_TYPE_NONE,		 /* value_type */
      param_override_finalize,	 /* finalize */
      param_override_set_default, /* value_set_default */
      param_override_validate,	  /* value_validate */
      param_override_values_cmp,  /* values_cmp */
    };
    type = c_param_type_register_static (g_intern_static_string ("CParamOverride"), &pspec_info);
    set_is_valid_vfunc (type, param_override_is_valid);
    *spec_types++ = type;
    g_assert (type == C_TYPE_PARAM_OVERRIDE);
  }

  /* C_TYPE_PARAM_GTYPE
   */
  {
    CParamSpecTypeInfo pspec_info = {
      sizeof (CParamSpecGType),	/* instance_size */
      0,			/* n_preallocs */
      param_gtype_init,		/* instance_init */
      0xdeadbeef,		/* value_type, assigned further down */
      NULL,			/* finalize */
      param_gtype_set_default,	/* value_set_default */
      param_gtype_validate,	/* value_validate */
      param_gtype_values_cmp,	/* values_cmp */
    };
    pspec_info.value_type = G_TYPE_GTYPE;
    type = c_param_type_register_static (g_intern_static_string ("CParamGType"), &pspec_info);
    set_is_valid_vfunc (type, param_gtype_is_valid);
    *spec_types++ = type;
    g_assert (type == C_TYPE_PARAM_GTYPE);
  }

  /* C_TYPE_PARAM_VARIANT
   */
  {
    const CParamSpecTypeInfo pspec_info = {
      sizeof (CParamSpecVariant), /* instance_size */
      0,                          /* n_preallocs */
      param_variant_init,         /* instance_init */
      G_TYPE_VARIANT,             /* value_type */
      param_variant_finalize,     /* finalize */
      param_variant_set_default,  /* value_set_default */
      param_variant_validate,     /* value_validate */
      param_variant_values_cmp,   /* values_cmp */
    };
    type = c_param_type_register_static (g_intern_static_string ("CParamVariant"), &pspec_info);
    set_is_valid_vfunc (type, param_variant_is_valid);
    *spec_types++ = type;
    g_assert (type == C_TYPE_PARAM_VARIANT);
  }

  g_assert (spec_types == spec_types_bound);
}

/* --- CParamSpec initialization --- */

/**
 * c_param_spec_char:
 * @name: canonical name of the property specified
 * @nick: (nullable): nick name for the property specified
 * @blurb: (nullable): description of the property specified
 * @minimum: minimum value for the property specified
 * @maximum: maximum value for the property specified
 * @default_value: default value for the property specified
 * @flags: flags for the property specified
 *
 * Creates a new #CParamSpecChar instance specifying a %G_TYPE_CHAR property.
 *
 * Returns: (transfer full): a newly created parameter specification
 */
CParamSpec*
c_param_spec_char (const gchar *name,
		   const gchar *nick,
		   const gchar *blurb,
		   gint8	minimum,
		   gint8	maximum,
		   gint8	default_value,
		   CParamFlags	flags)
{
  CParamSpecChar *cspec;

  g_return_val_if_fail (default_value >= minimum && default_value <= maximum, NULL);

  cspec = c_param_spec_internal (C_TYPE_PARAM_CHAR,
				 name,
				 nick,
				 blurb,
				 flags);
  
  cspec->minimum = minimum;
  cspec->maximum = maximum;
  cspec->default_value = default_value;
  
  return C_PARAM_SPEC (cspec);
}

/**
 * c_param_spec_uchar:
 * @name: canonical name of the property specified
 * @nick: (nullable): nick name for the property specified
 * @blurb: (nullable): description of the property specified
 * @minimum: minimum value for the property specified
 * @maximum: maximum value for the property specified
 * @default_value: default value for the property specified
 * @flags: flags for the property specified
 *
 * Creates a new #CParamSpecUChar instance specifying a %G_TYPE_UCHAR property.
 *
 * Returns: (transfer full): a newly created parameter specification
 */
CParamSpec*
c_param_spec_uchar (const gchar *name,
		    const gchar *nick,
		    const gchar *blurb,
		    guint8	 minimum,
		    guint8	 maximum,
		    guint8	 default_value,
		    CParamFlags	 flags)
{
  CParamSpecUChar *uspec;

  g_return_val_if_fail (default_value >= minimum && default_value <= maximum, NULL);

  uspec = c_param_spec_internal (C_TYPE_PARAM_UCHAR,
				 name,
				 nick,
				 blurb,
				 flags);
  
  uspec->minimum = minimum;
  uspec->maximum = maximum;
  uspec->default_value = default_value;
  
  return C_PARAM_SPEC (uspec);
}

/**
 * c_param_spec_boolean:
 * @name: canonical name of the property specified
 * @nick: (nullable): nick name for the property specified
 * @blurb: (nullable): description of the property specified
 * @default_value: default value for the property specified
 * @flags: flags for the property specified
 *
 * Creates a new #CParamSpecBoolean instance specifying a %G_TYPE_BOOLEAN
 * property. In many cases, it may be more appropriate to use an enum with
 * c_param_spec_enum(), both to improve code clarity by using explicitly named
 * values, and to allow for more values to be added in future without breaking
 * API.
 *
 * See c_param_spec_internal() for details on property names.
 *
 * Returns: (transfer full): a newly created parameter specification
 */
CParamSpec*
c_param_spec_boolean (const gchar *name,
		      const gchar *nick,
		      const gchar *blurb,
		      gboolean	   default_value,
		      CParamFlags  flags)
{
  CParamSpecBoolean *bspec;

  g_return_val_if_fail (default_value == TRUE || default_value == FALSE, NULL);

  bspec = c_param_spec_internal (C_TYPE_PARAM_BOOLEAN,
				 name,
				 nick,
				 blurb,
				 flags);
  
  bspec->default_value = default_value;
  
  return C_PARAM_SPEC (bspec);
}

/**
 * c_param_spec_int:
 * @name: canonical name of the property specified
 * @nick: (nullable): nick name for the property specified
 * @blurb: (nullable): description of the property specified
 * @minimum: minimum value for the property specified
 * @maximum: maximum value for the property specified
 * @default_value: default value for the property specified
 * @flags: flags for the property specified
 *
 * Creates a new #CParamSpecInt instance specifying a %G_TYPE_INT property.
 *
 * See c_param_spec_internal() for details on property names.
 *
 * Returns: (transfer full): a newly created parameter specification
 */
CParamSpec*
c_param_spec_int (const gchar *name,
		  const gchar *nick,
		  const gchar *blurb,
		  gint	       minimum,
		  gint	       maximum,
		  gint	       default_value,
		  CParamFlags  flags)
{
  CParamSpecInt *ispec;

  g_return_val_if_fail (default_value >= minimum && default_value <= maximum, NULL);

  ispec = c_param_spec_internal (C_TYPE_PARAM_INT,
				 name,
				 nick,
				 blurb,
				 flags);
  
  ispec->minimum = minimum;
  ispec->maximum = maximum;
  ispec->default_value = default_value;
  
  return C_PARAM_SPEC (ispec);
}

/**
 * c_param_spec_uint:
 * @name: canonical name of the property specified
 * @nick: (nullable): nick name for the property specified
 * @blurb: (nullable): description of the property specified
 * @minimum: minimum value for the property specified
 * @maximum: maximum value for the property specified
 * @default_value: default value for the property specified
 * @flags: flags for the property specified
 *
 * Creates a new #CParamSpecUInt instance specifying a %G_TYPE_UINT property.
 *
 * See c_param_spec_internal() for details on property names.
 *
 * Returns: (transfer full): a newly created parameter specification
 */
CParamSpec*
c_param_spec_uint (const gchar *name,
		   const gchar *nick,
		   const gchar *blurb,
		   guint	minimum,
		   guint	maximum,
		   guint	default_value,
		   CParamFlags	flags)
{
  CParamSpecUInt *uspec;

  g_return_val_if_fail (default_value >= minimum && default_value <= maximum, NULL);

  uspec = c_param_spec_internal (C_TYPE_PARAM_UINT,
				 name,
				 nick,
				 blurb,
				 flags);
  
  uspec->minimum = minimum;
  uspec->maximum = maximum;
  uspec->default_value = default_value;
  
  return C_PARAM_SPEC (uspec);
}

/**
 * c_param_spec_long:
 * @name: canonical name of the property specified
 * @nick: (nullable): nick name for the property specified
 * @blurb: (nullable): description of the property specified
 * @minimum: minimum value for the property specified
 * @maximum: maximum value for the property specified
 * @default_value: default value for the property specified
 * @flags: flags for the property specified
 *
 * Creates a new #CParamSpecLong instance specifying a %G_TYPE_LONG property.
 *
 * See c_param_spec_internal() for details on property names.
 *
 * Returns: (transfer full): a newly created parameter specification
 */
CParamSpec*
c_param_spec_long (const gchar *name,
		   const gchar *nick,
		   const gchar *blurb,
		   glong	minimum,
		   glong	maximum,
		   glong	default_value,
		   CParamFlags	flags)
{
  CParamSpecLong *lspec;

  g_return_val_if_fail (default_value >= minimum && default_value <= maximum, NULL);

  lspec = c_param_spec_internal (C_TYPE_PARAM_LONG,
				 name,
				 nick,
				 blurb,
				 flags);
  
  lspec->minimum = minimum;
  lspec->maximum = maximum;
  lspec->default_value = default_value;
  
  return C_PARAM_SPEC (lspec);
}

/**
 * c_param_spec_ulong:
 * @name: canonical name of the property specified
 * @nick: (nullable): nick name for the property specified
 * @blurb: (nullable): description of the property specified
 * @minimum: minimum value for the property specified
 * @maximum: maximum value for the property specified
 * @default_value: default value for the property specified
 * @flags: flags for the property specified
 *
 * Creates a new #CParamSpecULong instance specifying a %G_TYPE_ULONG
 * property.
 *
 * See c_param_spec_internal() for details on property names.
 *
 * Returns: (transfer full): a newly created parameter specification
 */
CParamSpec*
c_param_spec_ulong (const gchar *name,
		    const gchar *nick,
		    const gchar *blurb,
		    gulong	 minimum,
		    gulong	 maximum,
		    gulong	 default_value,
		    CParamFlags	 flags)
{
  CParamSpecULong *uspec;

  g_return_val_if_fail (default_value >= minimum && default_value <= maximum, NULL);

  uspec = c_param_spec_internal (C_TYPE_PARAM_ULONG,
				 name,
				 nick,
				 blurb,
				 flags);
  
  uspec->minimum = minimum;
  uspec->maximum = maximum;
  uspec->default_value = default_value;
  
  return C_PARAM_SPEC (uspec);
}

/**
 * c_param_spec_int64:
 * @name: canonical name of the property specified
 * @nick: (nullable): nick name for the property specified
 * @blurb: (nullable): description of the property specified
 * @minimum: minimum value for the property specified
 * @maximum: maximum value for the property specified
 * @default_value: default value for the property specified
 * @flags: flags for the property specified
 *
 * Creates a new #CParamSpecInt64 instance specifying a %G_TYPE_INT64 property.
 *
 * See c_param_spec_internal() for details on property names.
 *
 * Returns: (transfer full): a newly created parameter specification
 */
CParamSpec*
c_param_spec_int64 (const gchar *name,
		    const gchar *nick,
		    const gchar *blurb,
		    gint64	 minimum,
		    gint64	 maximum,
		    gint64	 default_value,
		    CParamFlags	 flags)
{
  CParamSpecInt64 *lspec;
  
  g_return_val_if_fail (default_value >= minimum && default_value <= maximum, NULL);

  lspec = c_param_spec_internal (C_TYPE_PARAM_INT64,
				 name,
				 nick,
				 blurb,
				 flags);
  
  lspec->minimum = minimum;
  lspec->maximum = maximum;
  lspec->default_value = default_value;
  
  return C_PARAM_SPEC (lspec);
}

/**
 * c_param_spec_uint64:
 * @name: canonical name of the property specified
 * @nick: (nullable): nick name for the property specified
 * @blurb: (nullable): description of the property specified
 * @minimum: minimum value for the property specified
 * @maximum: maximum value for the property specified
 * @default_value: default value for the property specified
 * @flags: flags for the property specified
 *
 * Creates a new #CParamSpecUInt64 instance specifying a %G_TYPE_UINT64
 * property.
 *
 * See c_param_spec_internal() for details on property names.
 *
 * Returns: (transfer full): a newly created parameter specification
 */
CParamSpec*
c_param_spec_uint64 (const gchar *name,
		     const gchar *nick,
		     const gchar *blurb,
		     guint64	  minimum,
		     guint64	  maximum,
		     guint64	  default_value,
		     CParamFlags  flags)
{
  CParamSpecUInt64 *uspec;
  
  g_return_val_if_fail (default_value >= minimum && default_value <= maximum, NULL);
  
  uspec = c_param_spec_internal (C_TYPE_PARAM_UINT64,
				 name,
				 nick,
				 blurb,
				 flags);
  
  uspec->minimum = minimum;
  uspec->maximum = maximum;
  uspec->default_value = default_value;
  
  return C_PARAM_SPEC (uspec);
}

/**
 * c_param_spec_unichar:
 * @name: canonical name of the property specified
 * @nick: (nullable): nick name for the property specified
 * @blurb: (nullable): description of the property specified
 * @default_value: default value for the property specified
 * @flags: flags for the property specified
 *
 * Creates a new #CParamSpecUnichar instance specifying a %G_TYPE_UINT
 * property. #GValue structures for this property can be accessed with
 * g_value_set_uint() and g_value_get_uint().
 *
 * See c_param_spec_internal() for details on property names.
 *
 * Returns: (transfer full): a newly created parameter specification
 */
CParamSpec*
c_param_spec_unichar (const gchar *name,
		      const gchar *nick,
		      const gchar *blurb,
		      gunichar	   default_value,
		      CParamFlags  flags)
{
  CParamSpecUnichar *uspec;

  uspec = c_param_spec_internal (C_TYPE_PARAM_UNICHAR,
				 name,
				 nick,
				 blurb,
				 flags);
  
  uspec->default_value = default_value;
  
  return C_PARAM_SPEC (uspec);
}

/**
 * c_param_spec_enum:
 * @name: canonical name of the property specified
 * @nick: (nullable): nick name for the property specified
 * @blurb: (nullable): description of the property specified
 * @enum_type: a #GType derived from %G_TYPE_ENUM
 * @default_value: default value for the property specified
 * @flags: flags for the property specified
 *
 * Creates a new #CParamSpecEnum instance specifying a %G_TYPE_ENUM
 * property.
 *
 * See c_param_spec_internal() for details on property names.
 *
 * Returns: (transfer full): a newly created parameter specification
 */
CParamSpec*
c_param_spec_enum (const gchar *name,
		   const gchar *nick,
		   const gchar *blurb,
		   GType	enum_type,
		   gint		default_value,
		   CParamFlags	flags)
{
  CParamSpecEnum *espec;
  GEnumClass *enum_class;
  
  g_return_val_if_fail (G_TYPE_IS_ENUM (enum_type), NULL);

  enum_class = g_type_class_ref (enum_type);

  g_return_val_if_fail (g_enum_get_value (enum_class, default_value) != NULL, NULL);
  
  espec = c_param_spec_internal (C_TYPE_PARAM_ENUM,
				 name,
				 nick,
				 blurb,
				 flags);

  espec->enum_class = enum_class;
  espec->default_value = default_value;
  C_PARAM_SPEC (espec)->value_type = enum_type;
  
  return C_PARAM_SPEC (espec);
}

/**
 * c_param_spec_flags:
 * @name: canonical name of the property specified
 * @nick: (nullable): nick name for the property specified
 * @blurb: (nullable): description of the property specified
 * @flags_type: a #GType derived from %G_TYPE_FLAGS
 * @default_value: default value for the property specified
 * @flags: flags for the property specified
 *
 * Creates a new #CParamSpecFlags instance specifying a %G_TYPE_FLAGS
 * property.
 *
 * See c_param_spec_internal() for details on property names.
 *
 * Returns: (transfer full): a newly created parameter specification
 */
CParamSpec*
c_param_spec_flags (const gchar *name,
		    const gchar *nick,
		    const gchar *blurb,
		    GType	 flags_type,
		    guint	 default_value,
		    CParamFlags	 flags)
{
  CParamSpecFlags *fspec;
  GFlagsClass *flags_class;
  
  g_return_val_if_fail (G_TYPE_IS_FLAGS (flags_type), NULL);

  flags_class = g_type_class_ref (flags_type);

  g_return_val_if_fail ((default_value & flags_class->mask) == default_value, NULL);
  
  fspec = c_param_spec_internal (C_TYPE_PARAM_FLAGS,
				 name,
				 nick,
				 blurb,
				 flags);
  
  fspec->flags_class = flags_class;
  fspec->default_value = default_value;
  C_PARAM_SPEC (fspec)->value_type = flags_type;
  
  return C_PARAM_SPEC (fspec);
}

/**
 * c_param_spec_float:
 * @name: canonical name of the property specified
 * @nick: (nullable): nick name for the property specified
 * @blurb: (nullable): description of the property specified
 * @minimum: minimum value for the property specified
 * @maximum: maximum value for the property specified
 * @default_value: default value for the property specified
 * @flags: flags for the property specified
 *
 * Creates a new #CParamSpecFloat instance specifying a %G_TYPE_FLOAT property.
 *
 * See c_param_spec_internal() for details on property names.
 *
 * Returns: (transfer full): a newly created parameter specification
 */
CParamSpec*
c_param_spec_float (const gchar *name,
		    const gchar *nick,
		    const gchar *blurb,
		    gfloat	 minimum,
		    gfloat	 maximum,
		    gfloat	 default_value,
		    CParamFlags	 flags)
{
  CParamSpecFloat *fspec;

  g_return_val_if_fail (default_value >= minimum && default_value <= maximum, NULL);

  fspec = c_param_spec_internal (C_TYPE_PARAM_FLOAT,
				 name,
				 nick,
				 blurb,
				 flags);
  
  fspec->minimum = minimum;
  fspec->maximum = maximum;
  fspec->default_value = default_value;
  
  return C_PARAM_SPEC (fspec);
}

/**
 * c_param_spec_double:
 * @name: canonical name of the property specified
 * @nick: (nullable): nick name for the property specified
 * @blurb: (nullable): description of the property specified
 * @minimum: minimum value for the property specified
 * @maximum: maximum value for the property specified
 * @default_value: default value for the property specified
 * @flags: flags for the property specified
 *
 * Creates a new #CParamSpecDouble instance specifying a %G_TYPE_DOUBLE
 * property.
 *
 * See c_param_spec_internal() for details on property names.
 *
 * Returns: (transfer full): a newly created parameter specification
 */
CParamSpec*
c_param_spec_double (const gchar *name,
		     const gchar *nick,
		     const gchar *blurb,
		     gdouble	  minimum,
		     gdouble	  maximum,
		     gdouble	  default_value,
		     CParamFlags  flags)
{
  CParamSpecDouble *dspec;

  g_return_val_if_fail (default_value >= minimum && default_value <= maximum, NULL);

  dspec = c_param_spec_internal (C_TYPE_PARAM_DOUBLE,
				 name,
				 nick,
				 blurb,
				 flags);
  
  dspec->minimum = minimum;
  dspec->maximum = maximum;
  dspec->default_value = default_value;
  
  return C_PARAM_SPEC (dspec);
}

/**
 * c_param_spec_string:
 * @name: canonical name of the property specified
 * @nick: (nullable): nick name for the property specified
 * @blurb: (nullable): description of the property specified
 * @default_value: (nullable): default value for the property specified
 * @flags: flags for the property specified
 *
 * Creates a new #CParamSpecString instance.
 *
 * See c_param_spec_internal() for details on property names.
 *
 * Returns: (transfer full): a newly created parameter specification
 */
CParamSpec*
c_param_spec_string (const gchar *name,
		     const gchar *nick,
		     const gchar *blurb,
		     const gchar *default_value,
		     CParamFlags  flags)
{
  CParamSpecString *sspec = c_param_spec_internal (C_TYPE_PARAM_STRING,
						   name,
						   nick,
						   blurb,
						   flags);

  g_free (sspec->default_value);
  sspec->default_value = g_strdup (default_value);
  
  return C_PARAM_SPEC (sspec);
}

/**
 * c_param_spec_param:
 * @name: canonical name of the property specified
 * @nick: (nullable): nick name for the property specified
 * @blurb: (nullable): description of the property specified
 * @param_type: a #GType derived from %C_TYPE_PARAM
 * @flags: flags for the property specified
 *
 * Creates a new #CParamSpecParam instance specifying a %C_TYPE_PARAM
 * property.
 *
 * See c_param_spec_internal() for details on property names.
 *
 * Returns: (transfer full): a newly created parameter specification
 */
CParamSpec*
c_param_spec_param (const gchar *name,
		    const gchar *nick,
		    const gchar *blurb,
		    GType	 param_type,
		    CParamFlags  flags)
{
  CParamSpecParam *pspec;
  
  g_return_val_if_fail (C_TYPE_IS_PARAM (param_type), NULL);
  
  pspec = c_param_spec_internal (C_TYPE_PARAM_PARAM,
				 name,
				 nick,
				 blurb,
				 flags);

  C_PARAM_SPEC (pspec)->value_type = param_type;
  
  return C_PARAM_SPEC (pspec);
}

/**
 * c_param_spec_boxed:
 * @name: canonical name of the property specified
 * @nick: (nullable): nick name for the property specified
 * @blurb: (nullable): description of the property specified
 * @boxed_type: %G_TYPE_BOXED derived type of this property
 * @flags: flags for the property specified
 *
 * Creates a new #CParamSpecBoxed instance specifying a %G_TYPE_BOXED
 * derived property.
 *
 * See c_param_spec_internal() for details on property names.
 *
 * Returns: (transfer full): a newly created parameter specification
 */
CParamSpec*
c_param_spec_boxed (const gchar *name,
		    const gchar *nick,
		    const gchar *blurb,
		    GType	 boxed_type,
		    CParamFlags  flags)
{
  CParamSpecBoxed *bspec;
  
  g_return_val_if_fail (G_TYPE_IS_BOXED (boxed_type), NULL);
  g_return_val_if_fail (G_TYPE_IS_VALUE_TYPE (boxed_type), NULL);
  
  bspec = c_param_spec_internal (C_TYPE_PARAM_BOXED,
				 name,
				 nick,
				 blurb,
				 flags);

  C_PARAM_SPEC (bspec)->value_type = boxed_type;
  
  return C_PARAM_SPEC (bspec);
}

/**
 * c_param_spec_pointer:
 * @name: canonical name of the property specified
 * @nick: (nullable): nick name for the property specified
 * @blurb: (nullable): description of the property specified
 * @flags: flags for the property specified
 *
 * Creates a new #CParamSpecPointer instance specifying a pointer property.
 * Where possible, it is better to use c_param_spec_object() or
 * c_param_spec_boxed() to expose memory management information.
 *
 * See c_param_spec_internal() for details on property names.
 *
 * Returns: (transfer full): a newly created parameter specification
 */
CParamSpec*
c_param_spec_pointer (const gchar *name,
		      const gchar *nick,
		      const gchar *blurb,
		      CParamFlags  flags)
{
  CParamSpecPointer *pspec;
  
  pspec = c_param_spec_internal (C_TYPE_PARAM_POINTER,
				 name,
				 nick,
				 blurb,
				 flags);

  return C_PARAM_SPEC (pspec);
}

/**
 * c_param_spec_gtype:
 * @name: canonical name of the property specified
 * @nick: (nullable): nick name for the property specified
 * @blurb: (nullable): description of the property specified
 * @is_a_type: a #GType whose subtypes are allowed as values
 *  of the property (use %G_TYPE_NONE for any type)
 * @flags: flags for the property specified
 *
 * Creates a new #CParamSpecGType instance specifying a
 * %G_TYPE_GTYPE property.
 *
 * See c_param_spec_internal() for details on property names.
 *
 * Since: 2.10
 *
 * Returns: (transfer full): a newly created parameter specification
 */
CParamSpec*
c_param_spec_gtype (const gchar *name,
		    const gchar *nick,
		    const gchar *blurb,
		    GType        is_a_type,
		    CParamFlags  flags)
{
  CParamSpecGType *tspec;
  
  tspec = c_param_spec_internal (C_TYPE_PARAM_GTYPE,
				 name,
				 nick,
				 blurb,
				 flags);

  tspec->is_a_type = is_a_type;

  return C_PARAM_SPEC (tspec);
}

/**
 * c_param_spec_value_array: (skip)
 * @name: canonical name of the property specified
 * @nick: (nullable): nick name for the property specified
 * @blurb: (nullable): description of the property specified
 * @element_spec: a #CParamSpec describing the elements contained in
 *  arrays of this property, may be %NULL
 * @flags: flags for the property specified
 *
 * Creates a new #CParamSpecValueArray instance specifying a
 * %G_TYPE_VALUE_ARRAY property. %G_TYPE_VALUE_ARRAY is a
 * %G_TYPE_BOXED type, as such, #GValue structures for this property
 * can be accessed with g_value_set_boxed() and g_value_get_boxed().
 *
 * See c_param_spec_internal() for details on property names.
 *
 * Returns: a newly created parameter specification
 */
CParamSpec*
c_param_spec_value_array (const gchar *name,
			  const gchar *nick,
			  const gchar *blurb,
			  CParamSpec  *element_spec,
			  CParamFlags  flags)
{
  CParamSpecValueArray *aspec;
  
  g_return_val_if_fail (element_spec == NULL || C_IS_PARAM_SPEC (element_spec), NULL);
  
  aspec = c_param_spec_internal (C_TYPE_PARAM_VALUE_ARRAY,
				 name,
				 nick,
				 blurb,
				 flags);

  if (element_spec)
    {
      aspec->element_spec = c_param_spec_ref (element_spec);
      c_param_spec_sink (element_spec);
    }

  return C_PARAM_SPEC (aspec);
}

/**
 * c_param_spec_object:
 * @name: canonical name of the property specified
 * @nick: (nullable): nick name for the property specified
 * @blurb: (nullable): description of the property specified
 * @object_type: %G_TYPE_OBJECT derived type of this property
 * @flags: flags for the property specified
 *
 * Creates a new #CParamSpecBoxed instance specifying a %G_TYPE_OBJECT
 * derived property.
 *
 * See c_param_spec_internal() for details on property names.
 *
 * Returns: (transfer full): a newly created parameter specification
 */
CParamSpec*
c_param_spec_object (const gchar *name,
		     const gchar *nick,
		     const gchar *blurb,
		     GType	  object_type,
		     CParamFlags  flags)
{
  CParamSpecObject *ospec;
  
  g_return_val_if_fail (g_type_is_a (object_type, G_TYPE_OBJECT), NULL);
  
  ospec = c_param_spec_internal (C_TYPE_PARAM_OBJECT,
				 name,
				 nick,
				 blurb,
				 flags);

  C_PARAM_SPEC (ospec)->value_type = object_type;
  
  return C_PARAM_SPEC (ospec);
}

/**
 * c_param_spec_override: (skip)
 * @name: the name of the property.
 * @overridden: The property that is being overridden
 *
 * Creates a new property of type #CParamSpecOverride. This is used
 * to direct operations to another paramspec, and will not be directly
 * useful unless you are implementing a new base type similar to GObject.
 *
 * Since: 2.4
 *
 * Returns: the newly created #CParamSpec
 */
CParamSpec*
c_param_spec_override (const gchar *name,
		       CParamSpec  *overridden)
{
  CParamSpec *pspec;
  
  g_return_val_if_fail (name != NULL, NULL);
  g_return_val_if_fail (C_IS_PARAM_SPEC (overridden), NULL);
  
  /* Dereference further redirections for property that was passed in
   */
  while (TRUE)
    {
      CParamSpec *indirect = c_param_spec_get_redirect_target (overridden);
      if (indirect)
	overridden = indirect;
      else
	break;
    }

  pspec = c_param_spec_internal (C_TYPE_PARAM_OVERRIDE,
				 name, NULL, NULL,
				 overridden->flags);
  
  pspec->value_type = C_PARAM_SPEC_VALUE_TYPE (overridden);
  C_PARAM_SPEC_OVERRIDE (pspec)->overridden = c_param_spec_ref (overridden);

  return pspec;
}

/**
 * c_param_spec_variant:
 * @name: canonical name of the property specified
 * @nick: (nullable): nick name for the property specified
 * @blurb: (nullable): description of the property specified
 * @type: a #GVariantType
 * @default_value: (nullable) (transfer full): a #GVariant of type @type to
 *                 use as the default value, or %NULL
 * @flags: flags for the property specified
 *
 * Creates a new #CParamSpecVariant instance specifying a #GVariant
 * property.
 *
 * If @default_value is floating, it is consumed.
 *
 * See c_param_spec_internal() for details on property names.
 *
 * Returns: (transfer full): the newly created #CParamSpec
 *
 * Since: 2.26
 */
CParamSpec*
c_param_spec_variant (const gchar        *name,
                      const gchar        *nick,
                      const gchar        *blurb,
                      const GVariantType *type,
                      GVariant           *default_value,
                      CParamFlags         flags)
{
  CParamSpecVariant *vspec;

  g_return_val_if_fail (type != NULL, NULL);
  g_return_val_if_fail (default_value == NULL ||
                        g_variant_is_of_type (default_value, type), NULL);

  vspec = c_param_spec_internal (C_TYPE_PARAM_VARIANT,
                                 name,
                                 nick,
                                 blurb,
                                 flags);

  vspec->type = g_variant_type_copy (type);
  if (default_value)
    vspec->default_value = g_variant_ref_sink (default_value);

  return C_PARAM_SPEC (vspec);
}
