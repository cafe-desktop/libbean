/*
 * bean-plugin-loader-python.c
 * This file is part of libbean
 *
 * Copyright (C) 2008 - Jesse van den Kieboom
 * Copyright (C) 2009 - Steve Frécinaux
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

#include "bean-plugin-loader-python.h"
#include "bean-python-internal.h"
#include "libbean/bean-plugin-info-priv.h"

/* _POSIX_C_SOURCE is defined in Python.h and in limits.h included by
 * glib-object.h, so we unset it here to avoid a warning. Yep, that's bad.
 */
#undef _POSIX_C_SOURCE
#include <pygobject.h>

typedef struct {
  PyThreadState *py_thread_state;

  guint n_loaded_plugins;

  guint init_failed : 1;
  guint must_finalize_python : 1;
} BeanPluginLoaderPythonPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (BeanPluginLoaderPython,
                            bean_plugin_loader_python,
                            BEAN_TYPE_PLUGIN_LOADER)

#define GET_PRIV(o) \
  (bean_plugin_loader_python_get_instance_private (o))

static GQuark quark_extension_type = 0;

G_MODULE_EXPORT void
bean_register_types (BeanObjectModule *module)
{
  bean_object_module_register_extension_type (module,
                                              BEAN_TYPE_PLUGIN_LOADER,
                                              BEAN_TYPE_PLUGIN_LOADER_PYTHON);
}

static GType
find_python_extension_type (GType     exten_type,
                            PyObject *pymodule)
{
  PyObject *pyexten_type, *pytype;
  GType the_type = G_TYPE_INVALID;

  pyexten_type = pyg_type_wrapper_new (exten_type);

  pytype = bean_python_internal_call ("find_extension_type",
                                      &PyType_Type, "(OO)",
                                      pyexten_type, pymodule);
  Py_DECREF (pyexten_type);

  if (pytype != NULL)
    {
      the_type = pyg_type_from_object (pytype);
      Py_DECREF (pytype);

      g_return_val_if_fail (g_type_is_a (the_type, exten_type),
                            G_TYPE_INVALID);
    }

  return the_type;
}

static gboolean
bean_plugin_loader_python_provides_extension (BeanPluginLoader *loader G_GNUC_UNUSED,
                                              BeanPluginInfo   *info,
                                              GType             exten_type)
{
  PyObject *pymodule = info->loader_data;
  GType the_type;
  PyGILState_STATE state = PyGILState_Ensure ();

  the_type = find_python_extension_type (exten_type, pymodule);

  PyGILState_Release (state);
  return the_type != G_TYPE_INVALID;
}

static BeanExtension *
bean_plugin_loader_python_create_extension (BeanPluginLoader *loader G_GNUC_UNUSED,
                                            BeanPluginInfo   *info,
                                            GType             exten_type,
                                            guint             n_parameters,
                                            GParameter       *parameters)
{
  PyObject *pymodule = info->loader_data;
  GType the_type;
  GObject *object = NULL;
  PyObject *pyobject;
  PyObject *pyplinfo;
  PyGILState_STATE state = PyGILState_Ensure ();

  the_type = find_python_extension_type (exten_type, pymodule);
  if (the_type == G_TYPE_INVALID)
    goto out;

  object = g_object_newv (the_type, n_parameters, parameters);
  if (object == NULL)
    goto out;

  /* Sink floating references if necessary */
  if (g_object_is_floating (object))
    g_object_ref_sink (object);

  /* We have to remember which interface we are instantiating
   * for the deprecated bean_extension_get_extension_type().
   */
  g_object_set_qdata (object, quark_extension_type,
                      GSIZE_TO_POINTER (exten_type));

  pyobject = pygobject_new (object);
  pyplinfo = pyg_boxed_new (BEAN_TYPE_PLUGIN_INFO, info, TRUE, TRUE);

  /* Set the plugin info as an attribute of the instance */
  if (PyObject_SetAttrString (pyobject, "plugin_info", pyplinfo) != 0)
    {
      g_warning ("Failed to set 'plugin_info' for '%s'",
                 g_type_name (the_type));

      if (PyErr_Occurred ())
        PyErr_Print ();

      g_clear_object (&object);
    }

  Py_DECREF (pyplinfo);
  Py_DECREF (pyobject);

out:

  PyGILState_Release (state);
  return object;
}

static gboolean
bean_plugin_loader_python_load (BeanPluginLoader *loader,
                                BeanPluginInfo   *info)
{
  BeanPluginLoaderPython *pyloader = BEAN_PLUGIN_LOADER_PYTHON (loader);
  BeanPluginLoaderPythonPrivate *priv = GET_PRIV (pyloader);
  const gchar *module_dir, *module_name;
  PyObject *pymodule;
  PyGILState_STATE state = PyGILState_Ensure ();

  module_dir = bean_plugin_info_get_module_dir (info);
  module_name = bean_plugin_info_get_module_name (info);

  pymodule = bean_python_internal_call ("load", &PyModule_Type, "(sss)",
                                        info->filename,
                                        module_dir, module_name);

  if (pymodule != NULL)
    {
      info->loader_data = pymodule;
      priv->n_loaded_plugins += 1;
    }

  PyGILState_Release (state);
  return pymodule != NULL;
}

