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
 * cparam.h: CParamSpec base class implementation
 */


#include <gobject/gvaluecollector.h>


G_BEGIN_DECLS

#define C_TYPE_PARAM			G_TYPE_MAKE_FUNDAMENTAL (19)

/* --- standard type macros --- */
/*
 * C_TYPE_IS_PARAM:
 * @type: a #GType ID
 * 
 * Checks whether @type "is a" %C_TYPE_PARAM.
 */
#define C_TYPE_IS_PARAM(type)		(G_TYPE_FUNDAMENTAL (type) == C_TYPE_PARAM)
/*
 * C_PARAM_SPEC:
 * @pspec: a valid #CParamSpec
 * 
 * Casts a derived #CParamSpec object (e.g. of type #CParamSpecInt) into
 * a #CParamSpec object.
 */
#define C_PARAM_SPEC(pspec)		(G_TYPE_CHECK_INSTANCE_CAST ((pspec), C_TYPE_PARAM, CParamSpec))
/*
 * C_IS_PARAM_SPEC:
 * @pspec: a #CParamSpec
 * 
 * Checks whether @pspec "is a" valid #CParamSpec structure of type %C_TYPE_PARAM
 * or derived.
 */
#if GLIB_VERSION_MAX_ALLOWED >= GLIB_VERSION_2_42
#define C_IS_PARAM_SPEC(pspec)		(G_TYPE_CHECK_INSTANCE_FUNDAMENTAL_TYPE ((pspec), C_TYPE_PARAM))
#else
#define C_IS_PARAM_SPEC(pspec)		(G_TYPE_CHECK_INSTANCE_TYPE ((pspec), C_TYPE_PARAM))
#endif
/*
 * C_PARAM_SPEC_CLASS:
 * @pclass: a valid #CParamSpecClass
 * 
 * Casts a derived #CParamSpecClass structure into a #CParamSpecClass structure.
 */
#define C_PARAM_SPEC_CLASS(pclass)      (G_TYPE_CHECK_CLASS_CAST ((pclass), C_TYPE_PARAM, CParamSpecClass))
/*
 * C_IS_PARAM_SPEC_CLASS:
 * @pclass: a #CParamSpecClass
 * 
 * Checks whether @pclass "is a" valid #CParamSpecClass structure of type 
 * %C_TYPE_PARAM or derived.
 */
#define C_IS_PARAM_SPEC_CLASS(pclass)   (G_TYPE_CHECK_CLASS_TYPE ((pclass), C_TYPE_PARAM))
/*
 * C_PARAM_SPEC_GET_CLASS:
 * @pspec: a valid #CParamSpec
 * 
 * Retrieves the #CParamSpecClass of a #CParamSpec.
 */
#define C_PARAM_SPEC_GET_CLASS(pspec)	(G_TYPE_INSTANCE_GET_CLASS ((pspec), C_TYPE_PARAM, CParamSpecClass))


/* --- convenience macros --- */
/*
 * C_PARAM_SPEC_TYPE:
 * @pspec: a valid #CParamSpec
 * 
 * Retrieves the #GType of this @pspec.
 */
#define C_PARAM_SPEC_TYPE(pspec)	(G_TYPE_FROM_INSTANCE (pspec))
/*
 * C_PARAM_SPEC_TYPE_NAME:
 * @pspec: a valid #CParamSpec
 * 
 * Retrieves the #GType name of this @pspec.
 */
#define C_PARAM_SPEC_TYPE_NAME(pspec)	(g_type_name (C_PARAM_SPEC_TYPE (pspec)))
/*
 * C_PARAM_SPEC_VALUE_TYPE:
 * @pspec: a valid #CParamSpec
 * 
 * Retrieves the #GType to initialize a #GValue for this parameter.
 */
#define	C_PARAM_SPEC_VALUE_TYPE(pspec)	(C_PARAM_SPEC (pspec)->value_type)
/*
 * C_VALUE_HOLDS_PARAM:
 * @value: a valid #GValue structure
 * 
 * Checks whether the given #GValue can hold values derived from type %C_TYPE_PARAM.
 * 
 * Returns: %TRUE on success.
 */
