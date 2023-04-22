
libbean_ctk_tests_sources = [
  ['plugin-manager'],
  ['plugin-manager-store'],
  ['plugin-manager-view'],
]

subdir('testing')

subdir('plugins')

libbean_ctk_tests_deps = [
  glib_dep,
  ctk_dep,
  libbean_dep,
  libbean_ctk_dep,

  libtesting_util_dep,
  libbean_ctk_testing_dep,
]

foreach test: libbean_ctk_tests_sources
  test_name = test.get(0)
  test_sources = ['@0@.c'.format(test_name)]

  test_exe = executable(
    test_name,
    test_sources,
    c_args: ['-UG_DISABLE_ASSERT', '-UG_DISABLE_CAST_CHECKS'],
    dependencies: libbean_ctk_tests_deps
  )

  test('test-@0@'.format(test_name), test_exe, env: test_env)
endforeach