static void
bean_plugin_loader_python_unload (BeanPluginLoader *loader,
                                  BeanPluginInfo   *info)
{
  BeanPluginLoaderPython *pyloader = BEAN_PLUGIN_LOADER_PYTHON (loader);
  BeanPluginLoaderPythonPrivate *priv = GET_PRIV (pyloader);
  PyGILState_STATE state = PyGILState_Ensure ();

  /* We have to use this as a hook as the
   * loader will not be finalized by applications
   */
  if (--priv->n_loaded_plugins == 0)
    bean_python_internal_call ("all_plugins_unloaded", NULL, NULL);

  Py_CLEAR (info->loader_data);
  PyGILState_Release (state);
}

static void
bean_plugin_loader_python_garbage_collect (BeanPluginLoader *loader G_GNUC_UNUSED)
{
  PyGILState_STATE state = PyGILState_Ensure ();

  bean_python_internal_call ("garbage_collect", NULL, NULL);

  PyGILState_Release (state);
}

static gboolean
bean_plugin_loader_python_initialize (BeanPluginLoader *loader)
{
  BeanPluginLoaderPython *pyloader = BEAN_PLUGIN_LOADER_PYTHON (loader);
  BeanPluginLoaderPythonPrivate *priv = GET_PRIV (pyloader);
  PyGILState_STATE state = 0;
  long hexversion;

  /* We can't support multiple Python interpreter states:
   * https://bugzilla.gnome.org/show_bug.cgi?id=677091
   */

  /* Python initialization */
  if (Py_IsInitialized ())
    {
      state = PyGILState_Ensure ();
    }
  else
    {
      Py_InitializeEx (FALSE);
      priv->must_finalize_python = TRUE;
    }

  hexversion = PyLong_AsLong (PySys_GetObject ((char *) "hexversion"));

#if PY_VERSION_HEX < 0x03000000
  if (hexversion >= 0x03000000)
#else
  if (hexversion < 0x03000000)
#endif
    {
      g_critical ("Attempting to mix incompatible Python versions");

      goto python_init_error;
    }

  /* Initialize PyGObject */
  pygobject_init (PYGOBJECT_MAJOR_VERSION,
                  PYGOBJECT_MINOR_VERSION,
                  PYGOBJECT_MICRO_VERSION);

  if (PyErr_Occurred ())
    {
      g_warning ("Error initializing Python Plugin Loader: "
                 "PyGObject initialization failed");

      goto python_init_error;
    }

  /* Initialize support for threads */
  pyg_enable_threads ();
  Py_Initialize ();

  /* Only redirect warnings when python was not already initialized */
  if (!priv->must_finalize_python)
    pyg_disable_warning_redirections ();

  /* Must be done last, finalize() depends on init_failed */
  if (!bean_python_internal_setup (!priv->must_finalize_python))
    {
      /* Already warned */
      goto python_init_error;
    }

  if (!priv->must_finalize_python)
    PyGILState_Release (state);
  else
    priv->py_thread_state = PyEval_SaveThread ();

  return TRUE;

python_init_error:

  if (PyErr_Occurred ())
    PyErr_Print ();

  g_warning ("Please check the installation of all the Python "
             "related packages required by libbean and try again");

  if (!priv->must_finalize_python)
    PyGILState_Release (state);

  priv->init_failed = TRUE;
  return FALSE;
}

static void
bean_plugin_loader_python_init (BeanPluginLoaderPython *pyloader G_GNUC_UNUSED)
{
}

static void
bean_plugin_loader_python_finalize (GObject *object)
{
  BeanPluginLoaderPython *pyloader = BEAN_PLUGIN_LOADER_PYTHON (object);
  BeanPluginLoaderPythonPrivate *priv = GET_PRIV (pyloader);
  PyGILState_STATE state;

  if (!Py_IsInitialized ())
    goto out;

  g_warn_if_fail (priv->n_loaded_plugins == 0);

  if (!priv->init_failed)
    {
      state = PyGILState_Ensure ();
      bean_python_internal_shutdown ();
      PyGILState_Release (state);
    }

  if (priv->py_thread_state)
    PyEval_RestoreThread (priv->py_thread_state);

  if (priv->must_finalize_python)
    {
      if (!priv->init_failed)
        PyGILState_Ensure ();

      Py_Finalize ();
    }

out:

  G_OBJECT_CLASS (bean_plugin_loader_python_parent_class)->finalize (object);
}

static void
bean_plugin_loader_python_class_init (BeanPluginLoaderPythonClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  BeanPluginLoaderClass *loader_class = BEAN_PLUGIN_LOADER_CLASS (klass);

  quark_extension_type = g_quark_from_static_string ("bean-extension-type");

  object_class->finalize = bean_plugin_loader_python_finalize;

  loader_class->initialize = bean_plugin_loader_python_initialize;
  loader_class->load = bean_plugin_loader_python_load;
  loader_class->unload = bean_plugin_loader_python_unload;
  loader_class->create_extension = bean_plugin_loader_python_create_extension;
  loader_class->provides_extension = bean_plugin_loader_python_provides_extension;
  loader_class->garbage_collect = bean_plugin_loader_python_garbage_collect;
}
