kernel_ld_args = ['-nostdlib'] 
kernel_ld_args_relink = ['-nostdlib', '-gc-sections']

subdir('libkern')
subdir('core')

if (use_limine)
  subdir('platform/limine-generic')
endif

if (arch == 'amd64')
  subdir('platform/amd64')
endif

azalea_lib = static_library('azalea_lib',
  kernel_sources,
  c_args: [freestanding_c_args, '-std=gnu2x'],
  cpp_args: [freestanding_cpp_args, '-std=gnu++23'],
  include_directories: [freestanding_include_dirs],
  link_args: [kernel_ld_args, '-Wl,-r'],
  dependencies: [
    kernel_deps
  ],
  build_by_default : false
)

ao = azalea_lib.extract_objects(kernel_sources)

azalea_partial = custom_target('azalea_partial',
  input : ao,
  output : 'azalea_partial',
  command : ['ld', kernel_ld_args, '-Ur','-o', '@OUTPUT@', '@INPUT@'],
)

map_source = custom_target('generate symbols',
          depends : [azalea_partial, demangle],
          input : azalea_partial,
          output : 'symbols.S',
          command : [meson.source_root()+'/tools/gen-symmap.sh', 
                     '@INPUT@', '@OUTPUT@'],
          build_by_default : false
)

c = find_program('c')
ld = find_program('ld')

map = custom_target('compile symbols',
  input : map_source,
  output : 'symbols.o',
  command : [c, '-c', '@INPUT@', '-o', '@OUTPUT@'],
)

azalea = custom_target('final relink',
  input : [ map, azalea_partial ],
  output : ['azalea'],
  command : [ld, kernel_ld_args_relink, '-o', '@OUTPUT@', '@INPUT@'],
  build_by_default : true
)
