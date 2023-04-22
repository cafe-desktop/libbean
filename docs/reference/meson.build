glib_prefix = glib_dep.get_pkgconfig_variable('prefix')
glib_docpath = join_paths(glib_prefix, 'share', 'gtk-doc', 'html')

ctk_prefix = ctk_dep.get_pkgconfig_variable('prefix')
gtk_docpath = join_paths(ctk_prefix, 'share', 'gtk-doc', 'html')

libbean_reference_ignored_h = [
  'gconstructor.h',
  'bean-debug.h',
  'bean-dirs.h',
  'bean-engine-priv.h',
  'bean-introspection.h',
  'bean-marshal.h',
  'bean-plugin-info-priv.h',
  'bean-plugin-loader.h',
  'bean-plugin-loader-c.h',
  'bean-utils.h',
]

libbean_reference_html_images = [
]

libbean_reference_srcdirs = [
  libbean_srcdir,
]

if build_ctk_widgetry == true
  libbean_reference_ignored_h += [
    'bean-ctk-disable-plugins-dialog.h',
    'bean-ctk-plugin-manager-store.h',
  ]

  libbean_reference_html_images += [
    join_paths('images', 'bean-ctk-plugin-manager.png'),
  ]

  libbean_reference_srcdirs += [
    libbean_ctk_srcdir,
  ]
endif

version_xml = configuration_data()
version_xml.set('BEAN_VERSION', version)

configure_file(
  input: 'version.xml.in',
  output: 'version.xml',
  configuration: version_xml,
)

libbean_reference_deps = [
  libbean_dep,
]

libbean_reference_scan_args = [
  '--rebuild-types',
  '--deprecated-guards="BEAN_DISABLE_DEPRECATED"',
  '--ignore-headers=' + ' '.join(libbean_reference_ignored_h),
]

libbean_reference_fixxref_args = [
  '--extra-dir=@0@'.format(join_paths(glib_docpath, 'glib')),
  '--extra-dir=@0@'.format(join_paths(glib_docpath, 'gobject')),
  '--extra-dir=@0@'.format(join_paths(glib_docpath, 'gmodule')),
  '--extra-dir=@0@'.format(join_paths(glib_docpath, 'gio')),
]

libbean_reference_mkdb_args = [
  '--sgml-mode',
  '--output-format=xml',
]

if build_ctk_widgetry == true
  libbean_reference_deps += [
    libbean_ctk_dep,
  ]

  libbean_reference_fixxref_args += [
    '--extra-dir=@0@'.format(join_paths(gtk_docpath, 'ctk')),
  ]
endif

gnome.gtkdoc(
  package_name,
  main_sgml: 'libbean-docs.sgml',
  gobject_typesfile: 'libbean.types',
  src_dir: libbean_reference_srcdirs,
  html_assets: libbean_reference_html_images,
  dependencies: libbean_reference_deps,
  scan_args: libbean_reference_scan_args,
  fixxref_args: libbean_reference_fixxref_args,
  mkdb_args: libbean_reference_mkdb_args,
  install: true,
  install_dir: join_paths(
    gtkdocdir,
    'html',
    package_name,
  )
)
