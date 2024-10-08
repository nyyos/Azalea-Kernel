.text
.global _gdt_load

// rdi = gdtr
// rsi = cs
// rdx = ss
_gdt_load:
  lgdt (%rdi)
  push %rsi
  lea _reload_ds(%rip), %rax
  push %rax
  lretq
_reload_ds:
  mov %dx, %ax
  mov %ax, %ds
  mov %ax, %es
  mov %ax, %ss
  // gs stores our cpulocals
  mov %ax, %fs
  ret
