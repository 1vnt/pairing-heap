project('pairing-heap', 'c', version : '0.1.0')

incs = [
  include_directories('.')
]

pairing_heap_dep = declare_dependency(
  include_directories: incs
)

#test_exe = executable('pairing_tests', 'tests.c')
#test('pairing heap test', test_exe)

if meson.version().version_compare('>=0.54.0')
  meson.override_dependency('pairing-heap', pairing_heap_dep)
endif
