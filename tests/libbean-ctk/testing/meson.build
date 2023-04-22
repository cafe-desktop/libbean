libbean_ctk_testing_name = 'libbean-ctk-testing'
libbean_ctk_testing_string = '@0@-@1@'.format(libbean_ctk_testing_name, api_version)

libbean_ctk_testing_public_h = [
  'testing.h',
]

libbean_ctk_testing_c = [
  'testing.c',
]

libbean_ctk_testing_deps = [
  glib_dep,
  introspection_dep,
  libbean_dep,
  libbean_ctk_test_dep,
  libtesting_util_dep,
]

libbean_ctk_testing_c_args = [
  '-DHAVE_CONFIG_H',
  '-DBUILDDIR="@0@"'.format(builddir),
  '-DSRCDIR="@0@"'.format(srcdir),
]

libbean_ctk_testing_lib = library(
  libbean_ctk_testing_string,
  libbean_ctk_testing_c,
  include_directories: rootdir,
  dependencies: libbean_ctk_testing_deps,
  c_args: libbean_ctk_testing_c_args,
  install: false,
)

libbean_ctk_testing_dep = declare_dependency(
  link_with: libbean_ctk_testing_lib,
  include_directories: include_directories('.'),
  dependencies: libbean_ctk_testing_deps,
)
