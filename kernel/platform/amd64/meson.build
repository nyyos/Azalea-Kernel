kernel_ld_args += [
  '-static',
    '-Wl,-zmax-page-size=0x1000',
    '-Wl,-T' + meson.current_source_dir() + '/linker.ld'
]

kernel_sources += files('entry.S')
