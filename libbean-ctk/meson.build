libbean_ctk_srcdir = include_directories('.')

libbean_ctk_public_h = files(
  'bean-ctk-autocleanups.h',
  'bean-ctk-configurable.h',
  'bean-ctk-plugin-manager.h',
  'bean-ctk-plugin-manager-view.h',
  'bean-ctk.h',
)

libbean_ctk_c = files(
  'bean-ctk-configurable.c',
  'bean-ctk-disable-plugins-dialog.c',
  'bean-ctk-plugin-manager.c',
  'bean-ctk-plugin-manager-store.c',
  'bean-ctk-plugin-manager-view.c',
)

libbean_ctk_deps = [
  glib_dep,
  gobject_dep,
  introspection_dep,
  ctk_dep,
  libbean_dep,
]

libbean_ctk_c_args = [
  '-DHAVE_CONFIG_H',
  '-DG_LOG_DOMAIN="libbean-ctk"',
]

libbean_ctk_c_args += hidden_visibility_args

libbean_ctk_link_args = [
]

install_headers(
  libbean_ctk_public_h,
  install_dir: join_paths(
    pkgincludedir,
    package_ctk_name,
  )
)

# Static library used for testing symbol access
libbean_ctk_test = static_library(
  package_ctk_string.strip('lib'),
  libbean_ctk_c,
  include_directories: rootdir,
  dependencies: libbean_ctk_deps,
  c_args: libbean_ctk_c_args,
  link_args: libbean_ctk_link_args,
)

libbean_ctk_sha = library(
  package_ctk_string.strip('lib'),
  libbean_ctk_c,
  version: lib_version,
  darwin_versions: lib_version_osx,
  include_directories: rootdir,
  dependencies: libbean_ctk_deps,
  c_args: libbean_ctk_c_args,
  link_args: libbean_ctk_link_args,
  install: true,
  install_dir: libdir,
)

libbean_ctk_dep_sources = []

if generate_gir == true
  libbean_ctk_gir_includes = [
    'GObject-2.0',
    'Gtk-3.0',
    libbean_gir.get(0)
  ]

  libbean_ctk_gir = gnome.generate_gir(
    libbean_ctk_sha,
    sources: libbean_ctk_c + libbean_ctk_public_h,
    nsversion: api_version,
    namespace: 'BeanGtk',
    symbol_prefix: 'bean_ctk',
    identifier_prefix: 'BeanGtk',
    header: 'libbean-ctk/bean-ctk.h',
    export_packages: [package_ctk_string],
    includes: libbean_ctk_gir_includes,
    link_with: [libbean_sha, libbean_ctk_sha],
    install: true,
    install_dir_gir: girdir,
    install_dir_typelib: typelibdir,
    extra_args: [
      '--warn-all',
    ],
  )

  libbean_ctk_dep_sources += [
    libbean_ctk_gir,
  ]
endif

libbean_ctk_dep = declare_dependency(
  link_with: libbean_ctk_sha,
  dependencies: libbean_ctk_deps,
  include_directories: rootdir,
  sources: libbean_ctk_dep_sources,
)

libbean_ctk_test_dep = declare_dependency(
  link_with: libbean_ctk_test,
  dependencies: libbean_ctk_deps,
  include_directories: rootdir,
  sources: libbean_ctk_dep_sources,
)

libbean_ctk_pc_reqs = [
  glib_dep,
  ctk_dep,
  package_string + ' >= @0@'.format(version)
]

libbean_ctk_pc = pkg.generate(
  libbean_ctk_sha,
  name: package_ctk_string,
  description: 'GObject plugins library widgetery',
  requires: libbean_ctk_pc_reqs,
  subdirs: package_string,
  install_dir: pkgconfigdir,
)
