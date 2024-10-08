kernel_ld_args += [
    '-static',
    '-zmax-page-size=0x1000',
    '-T' + meson.current_source_dir() + '/linker.ld'
]

kernel_ld_args_relink += [
  '-static',
  '-zmax-page-size=0x1000',
  '-T' + meson.current_source_dir() + '/linker.ld'
]

kernel_sources += files('entry.S', 'gdt.S')

kernel_sources += files(
  'plat.cc', 
  'crash.cc', 
  'gdt.cc'
)
