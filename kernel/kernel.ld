/* We want the symbol _start to be our entry point */
ENTRY(_start)

/* Define the program headers we want so the bootloader gives us the right */
/* MMU permissions */
PHDRS
{
    text PT_LOAD;
    rodata PT_LOAD;
    data PT_LOAD;
    dynamic PT_LOAD;
}

SECTIONS
{
    . = 0;

    PROVIDE_HIDDEN(addr_text_start = .);

    .text : {
        *(.text .text.*)
    } :text

    PROVIDE_HIDDEN(addr_text_end = .);

    /* Move to the next memory page for .rodata */
    . = ALIGN(CONSTANT(MAXPAGESIZE));
    
    PROVIDE_HIDDEN(addr_rodata_start = .);

    .rodata : {
        *(.rodata .rodata.*)
    } :rodata

    .note : {
        *(.note .note.*)
    } :rodata


    PROVIDE_HIDDEN(addr_rodata_end = .);

    /* Move to the next memory page for .data */
    . = ALIGN(CONSTANT(MAXPAGESIZE));

    PROVIDE_HIDDEN(addr_data_start = .);

    .data : {
        *(.data .data.*)

        KEEP(*(.requests_start_marker))
        KEEP(*(.requests))
        KEEP(*(.requests_end_marker))
    } :data

    /* Dynamic section for relocations, both in its own PHDR and inside data PHDR */
    .dynamic : {
        *(.dynamic)
    } :data :dynamic

    /* NOTE: .bss needs to be the last thing mapped to :data, otherwise lots of */
    /* unnecessary zeros will be written to the binary. */
    /* If you need, for example, .init_array and .fini_array, those should be placed */
    /* above this. */
    .bss : {
        *(.bss .bss.*)
        *(COMMON)
    } :data

    PROVIDE_HIDDEN(addr_data_end = .);


    /* Discard .note.* and .eh_frame since they may cause issues on some hosts. */
    /DISCARD/ : {
        *(.eh_frame)
    }
}
