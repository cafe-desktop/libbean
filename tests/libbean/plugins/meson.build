libbean_plugins_data = [
  'extension-c-nonexistent.plugin',
  'extension-lua51-nonexistent.plugin',
  'extension-python-nonexistent.plugin',
  'extension-python3-nonexistent.plugin',
  'info-missing-module.plugin',
  'info-missing-name.plugin',
  'invalid.plugin',
  'nonexistent-dep.plugin',
  'not-loadable.plugin',
  'os-dependant-help.plugin',
  'two-deps.plugin',
  'unkown-loader.plugin',
]

custom_target(
  'libbean-test-data',
  input: libbean_plugins_data,
  output: libbean_plugins_data,
  command: ['cp', '@INPUT@', '@OUTDIR@'],
  build_by_default: true,
)

subdir('embedded')
subdir('extension-c')

if build_lua51_loader == true
  subdir('extension-lua')
endif

if build_python3_loader == true
  subdir('extension-python')
endif
