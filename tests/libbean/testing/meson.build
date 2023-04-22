libbean_testing_name = 'libbean-testing'
libbean_testing_string = '@0@-@1@'.format(libbean_testing_name, api_version)

libbean_testing_public_h = [
  'testing.h',
  'testing-extension.h',
]

libbean_testing_c = [
  'testing.c',
  'testing-extension.c',
]

libbean_testing_deps = [
  glib_dep,
  introspection_dep,
  libbean_dep,
  libtesting_util_dep,
  libintrospection_dep,
]

libbean_testing_c_args = [
  '-DHAVE_CONFIG_H',
  '-DBUILDDIR="@0@"'.format(builddir),
  '-DSRCDIR="@0@"'.format(srcdir),
]

libbean_testing_lib = library(
  libbean_testing_string,
  libbean_testing_c,
  include_directories: rootdir,
  dependencies: libbean_testing_deps,
  c_args: libbean_testing_c_args,
  install: false,
)

libbean_testing_dep = declare_dependency(
  link_with: libbean_testing_lib,
  include_directories: include_directories('.'),
  dependencies: libbean_testing_deps,
)
