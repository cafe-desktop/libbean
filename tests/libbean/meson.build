
libbean_tests_sources = [
  ['engine'],
  ['extension-c'],
  ['extension-set'],
  ['plugin-info'],
]

libbean_tests_c_args = ['-UG_DISABLE_ASSERT', '-UG_DISABLE_CAST_CHECKS']

if build_lua51_loader == true
  libbean_tests_sources += [['extension-lua', [lua51_dep, luajit_dep]]]
endif

if build_python3_loader == true
  libbean_tests_sources += [['extension-py', [python3_dep, pygobject_dep]]]

  libbean_tests_c_args += [
    '-DPYGOBJECT_MAJOR_VERSION=@0@'.format(pygobject_version_major),
    '-DPYGOBJECT_MINOR_VERSION=@0@'.format(pygobject_version_minor),
    '-DPYGOBJECT_MICRO_VERSION=@0@'.format(pygobject_version_micro),
  ]
endif

subdir('introspection')
subdir('testing')

subdir('plugins')

libbean_tests_deps = [
  glib_dep,
  libbean_dep,
  libintrospection_dep,
  libintrospection_gir_dep,
  libtesting_util_dep,
  libbean_testing_dep,
  libembedded_dep,
]

foreach test: libbean_tests_sources
  test_name = test.get(0)
  test_sources = ['@0@.c'.format(test_name)]
  test_deps = libbean_tests_deps

  if test.length() > 1
    test_deps += test.get(1)
  endif

  test_exe = executable(
    test_name,
    test_sources,
    dependencies: test_deps,
    c_args: libbean_tests_c_args,
  )

  test('test-@0@'.format(test_name), test_exe, env: test_env)
endforeach
