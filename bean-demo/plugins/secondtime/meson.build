libsecondtime_name = 'secondtime'

libsecondtime_c = [
  'second-time.c',
]

libsecondtime_plugin_data = [
  'secondtime.plugin',
]

libsecondtime_c_args = [
  '-DHAVE_CONFIG_H',
]

libsecondtime_lib = shared_library(
  libsecondtime_name,
  libsecondtime_c,
  include_directories: rootdir,
  dependencies: [ctk_dep, libbean_dep],
  c_args: libsecondtime_c_args,
  install_dir: join_paths(bean_demo_plugins_dir, 'secondtime'),
  install: true,
)

custom_target(
  'lib@0@-data'.format(libsecondtime_name),
  input: libsecondtime_plugin_data,
  output: libsecondtime_plugin_data,
  command: ['cp', '@INPUT@', '@OUTDIR@'],
  build_by_default: true,
  install_dir: join_paths(bean_demo_plugins_dir, 'secondtime'),
  install: true,
)
