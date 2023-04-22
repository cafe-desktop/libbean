libbean_gtk_testing_name = 'libbean-gtk-testing'
libbean_gtk_testing_string = '@0@-@1@'.format(libbean_gtk_testing_name, api_version)

libbean_gtk_testing_public_h = [
  'testing.h',
]

libbean_gtk_testing_c = [
  'testing.c',
]

libbean_gtk_testing_deps = [
  glib_dep,
  introspection_dep,
  libbean_dep,
  libbean_gtk_test_dep,
  libtesting_util_dep,
]

libbean_gtk_testing_c_args = [
  '-DHAVE_CONFIG_H',
  '-DBUILDDIR="@0@"'.format(builddir),
  '-DSRCDIR="@0@"'.format(srcdir),
]

libbean_gtk_testing_lib = library(
  libbean_gtk_testing_string,
  libbean_gtk_testing_c,
  include_directories: rootdir,
  dependencies: libbean_gtk_testing_deps,
  c_args: libbean_gtk_testing_c_args,
  install: false,
)

libbean_gtk_testing_dep = declare_dependency(
  link_with: libbean_gtk_testing_lib,
  include_directories: include_directories('.'),
  dependencies: libbean_gtk_testing_deps,
)
