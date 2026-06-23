ENTRY(_start)





PHDRS
{
    text    PT_LOAD;
    rodata  PT_LOAD;
    data    PT_LOAD;
}






SECTIONS
{
    . = 0x0006E000;

    .text : {
        __stext = .;
        KEEP(*(.text.boot));
        KEEP(*(.text.vectors));
        *(.text*);
        __etext = .;
    } :text

    . = ALIGN(CONSTANT(MAXPAGESIZE));

    .rodata : {
        __srodata = .;
        *(.rodata*);
        __erodata = .;
    } :rodata

    . = ALIGN(CONSTANT(MAXPAGESIZE));

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

        __sstack = .;
        . += 65536;
        __estack = .;
    } :data

    /DISCARD/ : {
        *(.note)
        *(.comment)
        *(.eh_frame)
    }
}