#define C_VALUE_HOLDS_PARAM(value)	(G_TYPE_CHECK_VALUE_TYPE ((value), C_TYPE_PARAM))
       

/* --- flags --- */
/*
 * CParamFlags:
 * @C_PARAM_READABLE: the parameter is readable
 * @C_PARAM_WRITABLE: the parameter is writable
 * @C_PARAM_READWRITE: alias for %C_PARAM_READABLE | %C_PARAM_WRITABLE
 * @C_PARAM_CONSTRUCT: the parameter will be set upon object construction.
 *   See [vfunc@Object.constructed] for more details
 * @C_PARAM_CONSTRUCT_ONLY: the parameter can only be set upon object construction.
 *   See [vfunc@Object.constructed] for more details
 * @C_PARAM_LAX_VALIDATION: upon parameter conversion (see c_param_value_convert())
 *  strict validation is not required
 * @C_PARAM_STATIC_NAME: the string used as name when constructing the 
 *  parameter is guaranteed to remain valid and
 *  unmodified for the lifetime of the parameter. 
 *  Since 2.8
 * @C_PARAM_STATIC_NICK: the string used as nick when constructing the
 *  parameter is guaranteed to remain valid and
 *  unmmodified for the lifetime of the parameter.
 *  Since 2.8
 * @C_PARAM_STATIC_BLURB: the string used as blurb when constructing the 
 *  parameter is guaranteed to remain valid and 
 *  unmodified for the lifetime of the parameter. 
 *  Since 2.8
 * @C_PARAM_EXPLICIT_NOTIFY: calls to g_object_set_property() for this
 *   property will not automatically result in a "notify" signal being
 *   emitted: the implementation must call g_object_notify() themselves
 *   in case the property actually changes.  Since: 2.42.
 * @C_PARAM_PRIVATE: internal
 * @C_PARAM_DEPRECATED: the parameter is deprecated and will be removed
 *  in a future version. A warning will be generated if it is used
 *  while running with G_ENABLE_DIAGNOSTIC=1.
 *  Since 2.26
 * 
 * Through the #CParamFlags flag values, certain aspects of parameters
 * can be configured.
 *
 * See also: %C_PARAM_STATIC_STRINGS
 */
typedef enum
{
  C_PARAM_READABLE            = 1 << 0,
  C_PARAM_WRITABLE            = 1 << 1,
  C_PARAM_READWRITE           = (C_PARAM_READABLE | C_PARAM_WRITABLE),
  C_PARAM_CONSTRUCT	      = 1 << 2,
  C_PARAM_CONSTRUCT_ONLY      = 1 << 3,
  C_PARAM_LAX_VALIDATION      = 1 << 4,
  C_PARAM_STATIC_NAME	      = 1 << 5,
  C_PARAM_PRIVATE GOBJECT_DEPRECATED_ENUMERATOR_IN_2_26 = C_PARAM_STATIC_NAME,
  C_PARAM_STATIC_NICK	      = 1 << 6,
  C_PARAM_STATIC_BLURB	      = 1 << 7,
  /* User defined flags go here */
  C_PARAM_EXPLICIT_NOTIFY     = 1 << 30,
  /* Avoid warning with -Wpedantic for gcc6 */
  C_PARAM_DEPRECATED          = (gint)(1u << 31)
} CParamFlags;

/*
 * C_PARAM_STATIC_STRINGS:
 * 
 * #CParamFlags value alias for %C_PARAM_STATIC_NAME | %C_PARAM_STATIC_NICK | %C_PARAM_STATIC_BLURB.
 * 
 * It is recommended to use this for all properties by default, as it allows for
 * internal performance improvements in GObject.
 *
 * It is very rare that a property would have a dynamically constructed name,
 * nickname or blurb.
 *
 * Since 2.13.0
 */
