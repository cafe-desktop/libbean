glade_catalog_xml = configuration_data()
glade_catalog_xml.set('GETTEXT_PACKAGE', package_string)

bean_glade_catalog = configure_file(
  input: 'libbean-ctk.xml.in',
  output: 'libbean-ctk.xml',
  configuration: glade_catalog_xml,
  install_dir: catalogdir,
)

# Glade file does not currently validate and this does not appear to be
# the fault of libbean.
#if xmllint_prg.found()
#  glade_catalog_dtd_path = join_paths(
#    gladeui_dep.get_pkgconfig_variable('catalogdir'),
#   'glade-catalog.dtd'
#  )
#
#  test(
#    'Validate Glade catalogue',
#    xmllint_prg,
#    args: [
#      '--dtdvalid',
#      glade_catalog_dtd_path,
#      bean_glade_catalog,
#      '--noout',
#    ]
#  )
#endif
