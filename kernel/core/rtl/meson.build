kernel_sources += files('abort.cc', 'cpp-stubs.cc', 'assert.cc')

if conf.get('CONFIG_UBSAN', 'n') == 'y'
  kernel_sources += files('ubsan.cc')
endif

if conf.get('CONFIG_STACKGUARD', 'n') == 'y'
  kernel_sources += files('stack.cc')
endif