#define	C_PARAM_STATIC_STRINGS (C_PARAM_STATIC_NAME | C_PARAM_STATIC_NICK | C_PARAM_STATIC_BLURB)
/* bits in the range 0xffffff00 are reserved for 3rd party usage */
/*
 * C_PARAM_MASK:
 * 
 * Mask containing the bits of #CParamSpec.flags which are reserved for GLib.
 */
#define	C_PARAM_MASK		(0x000000ff)
/*
 * C_PARAM_USER_SHIFT:
 * 
 * Minimum shift count to be used for user defined flags, to be stored in
 * #CParamSpec.flags. The maximum allowed is 10.
 */
#define	C_PARAM_USER_SHIFT	(8)

/* --- typedefs & structures --- */
typedef struct _CParamSpec      CParamSpec;
typedef struct _CParamSpecClass CParamSpecClass;
typedef struct _CParameter	CParameter GOBJECT_DEPRECATED_TYPE_IN_2_54;
typedef struct _CParamSpecPool  CParamSpecPool;

struct _CParamSpec
{
  GTypeInstance  g_type_instance;

  const gchar   *name;          /* interned string */
  CParamFlags    flags;
  GType		 value_type;
  GType		 owner_type;	/* class or interface using this property */

  /*< private >*/
  gchar         *_nick;
  gchar         *_blurb;
  GData		*qdata;
  guint          ref_count;
  guint		 param_id;	/* sort-criteria */
};
/*
 * CParamSpecClass:
 * @g_type_class: the parent class
 * @value_type: the #GValue type for this parameter
 * @finalize: The instance finalization function (optional), should chain 
 *  up to the finalize method of the parent class.
 * @value_set_default: Resets a @value to the default value for this type
 *  (recommended, the default is g_value_reset()), see 
 *  c_param_value_set_default().
 * @value_validate: Ensures that the contents of @value comply with the 
 *  specifications set out by this type (optional), see 
 *  c_param_value_validate().
 * @values_cmp: Compares @value1 with @value2 according to this type
 *  (recommended, the default is memcmp()), see c_param_values_cmp().
 * @value_is_valid: Checks if contents of @value comply with the specifications
 *   set out by this type, without modifying the value. This vfunc is optional.
 *   If it isn't set, GObject will use @value_validate. Since 2.74
 *
 * The class structure for the CParamSpec type.
 * Normally, CParamSpec classes are filled by
 * c_param_type_register_static().
 */
struct _CParamSpecClass
{
  GTypeClass      g_type_class;

  GType		  value_type;

  void	        (*finalize)		(CParamSpec   *pspec);

  /* CParam methods */
  void          (*value_set_default)    (CParamSpec   *pspec,
					 GValue       *value);
  gboolean      (*value_validate)       (CParamSpec   *pspec,
					 GValue       *value);
  gint          (*values_cmp)           (CParamSpec   *pspec,
					 const GValue *value1,
					 const GValue *value2);

  gboolean      (*value_is_valid)       (CParamSpec   *pspec,
                                         const GValue *value);

  /*< private >*/
  gpointer	  dummy[3];
};
/*
 * CParameter:
 * @name: the parameter name
 * @value: the parameter value
 * 
 * The CParameter struct is an auxiliary structure used
 * to hand parameter name/value pairs to g_object_newv().
 *
 * Deprecated: 2.54: This type is not introspectable.
 */
struct _CParameter /* auxiliary structure for _setv() variants */
{
  const gchar *name;
  GValue       value;
} GOBJECT_DEPRECATED_TYPE_IN_2_54;


/* --- prototypes --- */
GOBJECT_AVAILABLE_IN_ALL
CParamSpec*	c_param_spec_ref		(CParamSpec    *pspec);
GOBJECT_AVAILABLE_IN_ALL
void		c_param_spec_unref		(CParamSpec    *pspec);
GOBJECT_AVAILABLE_IN_ALL
void		c_param_spec_sink		(CParamSpec    *pspec);
GOBJECT_AVAILABLE_IN_ALL
CParamSpec*	c_param_spec_ref_sink   	(CParamSpec    *pspec);
GOBJECT_AVAILABLE_IN_ALL
gpointer        c_param_spec_get_qdata		(CParamSpec    *pspec,
						 GQuark         quark);
