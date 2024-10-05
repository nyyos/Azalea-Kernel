kernel_ld_args = ['-nostdlib', '-Wl,-gc-sections']

subdir('libkern')
subdir('core')

if (use_limine)
  subdir('platform/limine-generic')
endif

if (arch == 'amd64')
  subdir('platform/amd64')
endif

azalea = executable('azalea',
  kernel_sources,
  c_args: [freestanding_c_args, '-std=gnu2x'],
  cpp_args: [freestanding_cpp_args, '-std=gnu++23'],
  include_directories: [freestanding_include_dirs],
  link_args: kernel_ld_args,
  dependencies: [
    kernel_deps
  ]
)

map = custom_target('generate symbols',
          depends : [azalea],
          input : azalea,
          output : 'symbols.map',
          command : [meson.source_root()+'/tools/gen-symmap.sh', '@INPUT@', '@OUTPUT@'],
          build_by_default : true)

custom_target('demangle symbols',
  depends : [map],
  input : map, 
  output : 'demangled_symbols.map',
  command : [demangle, '@INPUT@', '@OUTPUT@'],
  build_by_default : true
)
