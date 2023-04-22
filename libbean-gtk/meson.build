libbean_gtk_srcdir = include_directories('.')

libbean_gtk_public_h = files(
  'bean-gtk-autocleanups.h',
  'bean-gtk-configurable.h',
  'bean-gtk-plugin-manager.h',
  'bean-gtk-plugin-manager-view.h',
  'bean-gtk.h',
)

libbean_gtk_c = files(
  'bean-gtk-configurable.c',
  'bean-gtk-disable-plugins-dialog.c',
  'bean-gtk-plugin-manager.c',
  'bean-gtk-plugin-manager-store.c',
  'bean-gtk-plugin-manager-view.c',
)

libbean_gtk_deps = [
  glib_dep,
  gobject_dep,
  introspection_dep,
  gtk_dep,
  libbean_dep,
]

libbean_gtk_c_args = [
  '-DHAVE_CONFIG_H',
  '-DG_LOG_DOMAIN="libbean-gtk"',
]

libbean_gtk_c_args += hidden_visibility_args

libbean_gtk_link_args = [
]

install_headers(
  libbean_gtk_public_h,
  install_dir: join_paths(
    pkgincludedir,
    package_gtk_name,
  )
)

# Static library used for testing symbol access
libbean_gtk_test = static_library(
  package_gtk_string.strip('lib'),
  libbean_gtk_c,
  include_directories: rootdir,
  dependencies: libbean_gtk_deps,
  c_args: libbean_gtk_c_args,
  link_args: libbean_gtk_link_args,
)

libbean_gtk_sha = library(
  package_gtk_string.strip('lib'),
  libbean_gtk_c,
  version: lib_version,
  darwin_versions: lib_version_osx,
  include_directories: rootdir,
  dependencies: libbean_gtk_deps,
  c_args: libbean_gtk_c_args,
  link_args: libbean_gtk_link_args,
  install: true,
  install_dir: libdir,
)

libbean_gtk_dep_sources = []

if generate_gir == true
  libbean_gtk_gir_includes = [
    'GObject-2.0',
    'Gtk-3.0',
    libbean_gir.get(0)
  ]

  libbean_gtk_gir = gnome.generate_gir(
    libbean_gtk_sha,
    sources: libbean_gtk_c + libbean_gtk_public_h,
    nsversion: api_version,
    namespace: 'PeasGtk',
    symbol_prefix: 'bean_gtk',
    identifier_prefix: 'PeasGtk',
    header: 'libbean-gtk/bean-gtk.h',
    export_packages: [package_gtk_string],
    includes: libbean_gtk_gir_includes,
    link_with: [libbean_sha, libbean_gtk_sha],
    install: true,
    install_dir_gir: girdir,
    install_dir_typelib: typelibdir,
    extra_args: [
      '--warn-all',
    ],
  )

  libbean_gtk_dep_sources += [
    libbean_gtk_gir,
  ]
endif

libbean_gtk_dep = declare_dependency(
  link_with: libbean_gtk_sha,
  dependencies: libbean_gtk_deps,
  include_directories: rootdir,
  sources: libbean_gtk_dep_sources,
)

libbean_gtk_test_dep = declare_dependency(
  link_with: libbean_gtk_test,
  dependencies: libbean_gtk_deps,
  include_directories: rootdir,
  sources: libbean_gtk_dep_sources,
)

libbean_gtk_pc_reqs = [
  glib_dep,
  gtk_dep,
  package_string + ' >= @0@'.format(version)
]

libbean_gtk_pc = pkg.generate(
  libbean_gtk_sha,
  name: package_gtk_string,
  description: 'GObject plugins library widgetery',
  requires: libbean_gtk_pc_reqs,
  subdirs: package_string,
  install_dir: pkgconfigdir,
)
