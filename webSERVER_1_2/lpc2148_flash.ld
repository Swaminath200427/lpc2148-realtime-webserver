MEMORY
{
    FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 512K  /* 512 KB Flash memory */
    RAM   (rwx) : ORIGIN = 0x40000000, LENGTH = 32K   /* 32 KB RAM */
}

/* Define the sections in memory */
SECTIONS
{
    /* Start of the code section */
    .text :
    {
        KEEP(*(.vectors))       /* Keep the interrupt vectors */
        *(.text*)               /* All .text sections */
        *(.rodata*)             /* Read-only data sections */
        . = ALIGN(4);           /* Align to 4 bytes */
        _etext = .;             /* End of text section */
    } > FLASH

    /* Data section loaded from flash to RAM */
    .data : AT (_etext)
    {
        _data_start = .;        /* Start of data section */
        *(.data*)               /* All .data sections */
        . = ALIGN(4);           /* Align to 4 bytes */
        _data_end = .;          /* End of data section */
    } > RAM

    /* BSS section for uninitialized data */
    .bss :
    {
        _bss_start = .;         /* Start of BSS section */
        *(.bss*)                /* All .bss sections */
        *(COMMON)               /* Common symbols */
        . = ALIGN(4);           /* Align to 4 bytes */
        _bss_end = .;           /* End of BSS section */
        PROVIDE(end = .);       /* Provide a symbol for the end */
    } > RAM

    /* Add a section for the ARM exception index, if required */
    .ARM.exidx : 
    {
        *(.ARM.exidx*)
    } > RAM

    /* Define load address for data section */
    _data_load = LOADADDR(.data);
}
