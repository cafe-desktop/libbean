bean_demo_name = 'bean-demo'
bean_demo_string = '@0@-@1@'.format(bean_demo_name, api_version)
bean_demo_plugins_dir = join_paths(get_option('libdir'), 'bean-demo', 'plugins')

bean_demo_c = [
  'bean-demo-window.c',
  'bean-demo.c',
]

bean_demo_c_args = [
  '-DHAVE_CONFIG_H',
  '-DBEAN_BUILDDIR="@0@"'.format(builddir),
  '-DBEAN_PREFIX="@0@"'.format(prefix),
  '-DBEAN_LIBDIR="@0@"'.format(libdir),
]

executable(
  bean_demo_name,
  bean_demo_c,
  c_args: bean_demo_c_args,
  dependencies: [libbean_ctk_dep],
  install: true,
)

subdir('plugins')