GOBJECT_AVAILABLE_IN_ALL
void            c_param_spec_set_qdata		(CParamSpec    *pspec,
						 GQuark         quark,
						 gpointer       data);
GOBJECT_AVAILABLE_IN_ALL
void            c_param_spec_set_qdata_full	(CParamSpec    *pspec,
						 GQuark         quark,
						 gpointer       data,
						 GDestroyNotify destroy);
GOBJECT_AVAILABLE_IN_ALL
gpointer        c_param_spec_steal_qdata	(CParamSpec    *pspec,
						 GQuark         quark);
GOBJECT_AVAILABLE_IN_ALL
CParamSpec*     c_param_spec_get_redirect_target (CParamSpec   *pspec);

GOBJECT_AVAILABLE_IN_ALL
void		c_param_value_set_default	(CParamSpec    *pspec,
						 GValue	       *value);
GOBJECT_AVAILABLE_IN_ALL
gboolean	c_param_value_defaults		(CParamSpec    *pspec,
						 const GValue  *value);
GOBJECT_AVAILABLE_IN_ALL
gboolean	c_param_value_validate		(CParamSpec    *pspec,
						 GValue	       *value);
GOBJECT_AVAILABLE_IN_2_74
gboolean        c_param_value_is_valid          (CParamSpec    *pspec,
                                                 const GValue  *value);
GOBJECT_AVAILABLE_IN_ALL
gboolean	c_param_value_convert		(CParamSpec    *pspec,
						 const GValue  *src_value,
						 GValue	       *dest_value,
						 gboolean	strict_validation);
GOBJECT_AVAILABLE_IN_ALL
gint		c_param_values_cmp		(CParamSpec    *pspec,
						 const GValue  *value1,
						 const GValue  *value2);
GOBJECT_AVAILABLE_IN_ALL
const gchar *   c_param_spec_get_name           (CParamSpec    *pspec);
GOBJECT_AVAILABLE_IN_ALL
const gchar *   c_param_spec_get_nick           (CParamSpec    *pspec);
GOBJECT_AVAILABLE_IN_ALL
const gchar *   c_param_spec_get_blurb          (CParamSpec    *pspec);
GOBJECT_AVAILABLE_IN_ALL
void            c_value_set_param               (GValue	       *value,
						 CParamSpec    *param);
GOBJECT_AVAILABLE_IN_ALL
CParamSpec*     c_value_get_param               (const GValue  *value);
GOBJECT_AVAILABLE_IN_ALL
CParamSpec*     c_value_dup_param               (const GValue  *value);


GOBJECT_AVAILABLE_IN_ALL
void           c_value_take_param               (GValue        *value,
					         CParamSpec    *param);
GOBJECT_DEPRECATED_FOR(c_value_take_param)
void           c_value_set_param_take_ownership (GValue        *value,
                                                 CParamSpec    *param);
GOBJECT_AVAILABLE_IN_2_36
const GValue *  c_param_spec_get_default_value  (CParamSpec    *pspec);

GOBJECT_AVAILABLE_IN_2_46
GQuark          c_param_spec_get_name_quark     (CParamSpec    *pspec);

/* --- convenience functions --- */
typedef struct _CParamSpecTypeInfo CParamSpecTypeInfo;
/*
 * CParamSpecTypeInfo:
 * @instance_size: Size of the instance (object) structure.
 * @n_preallocs: Prior to GLib 2.10, it specified the number of pre-allocated (cached) instances to reserve memory for (0 indicates no caching). Since GLib 2.10, it is ignored, since instances are allocated with the [slice allocator][glib-Memory-Slices] now.
 * @instance_init: Location of the instance initialization function (optional).
 * @value_type: The #GType of values conforming to this #CParamSpec
 * @finalize: The instance finalization function (optional).
 * @value_set_default: Resets a @value to the default value for @pspec 
 *  (recommended, the default is g_value_reset()), see 
 *  c_param_value_set_default().
 * @value_validate: Ensures that the contents of @value comply with the 
 *  specifications set out by @pspec (optional), see 
 *  c_param_value_validate().
 * @values_cmp: Compares @value1 with @value2 according to @pspec 
 *  (recommended, the default is memcmp()), see c_param_values_cmp().
 * 
 * This structure is used to provide the type system with the information
 * required to initialize and destruct (finalize) a parameter's class and
 * instances thereof.
 *
 * The initialized structure is passed to the c_param_type_register_static() 
 * The type system will perform a deep copy of this structure, so its memory 
 * does not need to be persistent across invocation of 
 * c_param_type_register_static().
 */
