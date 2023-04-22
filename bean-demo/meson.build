bean_demo_name = 'bean-demo'
bean_demo_string = '@0@-@1@'.format(bean_demo_name, api_version)
bean_demo_plugins_dir = join_paths(get_option('libdir'), 'bean-demo', 'plugins')

bean_demo_c = [
  'bean-demo-window.c',
  'bean-demo.c',
]

bean_demo_c_args = [
  '-DHAVE_CONFIG_H',
  '-DPEAS_BUILDDIR="@0@"'.format(builddir),
  '-DPEAS_PREFIX="@0@"'.format(prefix),
  '-DPEAS_LIBDIR="@0@"'.format(libdir),
]

executable(
  bean_demo_name,
  bean_demo_c,
  c_args: bean_demo_c_args,
  dependencies: [libbean_gtk_dep],
  install: true,
)

subdir('plugins')
