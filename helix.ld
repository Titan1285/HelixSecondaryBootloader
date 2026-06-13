ENTRY(_start)





PHDRS
{
    text    PT_LOAD;
    rodata  PT_LOAD;
    data    PT_LOAD;
}






SECTIONS
{
    /* NOTE: update address if this changes in ROM */
    . = 0x00022450;

    .text : {
        __stext = .;
        KEEP(*(.text.boot));
        *(.text);
        __etext = .;
    } :text

    /* Vector table must come after. Linker seems to pull weird stunts otherwise */
    .vectors : {
        __svectors = .;
        KEEP(*(.vectors));
        __evectors = .;
    } :text

    .rodata : {
        __srodata = .;
        *(.rodata);
        __erodata = .;
    } :rodata

    .data : {
        __sdata = .;
        *(.data*)
        __edata = .;
    } :data

    .bss (NOLOAD) : {
        __sbss = .;
        *(.bss*);
        *(COMMON);
        __ebss = .;
    } :data

    /DISCARD/ : {
        *(.note)
        *(.comment)
        *(.eh_frame)
    }
}