struct _CParamSpecTypeInfo
{
  /* type system portion */
  guint16         instance_size;                               /* obligatory */
  guint16         n_preallocs;                                 /* optional */
  void		(*instance_init)	(CParamSpec   *pspec); /* optional */

  /* class portion */
  GType           value_type;				       /* obligatory */
  void          (*finalize)             (CParamSpec   *pspec); /* optional */
  void          (*value_set_default)    (CParamSpec   *pspec,  /* recommended */
					 GValue       *value);
  gboolean      (*value_validate)       (CParamSpec   *pspec,  /* optional */
					 GValue       *value);
  gint          (*values_cmp)           (CParamSpec   *pspec,  /* recommended */
					 const GValue *value1,
					 const GValue *value2);
};
GOBJECT_AVAILABLE_IN_ALL
GType	c_param_type_register_static	(const gchar		  *name,
					 const CParamSpecTypeInfo *pspec_info);

GOBJECT_AVAILABLE_IN_2_66
gboolean c_param_spec_is_valid_name    (const gchar              *name);

/* For registering builting types */
GType  _c_param_type_register_static_constant (const gchar              *name,
					       const CParamSpecTypeInfo *pspec_info,
					       GType                     opt_type);


/* --- protected --- */
GOBJECT_AVAILABLE_IN_ALL
gpointer	c_param_spec_internal		(GType	        param_type,
						 const gchar   *name,
						 const gchar   *nick,
						 const gchar   *blurb,
						 CParamFlags    flags);
GOBJECT_AVAILABLE_IN_ALL
CParamSpecPool* c_param_spec_pool_new		(gboolean	type_prefixing);
GOBJECT_AVAILABLE_IN_ALL
void		c_param_spec_pool_insert	(CParamSpecPool	*pool,
						 CParamSpec	*pspec,
						 GType		 owner_type);
GOBJECT_AVAILABLE_IN_ALL
void		c_param_spec_pool_remove	(CParamSpecPool	*pool,
						 CParamSpec	*pspec);
GOBJECT_AVAILABLE_IN_ALL
CParamSpec*	c_param_spec_pool_lookup	(CParamSpecPool	*pool,
						 const gchar	*param_name,
						 GType		 owner_type,
						 gboolean	 walk_ancestors);
GOBJECT_AVAILABLE_IN_ALL
GList*		c_param_spec_pool_list_owned	(CParamSpecPool	*pool,
						 GType		 owner_type);
GOBJECT_AVAILABLE_IN_ALL
CParamSpec**	c_param_spec_pool_list		(CParamSpecPool	*pool,
						 GType		 owner_type,
						 guint		*n_pspecs_p);
GOBJECT_AVAILABLE_IN_2_80
void            c_param_spec_pool_free          (CParamSpecPool *pool);

/* contracts:
 *
 * gboolean value_validate (CParamSpec *pspec,
 *                          GValue     *value):
 *	modify value contents in the least destructive way, so
 *	that it complies with pspec's requirements (i.e.
 *	according to minimum/maximum ranges etc...). return
 *	whether modification was necessary.
 *
 * gint values_cmp (CParamSpec   *pspec,
 *                  const GValue *value1,
 *                  const GValue *value2):
 *	return value1 - value2, i.e. (-1) if value1 < value2,
 *	(+1) if value1 > value2, and (0) otherwise (equality)
 */

G_END_DECLS
