libbean_ctk_plugins_data = [
  'hidden.plugin',
  'invalid-custom-icon.plugin',
  'invalid-stock-icon.plugin',
  'valid-stock-icon.plugin',
]

custom_target(
  'libbean-ctk-test-data',
  input: libbean_ctk_plugins_data,
  output: libbean_ctk_plugins_data,
  command: ['cp', '@INPUT@', '@OUTDIR@'],
  build_by_default: true,
)

subdir('builtin-configurable')
subdir('configurable')
subdir('valid-custom-icon')
