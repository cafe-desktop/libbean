libbean_srcdir = include_directories('.')

version_data = configuration_data()
version_data.set('BEAN_MAJOR_VERSION', version_major)
version_data.set('BEAN_MINOR_VERSION', version_minor)
version_data.set('BEAN_MICRO_VERSION', version_micro)
version_data.set('BEAN_VERSION', meson.project_version())

bean_version_h = configure_file(
  input: 'bean-version.h.in',
  output: 'bean-version.h',
  configuration: version_data,
  install_dir: join_paths(
    pkgincludedir,
    package_name,
  ),
)

libbean_public_h = files(
  'bean-activatable.h',
  'bean-autocleanups.h',
  'bean-engine.h',
  'bean-extension.h',
  'bean-extension-base.h',
  'bean-extension-set.h',
  'bean-object-module.h',
  'bean-plugin-info.h',
  'bean-version-macros.h',
  'bean.h',
)

libbean_sources = files(
  'bean-activatable.c',
  'bean-debug.c',
  'bean-dirs.c',
  'bean-engine.c',
  'bean-extension.c',
  'bean-extension-base.c',
  'bean-extension-set.c',
  'bean-i18n.c',
  'bean-introspection.c',
  'bean-object-module.c',
  'bean-plugin-info.c',
  'bean-plugin-loader.c',
  'bean-plugin-loader-c.c',
  'bean-utils.c',
)

if build_machine.system() == 'darwin'
  libbean_sources += [
    'bean-utils-osx.m',
    'bean-utils-osx.h',
  ]
endif


libbean_deps = [
  glib_dep,
  gobject_dep,
  gmodule_dep,
  gio_dep,
  introspection_dep,
]

libbean_c_args = [
  '-DHAVE_CONFIG_H',
  '-DG_LOG_DOMAIN="libbean"',
  '-DDATADIR="@0@"'.format(datadir),
  '-DLIBDIR="@0@"'.format(libdir),
]

libbean_link_args = [
]

if build_machine.system() == 'darwin'
  libbean_link_args += [
    '-Wl,-framework', '-Wl,Foundation',
    '-Wl,-framework', '-Wl,AppKit',
  ]
endif

libbean_marshal = gnome.genmarshal(
  'bean-marshal',
  sources: 'bean-marshal.list',
  prefix: 'bean_cclosure_marshal',
  install_header: false,
)

libbean_marshals_h = libbean_marshal.get(1)

libbean_sources += [
  libbean_marshal,
]

install_headers(
  libbean_public_h,
  install_dir: join_paths(
    pkgincludedir,
    package_name,
  ),
)

libbean_sha = library(
  package_string.strip('lib'),
  libbean_sources,
  version: lib_version,
  darwin_versions: lib_version_osx,
  include_directories: [rootdir, libbean_srcdir],
  dependencies: libbean_deps,
  c_args: libbean_c_args + hidden_visibility_args,
  objc_args: libbean_c_args,
  link_args: libbean_link_args,
  install: true,
  install_dir: libdir,
)

libbean_dep_sources = [
  libbean_marshals_h,
]

if generate_gir == true
  libbean_gir_includes = [
    'GObject-2.0',
    'GModule-2.0',
    'Gio-2.0',
    'GIRepository-2.0'
  ]

  libbean_gir = gnome.generate_gir(
    libbean_sha,
    sources: libbean_sources + libbean_public_h,
    nsversion: api_version,
    namespace: 'Bean',
    symbol_prefix: 'bean',
    identifier_prefix: 'Bean',
    header: 'libbean/bean.h',
    export_packages: [package_string],
    includes: libbean_gir_includes,
    link_with: libbean_sha,
    install: true,
    install_dir_gir: girdir,
    install_dir_typelib: typelibdir,
    extra_args: [
      '--warn-all',
    ],
  )

  libbean_dep_sources += [
    libbean_gir,
  ]
endif

libbean_dep = declare_dependency(
  link_with: libbean_sha,
  include_directories: [rootdir, libbean_srcdir],
  dependencies: libbean_deps,
  sources: libbean_dep_sources,
)

libbean_pc_reqs = [
  'glib-2.0 @0@'.format(glib_req),
  'gobject-2.0 @0@'.format(glib_req),
  'gmodule-2.0 @0@'.format(glib_req),
  'gio-2.0 @0@'.format(glib_req),
  'gobject-introspection-1.0 @0@'.format(introspection_req),
]

libbean_pc = pkg.generate(
  libbean_sha,
  name: package_string,
  description: 'GObject plugins library',
  subdirs: package_string,
  install_dir: pkgconfigdir,
  requires: libbean_pc_reqs,
)
