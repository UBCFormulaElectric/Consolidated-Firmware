/* Entry Point */
ENTRY(Reset_Handler)

/* Define stack pointer at the top of SRAM3 */
_estack = ORIGIN(SRAM3) + LENGTH(SRAM3);
/* Define minimal heap and stack size */
_Min_Heap_Size = 0x0; /* required amount of heap */
_Min_Stack_Size = 0x400; /* required amount of stack */

/* Define memory regions based on STM32H563 */
MEMORY
{
  FLASH     (rx)  : ORIGIN = 0x08000000, LENGTH = 2048K
  SRAM1_2   (xrw) : ORIGIN = 0x20000000, LENGTH = 256K   /* SRAM1 + SRAM2 */
  SRAM3     (xrw) : ORIGIN = 0x30000000, LENGTH = 320K
  BACKUPRAM (xrw) : ORIGIN = 0x38800000, LENGTH = 4K 
}

/* Output sections */
SECTIONS
{
  /* The startup code goes first into FLASH */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector)) /* Startup code */
    . = ALIGN(4);
  } >FLASH

  .text :
  {
    . = ALIGN(4);
    *(.text)            /* .text sections (code)  */
    *(.text*)           /* .text* sections (code) */
    *(.glue_7)          /* glue arm to thumb code */
    *(.glue_7t)         /* glue thumb to arm code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
    _etext = .;         /* define a global symbols at end of code */
  } >FLASH

  .rodata :
  {
    . = ALIGN(4);       
    *(.rodata)          /* .rodata* sections (constants, strings, etc.) *
    *(.rodata*)         /* .rodata sections (constants, strings, etc.) */
    . = ALIGN(4);
  } >FLASH

  .ARM.extab : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >FLASH
  .ARM : {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } >FLASH

  .preinit_array :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >FLASH

  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*))) KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >FLASH

  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*))) KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >FLASH

  /* used by the startup to initialize data */
  _sidata = LOADADDR(.data);

  /* Initialized data sections goes into RAM, load LMA copy after code */
  .data : AT (LOADADDR(.text) + SIZEOF(.text) + SIZEOF(.rodata))
  {
    . = ALIGN(4);
    _sdata = .;
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */
    *(.RamFunc)        /* .RamFunc sections */
    *(.RamFunc*)       /* .RamFunc* sections */

    . = ALIGN(4);
    _edata = .;        /* define a global symbol at data end */
  } >SRAM1_2

  .bss :
  {
    . = ALIGN(4);
    _sbss = .;
    __bss_start__ = _sbss;
    *(.bss) 
    *(.bss*)
    *(COMMON)
    
    . = ALIGN(4);
    _ebss = .;          /* define a global symbol at bss end */
    __bss_end__ = _ebss;
  } >SRAM1_2

  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } >SRAM1_2

  /* Backup RAM section */
  .backup_bss (NOLOAD) :
  {
    *(.bkup)
  } >BACKUPRAM

  /DISCARD/ : 
  { 
    libc.a ( * )
    libm.a ( * ) 
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
}
