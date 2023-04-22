libhelloworld_name = 'helloworld'

libhelloworld_c = [
  'beandemo-hello-world-configurable.c',
  'beandemo-hello-world-plugin.c',
]

libhelloworld_plugin_data = [
  'helloworld.plugin',
]

libhelloworld_c_args = [
  '-DHAVE_CONFIG_H',
]

libhelloworld_lib = shared_library(
  libhelloworld_name,
  libhelloworld_c,
  include_directories: rootdir,
  dependencies: [gtk_dep, libbean_gtk_dep],
  c_args: libhelloworld_c_args,
  install_dir: join_paths(bean_demo_plugins_dir, 'helloworld'),
  install: true,
)

custom_target(
  'lib@0@-data'.format(libhelloworld_name),
  input: libhelloworld_plugin_data,
  output: libhelloworld_plugin_data,
  command: ['cp', '@INPUT@', '@OUTDIR@'],
  build_by_default: true,
  install_dir: join_paths(bean_demo_plugins_dir, 'helloworld'),
  install: true,
)
