OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS 
{
    . = 0x27800000;
    .text :
    {
        obj/start.o
        *(.text)
    }
    . = ALIGN(4);
    .rodata :  { *(.rodata*) }
    . = ALIGN(4);
    .data :  { *(.data) }
    . = ALIGN(4);
    __bss_start = .;
    .bss ALIGN(4)  :  {*(.bss) *(COMMON)}
    __bss_end = .;
}